#include "app.h"
#include "ui/ui_common.h"
#include "ui/ui_header.h"
#include "ui/ui_footer.h"
#include "ui/ui_navigation.h"
#include "ui/pages/ui_status_page.h"
#include "data/data_manager.h"

void app_init(void) {
    // Initialize common UI components
    ui_common_init();

    // Create header and footer
    ui_header_create();
    ui_footer_create();

    // Initialize navigation menu
    ui_navigation_init();

    // Show the default page (Status Page)
    ui_status_page_create();

    // Initialize data manager
    data_manager_init();

    // Start timers for periodic updates
    start_timers();
}

void start_timers(void) {
    // Start timers for data updates, clock updates, etc.
    lv_timer_create(update_clock, 1000, NULL);         // Update clock every second
    lv_timer_create(data_manager_update, 100, NULL);   // Update data every 100 ms
}
