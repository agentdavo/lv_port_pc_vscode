#ifndef UI_STYLES_H
#define UI_STYLES_H

#include "lvgl.h"  // Include the LVGL header for types like lv_style_t

// Declare global styles for UI components
extern lv_style_t style_bg;
extern lv_style_t style_btn;
extern lv_style_t style_btn_pressed;
extern lv_style_t style_label;
extern lv_style_t style_indicator;
extern lv_style_t style_status_indicator;
extern lv_style_t style_panel;
extern lv_style_t style_heading;

// Function to initialize all styles
void styles_init(void);

#endif // UI_STYLES_H
