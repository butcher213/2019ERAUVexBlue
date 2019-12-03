#include <cstring>
#include <sstream>
#include <cmath>
#include "main.h"
#include "Robot.hpp"


static pros::Controller primary_controller(pros::E_CONTROLLER_MASTER);
static int lcdMenuSelection = 0;

void on_center_button() {
    switch (lcdMenuSelection) {
        case 0:
        break;
    }
}

void on_right_button() {
    lcdMenuSelection++;
}

void on_left_button() {
    lcdMenuSelection--;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();

	pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn2_cb(on_right_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
