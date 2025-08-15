/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>
#include "util.h"

struct zmk_widget_hid_indicators {
    sys_snode_t node;
    lv_obj_t *obj;
    lv_color_t cbuf[BUFFER_SIZE * BUFFER_SIZE];
    lv_obj_t *label;
};

int zmk_widget_hid_indicators_init(struct zmk_widget_hid_indicators *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_hid_indicators_obj(struct zmk_widget_hid_indicators *widget);