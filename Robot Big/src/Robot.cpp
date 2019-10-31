#include "Robot.hpp"

//*********************************
// Drive train constant definitions
//*********************************
const int Robot::MOVE_FORWARD = 127;
const int Robot::MOVE_BACKWARD = -Robot::MOVE_FORWARD;
const int Robot::STRAFE_RIGHT = 127;
const int Robot::STRAFE_LEFT = -Robot::STRAFE_RIGHT;
static const float ROTATE_MULT = .6;
const int Robot::ROTATE_CW = ROTATE_MULT * 127;
const int Robot::ROTATE_CCW = -Robot::ROTATE_CW;

//***********************************
// Tray actuator constant definitions
//***********************************
const int Robot::TRAY_RAISE = 127;
const int Robot::TRAY_LOWER = -Robot::TRAY_RAISE;

//**************************
// Arm constant definitions
//**************************
const int Robot::ARM_RAISE = 127;
const int Robot::ARM_LOWER = -Robot::ARM_RAISE;

//****************************
// Intake constant definitions
//****************************
static float INTAKE_MULT = .75;
const int Robot::INTAKE_PULL = INTAKE_MULT * 127;
const int Robot::INTAKE_PUSH = -Robot::INTAKE_PULL;

//***************************
// Singleton robot definition
//***************************
Robot Robot::robot_singleton = Robot();


// Returns the robot singleton
Robot Robot::singleton() {
    return robot_singleton;
}

// returns the sign of the value
int sign(double value) {
    return (int) (value / abs(value));
}

//*********************************
// Drive train function definitions
//*********************************
void Robot::forward(int velocity) {
    if (abs(velocity) >= DRIVE_MINIMUM_VOLTAGE) {
        frontLeft.move(velocity);
        frontRight.move(velocity);
        backLeft.move(velocity);
        backRight.move(velocity);
    }
}
void Robot::strafe(int velocity) {
    if (abs(velocity) >= DRIVE_MINIMUM_VOLTAGE) {
        frontLeft.move(velocity);
        frontRight.move(-velocity);
        backLeft.move(-velocity);
        backRight.move(velocity);
    }
}
void Robot::rotate(int velocity) {
    if (abs(velocity) >= DRIVE_MINIMUM_VOLTAGE) {
        frontLeft.move(velocity);
        frontRight.move(-velocity);
        backLeft.move(velocity);
        backRight.move(-velocity);
    }
}
void Robot::drive(int forwardVal, int strafeVal, int rotateVal) {
    int fl = forwardVal + strafeVal + rotateVal,
        fr = forwardVal - strafeVal - rotateVal,
        bl = forwardVal - strafeVal + rotateVal,
        br = forwardVal + strafeVal - rotateVal;

    if (fl > 127) fl = 127;
    else if (fl < -127) fl = -127;

    if (fr > 127) fr = 127;
    else if (fr < -127) fr = -127;

    if (bl > 127) bl = 127;
    else if (bl < -127) bl = -127;

    if (br > 127) br = 127;
    else if (br < -127) br = -127;

    frontLeft.move(fl);
    frontRight.move(fr);
    backLeft.move(bl);
    backRight.move(br);
}

//****************************************
// Autonomous control function definitions
//****************************************
void Robot::forwardDistance(double inches) {
    double target = inches * ENCODER_COUNTS_PER_INCH;
    int velocity = sign(inches) * Robot::MOVE_FORWARD;

    forward(velocity);

    while (abs(getDriveEncoderValue() - target) < ENCODER_TARGET_RANGE)
        pros::delay(5);

    forward(-velocity);
    pros::delay(getDriveVelocity() * DRIVE_VELOCITY_TO_BRAKE_TIME_MS);
    forward(0);
}
void Robot::strafeDistance(double inches) {
    double target = inches * ENCODER_COUNTS_PER_INCH;
    int velocity = sign(inches) * Robot::MOVE_FORWARD;

    strafe(velocity);

    while (abs(getDriveEncoderValue() - target) < ENCODER_TARGET_RANGE)
        pros::delay(5);

    strafe(-velocity);
    pros::delay(getDriveVelocity() * DRIVE_VELOCITY_TO_BRAKE_TIME_MS);
    strafe(0);
}
void Robot::rotateDegrees(double degrees) {
    double target = degrees * ENCODER_COUNTS_PER_DEGREE;
    int velocity = sign(degrees) * Robot::MOVE_FORWARD;

    rotate(velocity);

    while (abs(getDriveEncoderValue() - target) < ENCODER_TARGET_RANGE)
        pros::delay(5);

    rotate(-velocity);
    pros::delay(getDriveVelocity() * DRIVE_VELOCITY_TO_BRAKE_TIME_MS);
    rotate(0);
}

//**********************************
// Drive sensor function definitions
//**********************************
double Robot::getDriveEncoderValue() {
    double averageMagnitude = (abs(frontLeft.get_position())
                             + abs(frontRight.get_position())
                             + abs(backLeft.get_position())
                             + abs(backRight.get_position())) / 4;

    int signProduct = sign(frontLeft.get_position())
                    * sign(frontRight.get_position())
                    * sign(backLeft.get_position())
                    * sign(backRight.get_position());

    return signProduct * averageMagnitude;
}
double Robot::getDriveVelocity() {
    double averageMagnitude = (abs(frontLeft.get_actual_velocity())
                             + abs(frontRight.get_actual_velocity())
                             + abs(backLeft.get_actual_velocity())
                             + abs(backRight.get_actual_velocity())) / 4;

    int signProduct = sign(frontLeft.get_actual_velocity())
                    * sign(frontRight.get_actual_velocity())
                    * sign(backLeft.get_actual_velocity())
                    * sign(backRight.get_actual_velocity());

    return signProduct * averageMagnitude;
}

//***********************************
// Tray actuator function definitions
//***********************************
void Robot::actuateTray(int velocity) {
    actuator.move(velocity);
}

//**************************
// Arm function definitions
//**************************
void Robot::armVelocity(int velocity) {
    if (armBottomLimiter.get_value() == HIGH && velocity < 0)
        return;

    armLeft.move(velocity);
    armRight.move(velocity);
}

//****************************
// Intake function definitions
//****************************
void Robot::intakeVelocity(int velocity) {
    intakeLeft.move(velocity);
    intakeRight.move(velocity);
}
