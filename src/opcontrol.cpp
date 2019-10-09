#include "main.h"
#include "Robot.hpp"


/*
float max = max(x, y)

float maxX = x / max * 127
float maxY = y / max * 127

float newX = r/R * maxX
float newY = r/R * maxY

// r/R magnitude / 127
*/


#define ROBOT_CONTROL_FORWARD (pros::E_CONTROLLER_ANALOG_LEFT_Y)
#define ROBOT_CONTROL_STRAFE  (pros::E_CONTROLLER_ANALOG_LEFT_X)
#define ROBOT_CONTROL_ROTATE  (pros::E_CONTROLLER_ANALOG_RIGHT_X)

#define ROBOT_CONTROL_TRAY_RAISE (pros::E_CONTROLLER_DIGITAL_A)
#define ROBOT_CONTROL_TRAY_LOWER (pros::E_CONTROLLER_DIGITAL_B)

#define ROBOT_CONTROL_LIFT_RAISE (pros::E_CONTROLLER_DIGITAL_L1)
#define ROBOT_CONTROL_LIFT_LOWER (pros::E_CONTROLLER_DIGITAL_L2)

#define ROBOT_CONTROL_INTAKE_PULL (pros::E_CONTROLLER_DIGITAL_R2)
#define ROBOT_CONTROL_INTAKE_PUSH (pros::E_CONTROLLER_DIGITAL_R1)


//******************************
// Primary controller definition
//******************************
static pros::Controller primary_controller(pros::E_CONTROLLER_MASTER);

//********************************
// Secondary controller definition
//********************************
static pros::Controller secondary_controller(pros::E_CONTROLLER_PARTNER);


void driveControl() {
    // Robot::singleton().forward(primary_controller.get_analog(ROBOT_CONTROL_FORWARD));
    // Robot::singleton().strafe(primary_controller.get_analog(ROBOT_CONTROL_STRAFE));
    // Robot::singleton().rotate(primary_controller.get_analog(ROBOT_CONTROL_ROTATE));
    Robot::singleton().drive(primary_controller.get_analog(ROBOT_CONTROL_FORWARD),
                             primary_controller.get_analog(ROBOT_CONTROL_STRAFE),
                             primary_controller.get_analog(ROBOT_CONTROL_ROTATE));
}

void trayControl() {
    int trayActuatorVelocity;

    if (primary_controller.get_digital(ROBOT_CONTROL_TRAY_RAISE))
        trayActuatorVelocity = Robot::TRAY_RAISE;
    else if (primary_controller.get_digital(ROBOT_CONTROL_TRAY_LOWER))
        trayActuatorVelocity = Robot::TRAY_LOWER;
    else
        trayActuatorVelocity = 0;

    Robot::singleton().actuateTray(trayActuatorVelocity);
}

void liftControl() {
    int liftVelocity;

    if (primary_controller.get_digital(ROBOT_CONTROL_LIFT_RAISE))
        liftVelocity = Robot::LIFT_RAISE;
    else if (primary_controller.get_digital(ROBOT_CONTROL_LIFT_LOWER))
        liftVelocity = Robot::LIFT_LOWER;
    else
        liftVelocity = 0;

    Robot::singleton().liftVelocity(liftVelocity);
}

void intakeControl() {
    int intakeVelocity;

    if (primary_controller.get_digital(ROBOT_CONTROL_INTAKE_PULL))
        intakeVelocity = Robot::INTAKE_PULL;
    else if (primary_controller.get_digital(ROBOT_CONTROL_INTAKE_PUSH))
        intakeVelocity = Robot::INTAKE_PUSH * 0.5;
    else
        intakeVelocity = 0;

    Robot::singleton().intakeVelocity(intakeVelocity);
}


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    while (true) {
        driveControl();

        trayControl();

        liftControl();

        intakeControl();

		pros::delay(10);
	}
}

class DampenedMotor: public pros::Motor {
public:
    std::int32_t move_dampened(const std::int8_t voltage) {

    }
};
