#ifndef _BUTTON_HPP_
#define _BUTTON_HPP_


#include "main.h"


class Button {
public:
    // LvGL button object
    lv_obj_t *btn;

    // Builds the button
    Button(const char *name, int x, int y, int w, int h, lv_obj_t *parent = lv_scr_act());
    // frees resources in the button
    ~Button();

    // equivalent to lv_btn_get_state(button.btn);
    lv_btn_state_t getState();
};


#endif // _BUTTON_HPP_
