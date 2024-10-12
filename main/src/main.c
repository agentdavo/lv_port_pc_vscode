
/**
 * @file main
 *
 * @brief Integration of TinyGL with LVGL using LVGL's Canvas.
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "lv_conf.h"
#include "lvgl/lvgl.h"
#include "glob.h"

#include "tinygl/include/GL/gl.h"
#include "tinygl/include/zbuffer.h"

#define CHAD_API_IMPL
#define CHAD_MATH_IMPL
#include "tinygl/include-demo/3dMath.h"
#include "tinygl/src/font8x8_basic.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

/*********************
 *      DEFINES
 *********************/

#define CANVAS_WIDTH 512
#define CANVAS_HEIGHT 384

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_display_t *hal_init(int32_t w, int32_t h);
void renderPerformanceData(int frameNumber, double totalFrameTime);
double getCurrentTimeInMs();
static void render_timer_cb(lv_timer_t *timer);


/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * canvas = NULL;
static uint8_t  * cbuf[LV_CANVAS_BUF_SIZE(CANVAS_WIDTH, CANVAS_HEIGHT, 32, LV_DRAW_BUF_STRIDE_ALIGN)];

/**********************
 *   GLOBAL VARIABLES
 **********************/
ZBuffer *frameBuffer = NULL;

/*********************
 *   GLOBAL FUNCTIONS
 *********************/
extern void freertos_main(void);

#ifndef M_PI
#define M_PI 3.14159265
#endif

/**
 * @brief Retrieves the current system time in milliseconds.
 *
 * @return Current time in milliseconds as a double.
 */
double getCurrentTimeInMs()
{
#ifdef _WIN32
    // Windows implementation using QueryPerformanceCounter
    LARGE_INTEGER frequency;
    LARGE_INTEGER currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&currentTime);
    return (double)(currentTime.QuadPart * 1000) / frequency.QuadPart;
#else
    // POSIX implementation using gettimeofday
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)(time.tv_sec) * 1000.0 + (double)(time.tv_usec) / 1000.0;
#endif
}

/*
   Draw a gear wheel.  You'll probably want to call this function when
   building a display list since we do a lot of trig here.

   Input:  inner_radius - radius of hole at center
           outer_radius - radius at center of teeth
           width - width of gear
           teeth - number of teeth
           tooth_depth - depth of tooth
*/
static void
gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
     GLint teeth, GLfloat tooth_depth)
{
    GLint i;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;

    r0 = inner_radius;
    r1 = outer_radius - tooth_depth / 2.f;
    r2 = outer_radius + tooth_depth / 2.f;

    da = 2.f * (float)M_PI / teeth / 4.f;

    glShadeModel(GL_FLAT);

    glNormal3f(0.f, 0.f, 1.f);

    /* draw front face */
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= teeth; i++)
    {
        angle = i * 2.f * (float)M_PI / teeth;
        glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), width * 0.5f);
        glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), width * 0.5f);
        if (i < teeth)
        {
            glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), width * 0.5f);
            glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), width * 0.5f);
        }
    }
    glEnd();

    /* draw front sides of teeth */
    glBegin(GL_QUADS);
    da = 2.f * (float)M_PI / teeth / 4.f;
    for (i = 0; i < teeth; i++)
    {
        angle = i * 2.f * (float)M_PI / teeth;

        glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), width * 0.5f);
        glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), width * 0.5f);
        glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), width * 0.5f);
        glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), width * 0.5f);
    }
    glEnd();

    glNormal3f(0.0, 0.0, -1.0);

    /* draw back face */
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= teeth; i++)
    {
        angle = i * 2.f * (float)M_PI / teeth;
        glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -width * 0.5f);
        glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -width * 0.5f);
        if (i < teeth)
        {
            glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -width * 0.5f);
            glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -width * 0.5f);
        }
    }
    glEnd();

    /* draw back sides of teeth */
    glBegin(GL_QUADS);
    da = 2.f * (float)M_PI / teeth / 4.f;
    for (i = 0; i < teeth; i++)
    {
        angle = i * 2.f * (float)M_PI / teeth;

        glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -width * 0.5f);
        glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), -width * 0.5f);
        glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), -width * 0.5f);
        glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -width * 0.5f);
    }
    glEnd();

    /* draw outward faces of teeth */
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i < teeth; i++)
    {
        angle = i * 2.f * (float)M_PI / teeth;

        glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), width * 0.5f);
        glVertex3f(r1 * (float)cos(angle), r1 * (float)sin(angle), -width * 0.5f);
        u = r2 * (float)cos(angle + da) - r1 * (float)cos(angle);
        v = r2 * (float)sin(angle + da) - r1 * (float)sin(angle);
        len = (float)sqrt(u * u + v * v);
        u /= len;
        v /= len;
        glNormal3f(v, -u, 0.0);
        glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), width * 0.5f);
        glVertex3f(r2 * (float)cos(angle + da), r2 * (float)sin(angle + da), -width * 0.5f);
        glNormal3f((float)cos(angle), (float)sin(angle), 0.f);
        glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), width * 0.5f);
        glVertex3f(r2 * (float)cos(angle + 2 * da), r2 * (float)sin(angle + 2 * da), -width * 0.5f);
        u = r1 * (float)cos(angle + 3 * da) - r2 * (float)cos(angle + 2 * da);
        v = r1 * (float)sin(angle + 3 * da) - r2 * (float)sin(angle + 2 * da);
        glNormal3f(v, -u, 0.f);
        glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), width * 0.5f);
        glVertex3f(r1 * (float)cos(angle + 3 * da), r1 * (float)sin(angle + 3 * da), -width * 0.5f);
        glNormal3f((float)cos(angle), (float)sin(angle), 0.f);
    }

    glVertex3f(r1 * (float)cos(0), r1 * (float)sin(0), width * 0.5f);
    glVertex3f(r1 * (float)cos(0), r1 * (float)sin(0), -width * 0.5f);

    glEnd();

    glShadeModel(GL_SMOOTH);

    /* draw inside radius cylinder */
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= teeth; i++)
    {
        angle = i * 2.f * (float)M_PI / teeth;
        glNormal3f(-(float)cos(angle), -(float)sin(angle), 0.f);
        glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), -width * 0.5f);
        glVertex3f(r0 * (float)cos(angle), r0 * (float)sin(angle), width * 0.5f);
    }
    glEnd();
}

static GLfloat view_rotx = 20.0f, view_roty = 30.0f, view_rotz = 0.0f;
static GLint gear1, gear2, gear3;
static GLfloat angle = 0.0f;

/**
 * @brief Draw the gears on the screen.
 */
void draw_gears(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotatef(view_rotx, 1.0, 0.0, 0.0);
    glRotatef(view_roty, 0.0, 1.0, 0.0);
    glRotatef(view_rotz, 0.0, 0.0, 1.0);

    glPushMatrix();
    glTranslatef(-3.0, -2.0, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glCallList(gear1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.1f, -2.f, 0.f);
    glRotatef(-2.f * angle - 9.f, 0.f, 0.f, 1.f);
    glCallList(gear2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.1f, 4.2f, 0.f);
    glRotatef(-2.f * angle - 25.f, 0.f, 0.f, 1.f);
    glCallList(gear3);
    glPopMatrix();

    glPopMatrix();
}

/**
 * @brief Initialize the gears by creating display lists and setting up materials.
 */
void init_gears(void)
{
    static GLfloat pos[4] = {5.f, 5.f, 10.f, 0.f};
    static GLfloat red[4] = {1.0, 0.0, 0.0, 0.0};
	static GLfloat green[4] = {0.0, 1.0, 0.0, 0.0};
	static GLfloat blue[4] = {0.0, 0.0, 1.0, 0.0};
	static GLfloat white[4] = {1.0, 1.0, 1.0, 0.0};
	static GLfloat shininess = 5;

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    /* make the gears */
    gear1 = glGenLists(1);
    glNewList(gear1, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
    glColor3fv(blue);
    gear(1.f, 4.f, 1.f, 20, 0.7f);
    glEndList();

    gear2 = glGenLists(1);
    glNewList(gear2, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	glColor3fv(red);
    gear(0.5f, 2.f, 2.f, 10, 0.7f);
    glEndList();

    gear3 = glGenLists(1);
    glNewList(gear3, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	glColor3fv(green);
    gear(1.3f, 2.f, 0.5f, 10, 0.7f);
    glEndList();

    glEnable(GL_NORMALIZE);
}

/**
 * @brief Renders FPS and performance data on the screen.
 *
 * @param frameNumber    The current frame number.
 * @param totalFrameTime Total time taken for the frame (in milliseconds).
 */
void renderPerformanceData(int frameNumber, double totalFrameTime)
{

    // Set up orthographic projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Set text size and color
    glTextSize(GL_TEXT_SIZE16x16);   // Ensure glTextSize is defined and accessible
    unsigned int color = 0xFFFFFFFF; // White color with full alpha

    // Prepare the text to display, line by line
    char textBuffer1[128];
    char textBuffer2[128];

    // Fill the buffers with relevant text
    snprintf(textBuffer1, sizeof(textBuffer1), "FRM: %d", frameNumber + 1);
    snprintf(textBuffer2, sizeof(textBuffer2), "FPS: %.1f", (totalFrameTime > 0.0) ? 1000.0 / totalFrameTime : 0.0);

    // Render each line of text at the desired positions
    int x = 10; // Position from the left
    int y = 10; // Position from the top, increase for each new line

    glDrawText((unsigned char *)textBuffer1, x, y, color);      // First line: Frame Number
    glDrawText((unsigned char *)textBuffer2, x, y + 20, color); // Second line: FPS

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

/* Copy TinyGL framebuffer (ARGB8888) to LVGL buffer (XRGB8888), processing 4 pixels at a time */
static void ZB_copyFrameBufferLVGL(ZBuffer *zb, lv_color32_t *lv_buf)
{
    uint32_t *q_ptr = (uint32_t *)frameBuffer->pbuf;  // Pointer to TinyGL framebuffer (ARGB8888 format)
    lv_color32_t *p_ptr = lv_buf;                     // Pointer to LVGL buffer (XRGB8888 format)

    int total_pixels = frameBuffer->xsize * frameBuffer->ysize;
    int i;

    // Process 4 pixels at a time
    for (i = 0; i <= total_pixels - 4; i += 4)
    {
        // Pixel 1
        uint32_t pixel1 = q_ptr[i];
        p_ptr[i].red   = (pixel1 >> 16) & 0xFF;
        p_ptr[i].green = (pixel1 >> 8) & 0xFF;
        p_ptr[i].blue  = pixel1 & 0xFF;
        p_ptr[i].alpha = 0xFF;

        // Pixel 2
        uint32_t pixel2 = q_ptr[i + 1];
        p_ptr[i + 1].red   = (pixel2 >> 16) & 0xFF;
        p_ptr[i + 1].green = (pixel2 >> 8) & 0xFF;
        p_ptr[i + 1].blue  = pixel2 & 0xFF;
        p_ptr[i + 1].alpha = 0xFF;

        // Pixel 3
        uint32_t pixel3 = q_ptr[i + 2];
        p_ptr[i + 2].red   = (pixel3 >> 16) & 0xFF;
        p_ptr[i + 2].green = (pixel3 >> 8) & 0xFF;
        p_ptr[i + 2].blue  = pixel3 & 0xFF;
        p_ptr[i + 2].alpha = 0xFF;

        // Pixel 4
        uint32_t pixel4 = q_ptr[i + 3];
        p_ptr[i + 3].red   = (pixel4 >> 16) & 0xFF;
        p_ptr[i + 3].green = (pixel4 >> 8) & 0xFF;
        p_ptr[i + 3].blue  = pixel4 & 0xFF;
        p_ptr[i + 3].alpha = 0xFF;
    }

    // Handle any remaining pixels (if total_pixels is not a multiple of 4)
    for (; i < total_pixels; i++)
    {
        uint32_t pixel = q_ptr[i];
        p_ptr[i].red   = (pixel >> 16) & 0xFF;
        p_ptr[i].green = (pixel >> 8) & 0xFF;
        p_ptr[i].blue  = pixel & 0xFF;
        p_ptr[i].alpha = 0xFF;
    }
}


int main(int argc, char **argv)
{
    (void)argc; /* Unused */
    (void)argv; /* Unused */

    printf("Initializing LVGL...\n");
    lv_init();

    printf("Initializing HAL...\n");
    hal_init(CANVAS_WIDTH + 20, CANVAS_HEIGHT + 20);

    printf("Creating LVGL canvas...\n");

    int buf_size = LV_CANVAS_BUF_SIZE(CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_ARGB8888, LV_DRAW_BUF_STRIDE_ALIGN);
    printf("cbuf buffer size: %d\n", buf_size);

    /* Create the LVGL canvas */
    canvas = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_NATIVE);
    lv_canvas_fill_bg(canvas, lv_color_hex3(0x000), LV_OPA_COVER);
    lv_obj_center(canvas);

    /* Initialize TinyGL framebuffer */
    frameBuffer = ZB_open(CANVAS_WIDTH, CANVAS_HEIGHT, ZB_MODE_RGBA, NULL);

    printf("TinyGL framebuffer size: %d x %d\n", frameBuffer->xsize, frameBuffer->ysize);
    printf("Canvas buffer size: %d x %d\n", CANVAS_WIDTH, CANVAS_HEIGHT);

    /* Initialize TinyGL */
    glInit(frameBuffer);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glViewport(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);

    GLfloat h = (GLfloat)CANVAS_HEIGHT / (GLfloat)CANVAS_WIDTH;
    GLfloat xmax, znear, zfar;
    znear = 5.0f;
    zfar = 100.0f;
    xmax = znear * 0.5f;

    glBlendEquation(GL_FUNC_ADD);
    glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-xmax, xmax, -xmax * h, xmax * h, znear, zfar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -20.0);

    static GLfloat pos[4] = {5, 5, 10, 0.0}; // Light at infinity.
	static GLfloat red[4] = {1.0, 0.0, 0.0, 0.0};
	static GLfloat green[4] = {0.0, 1.0, 0.0, 0.0};
	static GLfloat blue[4] = {0.0, 0.0, 1.0, 0.0};
	static GLfloat white[4] = {1.0, 1.0, 1.0, 0.0};
	static GLfloat shininess = 5;
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHT0);

    printf("Initializing gears...\n");

    // Initialize and create the gears
    init_gears();

    /* Set up the LVGL timer for rendering */
    lv_timer_create(render_timer_cb, 1, NULL);

    #if LV_USE_OS == LV_OS_NONE

    while (1)
    {
        /* Periodically call the lv_task handler.
        * It could be done in a timer interrupt or an OS task too.*/
        lv_timer_handler();
        usleep(5 * 1000);
    }

    #elif LV_USE_OS == LV_OS_FREERTOS

    /* Run FreeRTOS and create LVGL task */
    freertos_main();

    #endif

    /* Cleanup resources (This part may never be reached in embedded systems) */
    glDeleteLists(gear1, 1);
    glDeleteLists(gear2, 1);
    glDeleteLists(gear3, 1);
    ZB_close(frameBuffer);
    lv_obj_del(canvas);
    free(cbuf);

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{

  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
}

/**
 * @brief Timer callback to render the TinyGL scene and update the display.
 *
 * @param timer Pointer to the LVGL timer (unused in this case).
 */

static int frameNumber = 0;
static double totalFrameTime = 0.0;
static double lastFpsUpdateTime = 0.0;

static void render_timer_cb(lv_timer_t *timer)
{
    (void)timer; // Unused

    double currentTime = getCurrentTimeInMs();
    double deltaTime = currentTime - lastFpsUpdateTime;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Update TinyGL scene */
    draw_gears();

    double fps = frameNumber * 1000.0 / deltaTime;
    renderPerformanceData(frameNumber, deltaTime);
    lastFpsUpdateTime = currentTime;

    /* Update angle for animation */
    angle += 1.0f;
    if (angle > 360.0f)
        angle -= 360.0f;

    // Copy TinyGL framebuffer to LVGL buffer
    ZB_copyFrameBufferLVGL(frameBuffer, (lv_color32_t *)cbuf);

    /* Increment frame number */
    frameNumber++;

    // Invalidate the canvas to trigger a redraw in LVGL
    lv_obj_invalidate(canvas);
}
