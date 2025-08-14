/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include "hid_indicators.h"
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/hid_indicators_changed.h>
#include "../assets/custom_fonts.h"

#define LED_NLCK 0x01
#define LED_CLCK 0x02
#define LED_SLCK 0x04


static lv_obj_t *hid_anim =
    NULL; // Variable estática para almacenar el objeto animado

struct hid_indicators_state {
  uint8_t hid_indicators;
};

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

// /* WORK FINE
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

    // lv_label_set_text(label, text);
    // lv_obj_align(label, LV_ALIGN_OUT_TOP_LEFT, 3, 22); // point
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_BACKGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_CENTER);
    lv_obj_t *canvas = lv_canvas_create(widget->obj);
    lv_obj_align(canvas, LV_ALIGN_TOP_RIGHT, BUFFER_OFFSET_MIDDLE, 0); // point
    lv_canvas_set_buffer(canvas, widget->cbuf, BUFFER_SIZE, BUFFER_SIZE, LV_IMG_CF_TRUE_COLOR);
    fill_background(canvas);
    lv_canvas_draw_text(widget->obj, 0, 40, 25, &label_dsc, text);
    rotate_canvas(canvas, widget->cbuf);
}

// static void set_hid_indicators(lv_obj_t *label,
//                                struct hid_indicators_state state) {
//   // char text[14] = "";

//   // Construir el texto según los indicadores activos.
//   // Se agregan las letras correspondientes si cada indicador está activo.

// #if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA_ONLY_CAPSLOCK)
//   if (state.hid_indicators & LED_CLCK) {
// #else
//   if (state.hid_indicators & (LED_CLCK | LED_NLCK | LED_SLCK)) {
// #endif
//     lv_label_set_text(label, "");
//     // WORK FINE!!!
//     // strncat(text, "CAPS", sizeof(text) - strlen(text) - 1);
//     // strncat(text, ".", sizeof(text) - strlen(text) - 1);

//     if (!hid_anim) { // Si no existe aún, creamos la animación

//       hid_anim = lv_animimg_create(label);
//       lv_obj_center(hid_anim);

//       // lv_animimg_set_src(hid_anim, (const void **)luna_imgs_bark_90, 10);
//       lv_animimg_set_src(hid_anim, (const void **)luna_imgs_bark_90, 2);
//       lv_animimg_set_duration(
//           hid_anim, CONFIG_NICE_OLED_WIDGET_HID_INDICATORS_LUNA_ANIMATION_MS);
//       lv_animimg_set_repeat_count(hid_anim, LV_ANIM_REPEAT_INFINITE);
//       lv_animimg_start(hid_anim);

//       lv_obj_align(hid_anim, LV_ALIGN_TOP_LEFT, 100, 15);
//       // lv_obj_align(hid_anim, LV_ALIGN_TOP_LEFT, 36, 0);
//       // lv_obj_align(hid_anim, LV_ALIGN_TOP_LEFT, 33, 3);
//     }
//   } else {
//     // Si LED_CLCK no está activo, y se había creado la animación, la eliminamos
//     if (hid_anim) {
//       lv_obj_del(hid_anim);
//       hid_anim = NULL;
//     }
//     lv_label_set_text(label, "");
//   }
// }

void hid_indicators_update_cb(struct hid_indicators_state state) {
  struct zmk_widget_hid_indicators *widget;
  SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
    set_hid_indicators(widget, state);
  }
}

static struct hid_indicators_state
hid_indicators_get_state(const zmk_event_t *eh) {
  struct zmk_hid_indicators_changed *ev = as_zmk_hid_indicators_changed(eh);
  return (struct hid_indicators_state){
      .hid_indicators = ev->indicators,
  };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_hid_indicators, struct hid_indicators_state,
                            hid_indicators_update_cb, hid_indicators_get_state)

ZMK_SUBSCRIPTION(widget_hid_indicators, zmk_hid_indicators_changed);

int zmk_widget_hid_indicators_init(struct zmk_widget_hid_indicators *widget,
                                   lv_obj_t *parent) {
  widget->obj = lv_label_create(parent);

  sys_slist_append(&widgets, &widget->node);

  widget_hid_indicators_init();

  return 0;
}

lv_obj_t *zmk_widget_hid_indicators_obj(struct zmk_widget_hid_indicators *widget) {
  return widget->obj;
}