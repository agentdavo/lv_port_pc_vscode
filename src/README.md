# CNC Machine UI Project

## Overview
This project is designed to create a robust, efficient, and user-friendly graphical interface for CNC machines using **LVGL**. The interface consists of multiple screens, each providing specific functionality to a CNC operator. A persistent footer allows quick navigation between key machine operations and probing types.

### File Structure
Here’s an overview of the key files in this project, organized by their folder location:

---

### **`app.c` / `app.h`**
- **Purpose**: The entry point for the application, managing the overall application flow.
- **Details**: Contains initialization of the LVGL library, and sets up the main screen and navigation structure. Coordinates navigation between pages like `status`, `mdi`, `probing`, etc.

---

### **`cnc_communication.c` / `cnc_communication.h`**
- **Purpose**: Manages communication between the CNC machine and the UI.
- **Details**: Handles sending and receiving data between the machine's control system and the user interface (e.g., machine state updates, G-code execution, etc.).

---

### **`cnc_state_machine.c` / `cnc_state_machine.h`**
- **Purpose**: Manages the state machine of the CNC controller.
- **Details**: Ensures the machine transitions between various operational states (e.g., idle, running, error states) and properly reflects these states on the UI.

---

### **`data_manager.c` / `data_manager.h`**
- **Purpose**: Manages data, including machine parameters, offsets, and configurations.
- **Details**: Central hub for loading/saving user data, such as tool offsets, G-code files, or user profiles.

---

### **`lvgl_adapter.c` / `lvgl_adapter.h`**
- **Purpose**: Bridges between LVGL and the system-specific display and input drivers.
- **Details**: Adapts the LVGL rendering engine to the hardware of the CNC machine’s display and manages input from touchscreens or buttons.

---

### **`uistyles.c` / `uistyles.h`**
- **Purpose**: Contains the style definitions for all UI components.
- **Details**: Sets up color schemes, fonts, and general appearance of buttons, labels, headers, and footers.

---

### **`footer_events.c` / `footer_events.h`**
- **Purpose**: Handles events related to the footer.
- **Details**: Processes clicks on footer buttons, ensuring that the correct action is triggered when navigating through different pages or selecting operations like probing.

---

### **`footer_menu.c` / `footer_menu.h`**
- **Purpose**: Implements the persistent footer menu that is always visible at the bottom of the screen.
- **Details**: Handles the creation and updating of the footer with navigation buttons and probing types, ensuring dynamic changes based on the current screen.

---

### **Pages Folder** (`pages`)
- **Purpose**: Contains the specific UI pages for different operations. Each `.c` file implements the logic and UI elements for a different CNC operation or display screen.
  
  - **`ui_cnc_simulation_page.c` / `ui_cnc_simulation_page.h`**: 
    - **Purpose**: Displays a graphical simulation of the CNC toolpath.
    - **Details**: Renders a 3D simulation, showing the tool movements in real-time.

  - **`ui_diagnostics_page.c` / `ui_diagnostics_page.h`**: 
    - **Purpose**: Displays diagnostic information and error messages.
    - **Details**: Shows alarms, error logs, and real-time system health status.

  - **`ui_mdi_page.c` / `ui_mdi_page.h`**: 
    - **Purpose**: Implements Manual Data Input (MDI) mode.
    - **Details**: Allows operators to manually enter G-code commands for direct execution.

  - **`ui_offsets_page.c` / `ui_offsets_page.h`**: 
    - **Purpose**: Displays tool and work offsets.
    - **Details**: Allows operators to view and adjust tool and work offsets, including support for setting offsets based on probing.

  - **`ui_probing_page.c` / `ui_probing_page.h`**: 
    - **Purpose**: Implements different probing cycles (e.g., X, Y, Z axis, center-finding, and corner probing).
    - **Details**: Provides UI for selecting and executing probing operations, with corresponding updates to tool offsets.

  - **`ui_settings_page.c` / `ui_settings_page.h`**: 
    - **Purpose**: Displays machine settings and configurations.
    - **Details**: Allows operators to adjust machine settings like feed rates, spindle speed, and other machine-specific options.

  - **`ui_status_page.c` / `ui_status_page.h`**: 
    - **Purpose**: Displays the overall status of the machine.
    - **Details**: Shows the current operation status, axis positions, spindle speed, and any ongoing alarms or errors.

  - **`ui_visualisation_page.c` / `ui_visualisation_page.h`**: 
    - **Purpose**: Provides a visual toolpath preview.
    - **Details**: Displays a graphical representation of the toolpath before executing the CNC program.

---

### **`ui_common.c` / `ui_common.h`**
- **Purpose**: Contains common utility functions and UI components that are shared across different pages.
- **Details**: Reduces code duplication by implementing reusable UI elements like labels, buttons, and layouts that multiple pages can reference.

---

### **`ui_dashboard.c` / `ui_dashboard.h`**
- **Purpose**: Implements a customizable dashboard for CNC machine operators.
- **Details**: Allows the operator to customize the display with key metrics like cycle times, tool positions, spindle load, etc.

---

### **`ui_footer.c` / `ui_footer.h`**
- **Purpose**: Implements the footer for the application, providing navigation and quick access buttons.
- **Details**: Handles dynamic updates to the footer as the user switches between pages, and provides navigation buttons for probe types or other operations.

---

### **`ui_header.c` / `ui_header.h`**
- **Purpose**: Implements the top header for displaying key machine status information.
- **Details**: Displays program names, cycle start/stop indicators, and active tool number, with real-time updates.

---

### **`ui_navigation.c` / `ui_navigation.h`**
- **Purpose**: Manages the navigation between different pages in the UI.
- **Details**: Ensures smooth transitions between different CNC operations and provides logic for navigating through the pages with footer controls.

---

### **Utils Folder (`utils`)**
- **Purpose**: Contains utility code for configuration, error handling, logging, and user profiles.

  - **`config.c` / `config.h`**: 
    - **Purpose**: Handles loading and saving of configuration files.
    - **Details**: Manages machine settings, user preferences, and system configurations.

  - **`error_handling.c` / `error_handling.h`**: 
    - **Purpose**: Implements the error handling mechanism.
    - **Details**: Logs and displays errors, manages alarms, and offers potential recovery suggestions for common CNC errors.

  - **`logger.c` / `logger.h`**: 
    - **Purpose**: Logs important events and system states.
    - **Details**: Provides a logging interface to track operations, errors, and system status.

  - **`user_profiles.c` / `user_profiles.h`**: 
    - **Purpose**: Manages user-specific settings and profiles.
    - **Details**: Allows CNC operators to save and load personalized settings for machine operations.

---

### **Future Enhancements**
- **Voice Commands**: A potential future feature to add hands-free control via voice commands.
- **Multi-language Support**: Adding support for multiple languages to enhance the global usability of the interface.

---
