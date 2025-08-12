#include <zephyr/kernel.h>
#include "output.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static void draw_usb_connected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, 2, 2, &usb, &img_dsc);
    // lv_canvas_draw_img(canvas, 45, 2, &usb, &img_dsc);
}

static void draw_ble_unbonded(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, 1, 0, &bt_unbonded, &img_dsc);
    // lv_canvas_draw_img(canvas, 44, 0, &bt_unbonded, &img_dsc);
}
#endif

static void draw_ble_disconnected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, 6, 0, &bt_no_signal, &img_dsc);
    // lv_canvas_draw_img(canvas, 49, 0, &bt_no_signal, &img_dsc);
}

static void draw_ble_connected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, 6, 0, &bt, &img_dsc);
    // lv_canvas_draw_img(canvas, 49, 0, &bt, &img_dsc);
}

void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
    // lv_draw_label_dsc_t label_dsc;
    // init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    // lv_canvas_draw_text(canvas, 0, 1, 25, &label_dsc, "SIG");

    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);
    lv_canvas_draw_rect(canvas, 0, 0, 24, 15, &rect_white_dsc);
    // lv_canvas_draw_rect(canvas, 43, 0, 24, 15, &rect_white_dsc);
    
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        draw_usb_connected(canvas);
        break;

    case ZMK_TRANSPORT_BLE:
        if (state->active_profile_bonded) {
            if (state->active_profile_connected) {
                draw_ble_connected(canvas);
            } else {
                draw_ble_disconnected(canvas);
            }
        } else {
            draw_ble_unbonded(canvas);
        }
        break;
    }
#else
    if (state->connected) {
        draw_ble_connected(canvas);
    } else {
        draw_ble_disconnected(canvas);
    }
#endif
}

// peripheral output status
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static void draw_usb_connected_peripheral(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    // Offset Y by 18 for peripheral
    lv_canvas_draw_img(canvas, 2, 20, &usb, &img_dsc);
}

static void draw_ble_unbonded_peripheral(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    // Offset Y by 20
    lv_canvas_draw_img(canvas, 1, 18, &bt_unbonded, &img_dsc);
}
#endif

static void draw_ble_disconnected_peripheral(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    // Offset Y by 18
    lv_canvas_draw_img(canvas, 6, 18, &bt_no_signal, &img_dsc);
}

static void draw_ble_connected_peripheral(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    // Offset Y by 18
    lv_canvas_draw_img(canvas, 6, 18, &bt, &img_dsc);
}

void draw_peripheral_output_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    // Offset Y by 18 for background rect
    lv_canvas_draw_rect(canvas, 0, 18, 24, 15, &rect_white_dsc);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        draw_usb_connected_peripheral(canvas);
        break;

    case ZMK_TRANSPORT_BLE:
        if (state->active_profile_bonded) {
            if (state->active_profile_connected) {
                draw_ble_connected_peripheral(canvas);
            } else {
                draw_ble_disconnected_peripheral(canvas);
            }
        } else {
            draw_ble_unbonded_peripheral(canvas);
        }
        break;
    }
#else
    if (state->connected) {
        draw_ble_connected_peripheral(canvas);
    } else {
        draw_ble_disconnected_peripheral(canvas);
    }
#endif
}