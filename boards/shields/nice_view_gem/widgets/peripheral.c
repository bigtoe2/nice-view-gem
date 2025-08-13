#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_connection_changed.h>
#include <zmk/event_manager.h>
#include <zmk/battery.h>
#include <zmk/events/split_peripheral_battery_state_changed.h>

void on_ble_connected(void) {
    uint8_t battery_level = zmk_battery_state_of_charge();

    struct zmk_peripheral_battery_state_changed evt = {
        .state_of_charge = battery_level,
    };
    ZMK_EVENT_RAISE(&evt);
}

static void ble_connection_changed_cb(const zmk_event_t *eh) {
    const struct zmk_ble_connection_changed *ev = as_zmk_ble_connection_changed(eh);
    if (ev->status == ZMK_BLE_STATUS_CONNECTED) {
        on_ble_connected();
    }
}

ZMK_SUBSCRIPTION(ble_connection_changed_subscriber, zmk_ble_connection_changed);
