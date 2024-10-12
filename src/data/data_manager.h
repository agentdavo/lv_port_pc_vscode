#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "lvgl.h"
#include "../ui/pages/ui_status_page.h"

void data_manager_init(void);
void data_manager_update(lv_timer_t *timer);
void fetch_real_time_data(void);
double get_axis_position(int axis_index);

#endif // DATA_MANAGER_H

