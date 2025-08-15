#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/hid.h>
#include <lvgl.h>
#include "modifiers.h"
#include "../assets/custom_fonts.h"

#include <zephyr/bluetooth/services/bas.h>
#include "hid_indicators.h"
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/hid_indicators_changed.h>

#define LED_NLCK 0x01
#define LED_CLCK 0x02
#define LED_SLCK 0x04

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

struct hid_indicators_state {
  uint8_t hid_indicators;
};

static void set_hid_indicators(struct zmk_widget_hid_indicators *widget, struct hid_indicators_state state) {

    char text[14] = {};
    bool caps_lock_on = false;

    if (state.hid_indicators & LED_CLCK) {
        // strncat(text, "C", 1);
        caps_lock_on = true;
    }
    if (state.hid_indicators & LED_NLCK) {
        // strncat(text, "N", 1);
        caps_lock_on = true;
    }
    if (state.hid_indicators & LED_SLCK) {
        // strncat(text, "S", 1);
        caps_lock_on = true;
    }
    if (caps_lock_on) {
        // strncat(text, "CAPS", 4);
        strncat(text, "CAPSSSSS", 1);
    }

    
    lv_label_set_text(widget->label, idx ? text : "");
    // rotate_canvas(canvas, widget->cbuf);
}

void hid_indicators_update_cb(struct hid_indicators_state state) {
  struct zmk_widget_hid_indicators *widget;
  SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
    set_hid_indicators(widget, state);
  }
}

static struct hid_indicators_state hid_indicators_get_state(const zmk_event_t *eh) {
  struct zmk_hid_indicators_changed *ev = as_zmk_hid_indicators_changed(eh);
  return (struct hid_indicators_state){
      .hid_indicators = ev->indicators,
  };
}


ZMK_DISPLAY_WIDGET_LISTENER(widget_hid_indicators, struct hid_indicators_state,
                            hid_indicators_update_cb, hid_indicators_get_state)

ZMK_SUBSCRIPTION(widget_hid_indicators, zmk_hid_indicators_changed);

int zmk_widget_mod_status_init(struct zmk_widget_mod_status *widget, lv_obj_t *parent)
{
    fill_background(parent);

    parent = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(parent, LVGL_BACKGROUND, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(parent, 255, LV_PART_MAIN);
    
    lv_style_t global_style;
    lv_style_init(&global_style);
    lv_style_set_text_color(&global_style, LVGL_FOREGROUND);
    lv_style_set_text_letter_space(&global_style, 1);
    lv_style_set_text_line_space(&global_style, 1);
    lv_obj_add_style(parent, &global_style, LV_PART_MAIN);

    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 68, 68);

    widget->label = lv_label_create(widget->obj);
    lv_obj_align(widget->label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(widget->label, "-");
    lv_obj_set_style_text_font(widget->label, &pixel_operator_mono, 0); //



    widget_hid_indicators_init();

    return 0;
}

lv_obj_t *zmk_widget_mod_status_obj(struct zmk_widget_mod_status *widget)
{
    return widget->obj;
}