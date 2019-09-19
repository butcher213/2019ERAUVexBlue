#include "Robot.hpp"

//*********************************
// Drive train constant definitions
//*********************************
const int Robot::MOVE_FORWARD = 1; // TODO check value correct
const int Robot::MOVE_BACKWARD = -Robot::MOVE_FORWARD;
const int Robot::STRAFE_RIGHT = 1; // TODO check value correct
const int Robot::STRAFE_LEFT = -Robot::STRAFE_RIGHT;
const int Robot::ROTATE_CW = 1; // TODO check value correct
const int Robot::ROTATE_CCW = -Robot::ROTATE_CW;

//***********************************
// Tray actuator constant definitions
//***********************************
const int Robot::TRAY_RAISE = 1; // TODO check value correct
const int Robot::TRAY_LOWER = -Robot::TRAY_RAISE;

//**************************
// Lift constant definitions
//**************************
const int Robot::LIFT_RAISE = 1; // TODO check value correct
const int Robot::LIFT_LOWER = -Robot::LIFT_RAISE;

//****************************
// Intake constant definitions
//****************************
const int Robot::INTAKE_PULL = 1; // TODO check value correct
const int Robot::INTAKE_PUSH = -Robot::INTAKE_PULL;

//***************************
// Singleton robot definition
//***************************
Robot Robot::robot_singleton = Robot();


// Returns the robot singleton
Robot Robot::singleton() {
    return robot_singleton;
}

int sign(double value) {
    return (int) (value / abs(value));
}

//*********************************
// Drive train function definitions
//*********************************
void Robot::forward(int velocity) {
    frontLeft.move(velocity);
    frontRight.move(velocity);
    backLeft.move(velocity);
    backRight.move(velocity);
}
void Robot::strafe(int velocity) {
    frontLeft.move(velocity);
    frontRight.move(-velocity);
    backLeft.move(-velocity);
    backRight.move(velocity);
}
void Robot::rotate(int velocity) {
    frontLeft.move(velocity);
    frontRight.move(-velocity);
    backLeft.move(velocity);
    backRight.move(-velocity);
}
void Robot::forwardDistance(double inches) {
    double target = inches * ENCODER_COUNTS_PER_INCH;
    int velocity = sign(inches) * 127;

    forward(velocity);

    while (abs(getDriveEncoderValue() - target) < ENCODER_TARGET_RANGE)
        pros::delay(5);

    forward(-velocity);
    pros::delay(100);
    forward(0);
}
void Robot::strafeDistance(double inches) {
    double target = inches * ENCODER_COUNTS_PER_INCH;
    int velocity = sign(inches) * 127;

    strafe(velocity);

    while (abs(getDriveEncoderValue() - target) < ENCODER_TARGET_RANGE)
        pros::delay(5);

    strafe(-velocity);
    pros::delay(100);
    strafe(0);
}
void Robot::rotateDegrees(double degrees) {
    double target = degrees * ENCODER_COUNTS_PER_DEGREE;
    int velocity = sign(degrees) * 127;

    rotate(velocity);

    while (abs(getDriveEncoderValue() - target) < ENCODER_TARGET_RANGE)
        pros::delay(5);

    rotate(-velocity);
    pros::delay(100);
    rotate(0);
}
double Robot::getDriveEncoderValue() {
    double averageCount = (frontLeft.get_position()
                         + frontRight.get_position()
                         + backLeft.get_position()
                         + backRight.get_position()) / 4;

    return averageCount;
}

//***********************************
// Tray actuator function definitions
//***********************************
void Robot::actuateTray(int velocity) {
    actuator.move(velocity);
}

//**************************
// Lift function definitions
//**************************
void Robot::liftVelocity(int velocity) {
    lift.move(velocity);
}

//****************************
// Intake function definitions
//****************************
void Robot::intakeVelocity(int velocity) {
    intakeLeft.move(velocity);
    intakeRight.move(velocity);
}
