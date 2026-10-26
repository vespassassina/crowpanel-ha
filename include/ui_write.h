#pragma once
// LVGL redraws an object on every label text or style write, even when the value is unchanged.
// On this panel a redraw is copied into the framebuffer the LCD is reading, which shows as
// flicker, so the update scripts write only what changed. The helpers take the same arguments
// as the LVGL calls they wrap; a selector other than 0 always writes.
#include <cstring>
#include <lvgl.h>

inline void ui_label_set_text(lv_obj_t *label, const char *text) {
  const char *old = lv_label_get_text(label);
  if (old == nullptr || std::strcmp(old, text) != 0)
    lv_label_set_text(label, text);
}

inline void ui_set_text_color(lv_obj_t *obj, lv_color_t color, lv_style_selector_t sel) {
  if (sel != 0 || !lv_color_eq(lv_obj_get_style_text_color(obj, LV_PART_MAIN), color))
    lv_obj_set_style_text_color(obj, color, sel);
}

inline void ui_set_bg_color(lv_obj_t *obj, lv_color_t color, lv_style_selector_t sel) {
  if (sel != 0 || !lv_color_eq(lv_obj_get_style_bg_color(obj, LV_PART_MAIN), color))
    lv_obj_set_style_bg_color(obj, color, sel);
}

inline void ui_set_bg_opa(lv_obj_t *obj, lv_opa_t opa, lv_style_selector_t sel) {
  if (sel != 0 || lv_obj_get_style_bg_opa(obj, LV_PART_MAIN) != opa)
    lv_obj_set_style_bg_opa(obj, opa, sel);
}
