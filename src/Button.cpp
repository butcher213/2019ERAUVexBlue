#include "Button.hpp"


Button::Button(const char *name, int x, int y, int w, int h, lv_obj_t *parent) {
    btn = lv_btn_create(parent, NULL);
    lv_obj_t *label = lv_label_create(btn, NULL);
    lv_label_set_text(label, name);
    lv_obj_set_size(btn, w, h);
    lv_obj_set_pos(btn, x, y);
}

Button::~Button() {
    lv_obj_del(btn);
    delete btn;
}

lv_btn_state_t Button::getState() {
    return lv_btn_get_state(btn);
}
