#include "data_manager.h"
#include "../cnc/cnc_communication.h"

static double axis_positions[NUM_AXES];

void data_manager_init(void) {
    // Initialize data structures
    for (int i = 0; i < NUM_AXES; i++) {
        axis_positions[i] = 0.0;
    }
}

void data_manager_update(lv_timer_t *timer) {
    // Fetch data from CNC machine
    fetch_real_time_data();

    // Notify UI to update
    update_axis_positions(NULL);
}

void fetch_real_time_data(void) {
    // Placeholder for actual data fetching logic
    // Simulate axis positions
    for (int i = 0; i < NUM_AXES; i++) {
        axis_positions[i] += 0.001; // Simulated movement
    }
}

double get_axis_position(int axis_index) {
    if (axis_index >= 0 && axis_index < NUM_AXES) {
        return axis_positions[axis_index];
    } else {
        return 0.0;
    }
}

