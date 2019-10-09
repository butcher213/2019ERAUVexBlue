#include <cstring>
#include <sstream>
#include <cmath>
#include "main.h"
#include "Robot.hpp"
#include "Bash.hpp"


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
	// pros::lcd::initialize();
	// pros::lcd::set_text(1, "Hello PROS User!");
    //
	// pros::lcd::register_btn0_cb(on_left_button);
	// pros::lcd::register_btn1_cb(on_center_button);
	// pros::lcd::register_btn2_cb(on_right_button);

    Bash::initGUI();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}


void mapJoyInput(int x, int y) {
   float max = std::max(std::abs(x), std::abs(y));

   if (max != 0) {
       float r = std::sqrt(x*x + y*y);
       int x_ = r / max * x;
       int y_ = r / max * y;

       printf(" | %4d  %4d", x_, y_);
       if (r > 127) {
           std::cout << "<";
       }
       else {
           std::cout << " ";
       }
   }
   else {
       std::cout << " |    -     - ";
   }
}
/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
    static const int START = -127;
    static const int STOP = 127;
    static const float STEP = 127/8;

    std::cout << "   \\ X";
    for (float x = START; x <= STOP; x += STEP)
        printf("%7d       ", (int) x);
    std::cout << "  Y \\" << std::endl;

    for (float y = START; y <= STOP; y += STEP) {
        printf("%4d", (int) y);
        for (float x = START; x <= STOP; x += STEP) {
            mapJoyInput(x, y);
        }
        std::cout << std::endl << std::endl;
    }


    while (1) {
        float x = primary_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
        float y = primary_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        float r = sqrt(x*x + y*y);

        float max = std::max(std::abs(x), std::abs(y));

        if (max != 0) {
            pros::delay(1000);
            // float maxX = x / max * 127;
            // float maxY = y / max * 127;

            // float newX = r/127 * maxX;
            // float newY = r/127 * maxY;

            // simplified version
            float x_ = r / max * x;
            float y_ = r / max * y;

            std::ostringstream str1;
            std::ostringstream str2;
            str1 << "x: " << x_;
            str2 << "y: " << y_;

            pros::lcd::set_text(0, str1.str());
            pros::lcd::set_text(1, str2.str());
        }
    }
}
