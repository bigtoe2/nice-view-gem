#include <zmk/events/battery_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/battery.h>
#include <zmk/events/split_peripheral_battery_state_changed.h>

void on_ble_connected(void) {
    // Get current battery level
    uint8_t battery_level = zmk_battery_state_of_charge();

    // Create and submit the peripheral battery state changed event
    struct zmk_peripheral_battery_state_changed evt = {
        .state_of_charge = battery_level,
    };
    ZMK_EVENT_RAISE(&evt);
}
