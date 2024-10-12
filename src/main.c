#include <stdio.h>
#include "lvgl.h"
#include "app.h"
#include "styles/ui_styles.h"

void hal_init(void);
void delay_ms(uint32_t ms);

int main(void) {
    // Initialize LVGL
    lv_init();

    // Initialize hardware (display, input devices)
    hal_init();

    // Initialize styles
    styles_init();

    // Initialize application
    app_init();

    // Main loop
    while (1) {
        lv_timer_handler(); // For LVGL v9
        lv_tick_inc(5);     // Increment tick for LVGL timers
        delay_ms(5);        // Platform-specific delay function
    }
    return 0;
}

void hal_init(void) {
    // Initialize display and input drivers here
    // e.g., monitor_init(), mouse_init()
}

void delay_ms(uint32_t ms) {
    // Platform-specific millisecond delay
    // e.g., usleep(ms * 1000);
}
