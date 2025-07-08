#include "ui_styles.h"

lv_style_t style_bg;
lv_style_t style_btn;
lv_style_t style_btn_pressed;
lv_style_t style_label;
lv_style_t style_indicator;
lv_style_t style_status_indicator;
lv_style_t style_panel;
lv_style_t style_heading;

// Initialize the styles
void styles_init(void) {
    // Background Style (general background of the UI)
    lv_style_init(&style_bg);
    lv_style_set_bg_color(&style_bg, lv_color_hex(0x101010));  // Dark background
    lv_style_set_text_color(&style_bg, lv_color_hex(0xFFFFFF));  // White text for all labels by default

    // Button Style (for non-pressed buttons)
    lv_style_init(&style_btn);
    lv_style_set_bg_color(&style_btn, lv_color_hex(0x007ACC));  // Blue color for active buttons
    lv_style_set_text_color(&style_btn, lv_color_hex(0xFFFFFF));  // White text on buttons
    lv_style_set_radius(&style_btn, 8);  // Slightly rounded corners for buttons
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_border_color(&style_btn, lv_color_hex(0xFFFFFF));  // White border

    // Button Pressed Style (for pressed button states)
    lv_style_init(&style_btn_pressed);
    lv_style_set_bg_color(&style_btn_pressed, lv_color_hex(0x005F99));  // Darker blue for button press
    lv_style_set_border_width(&style_btn_pressed, 2);
    lv_style_set_border_color(&style_btn_pressed, lv_color_hex(0xFFFFFF));

    // Label Style (for general text like axis positions and labels)
    lv_style_init(&style_label);
    lv_style_set_text_color(&style_label, lv_color_hex(0xFFFFFF));  // White text
    lv_style_set_text_font(&style_label, &lv_font_montserrat_20);  // Medium-sized font

    // Status Indicator Style (for the "Ready" indicator or other statuses)
    lv_style_init(&style_status_indicator);
    lv_style_set_bg_color(&style_status_indicator, lv_color_hex(0x00FF00));  // Green for ready
    lv_style_set_text_color(&style_status_indicator, lv_color_hex(0x000000));  // Black text inside indicators
    lv_style_set_radius(&style_status_indicator, 5);
    lv_style_set_pad_all(&style_status_indicator, 10);  // Padding for inner text

    // Panel Style (used for areas like the axis display panel or data visualization)
    lv_style_init(&style_panel);
    lv_style_set_bg_color(&style_panel, lv_color_hex(0x202020));  // Darker grey for panel backgrounds
    lv_style_set_border_width(&style_panel, 2);
    lv_style_set_border_color(&style_panel, lv_color_hex(0x707070));  // Light grey borders

    // Indicator Style (for things like axis positions or spindle speed)
    lv_style_init(&style_indicator);
    lv_style_set_text_color(&style_indicator, lv_color_hex(0xFFFF00));  // Yellow for axis/spindle text
    lv_style_set_text_font(&style_indicator, &lv_font_montserrat_28);  // Large font for key data

    // Heading Style (for large headers like "Data Visualization", "Ready", etc.)
    lv_style_init(&style_heading);
    lv_style_set_text_color(&style_heading, lv_color_hex(0xFFFFFF));  // White text
    lv_style_set_text_font(&style_heading, &lv_font_montserrat_24);  // Large font for headings
    lv_style_set_bg_color(&style_heading, lv_color_hex(0x303030));  // Slightly lighter grey for the heading background
    lv_style_set_pad_all(&style_heading, 5);  // Padding around the heading text
}


// Styles for the header labels
void apply_header_styles(void) {
    lv_style_init(&style_header_label);
    lv_style_set_text_color(&style_header_label, lv_color_hex(0xFFFFFF));  // White text
    lv_style_set_bg_color(&style_header_label, lv_color_hex(0x333333));    // Dark background

    lv_style_init(&style_header_status);
    lv_style_set_text_color(&style_header_status, lv_color_hex(0xFFFFFF));
    lv_style_set_bg_color(&style_header_status, lv_color_hex(0xFF0000));  // Red for Error by default

    lv_style_init(&style_ready_status);
    lv_style_set_bg_color(&style_ready_status, lv_color_hex(0x00FF00));  // Green for Ready

    lv_style_init(&style_busy_status);
    lv_style_set_bg_color(&style_busy_status, lv_color_hex(0xFFFF00));  // Yellow for Busy

    lv_style_init(&style_warning_label);
    lv_style_set_text_color(&style_warning_label, lv_color_hex(0xFFAA00));  // Amber for warnings
}
