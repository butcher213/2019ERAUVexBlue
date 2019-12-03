#include "main.h"
#include "Robot.hpp"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
 RampingMotor frontLeft(MOTOR_FRONT_LEFT_PORT, pros::E_MOTOR_GEARSET_18, false, DRIVE_RAMP_SCALE);
 RampingMotor frontRight(MOTOR_FRONT_RIGHT_PORT, pros::E_MOTOR_GEARSET_18, true, DRIVE_RAMP_SCALE);
 RampingMotor backLeft(MOTOR_BACK_LEFT_PORT, pros::E_MOTOR_GEARSET_18, false, DRIVE_RAMP_SCALE);
 RampingMotor backRight(MOTOR_BACK_RIGHT_PORT, pros::E_MOTOR_GEARSET_18, true, DRIVE_RAMP_SCALE);
void forward(int vel) {
    frontLeft.rampedMove(vel);
    frontRight.rampedMove(vel);
    backLeft.rampedMove(vel);
    backRight.rampedMove(vel);
}
void autonomous() {
    forward(Robot::MOVE_FORWARD);
    pros::delay(20000);
    forward(Robot::MOVE_BACKWARD);
    pros::delay(10000);
    Robot::forward(0);


    // Robot::actuateTray(Robot::TRAY_LOWER);
    // pros::delay(1000);
    // Robot::actuateTray(0);
    // pros::delay(500);
    //
    // Robot::armVelocity(Robot::ARM_LOWER);
    // pros::delay(2000);
    // Robot::armVelocity(0);
    // pros::delay(500);
    //
    // Robot::actuateTray(Robot::TRAY_RAISE);
    // pros::delay(1000);
    // Robot::actuateTray(0);
    // pros::delay(500);
    //
    //
    // Robot::armVelocity(Robot::ARM_RAISE);
    // pros::delay(500);
    // Robot::armVelocity(0);
    // pros::delay(500);
    //
    // Robot::intakeVelocity(Robot::INTAKE_PUSH);
}
