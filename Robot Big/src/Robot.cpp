#include "Robot.hpp"
#include "../../g_inc/RampingMotor.cpp"

//*********************************
// Drive train constant definitions
//*********************************
const int Robot::MOVE_FORWARD = 127;
const int Robot::MOVE_BACKWARD = -Robot::MOVE_FORWARD;
const int Robot::STRAFE_RIGHT = 127;
const int Robot::STRAFE_LEFT = -Robot::STRAFE_RIGHT;
const int Robot::ROTATE_CW = 127;
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
const int Robot::INTAKE_PULL = 127;
const int Robot::INTAKE_PUSH = -Robot::INTAKE_PULL;


//**************
// Robot sensors
//**************
pros::ADIDigitalIn Robot::armBottomLimiter = pros::ADIDigitalIn(ARM_BOTTOM_LIMITER_PORT);
pros::ADIDigitalIn Robot::trayLowerLimiter = pros::ADIDigitalIn(TRAY_LOWER_LIMITER_PORT);

//*******************
// Drive train motors
//*******************
RampingMotor Robot::frontLeft  = RampingMotor(MOTOR_FRONT_LEFT_PORT,
                                              pros::E_MOTOR_GEARSET_18,
                                              false,
                                              DRIVE_RAMP_SCALE);
RampingMotor Robot::frontRight = RampingMotor(MOTOR_FRONT_RIGHT_PORT,
                                              pros::E_MOTOR_GEARSET_18,
                                              true,
                                              DRIVE_RAMP_SCALE);
RampingMotor Robot::backLeft   = RampingMotor(MOTOR_BACK_LEFT_PORT,
                                              pros::E_MOTOR_GEARSET_18,
                                              false,
                                              DRIVE_RAMP_SCALE);
RampingMotor Robot::backRight  = RampingMotor(MOTOR_BACK_RIGHT_PORT,
                                              pros::E_MOTOR_GEARSET_18,
                                              true,
                                              DRIVE_RAMP_SCALE);

//*********************
// Tray actuator motors
//*********************
RampingMotor Robot::actuator = RampingMotor(MOTOR_TRAY_ACTUATOR_PORT,
                                            pros::E_MOTOR_GEARSET_36,
                                            true,
                                            DRIVE_RAMP_SCALE);

//***********
// Arm motor
//***********
RampingMotor Robot::armLeft  = RampingMotor(ARM_PORT_LEFT,
                                            pros::E_MOTOR_GEARSET_36,
                                            false,
                                            DRIVE_RAMP_SCALE);
RampingMotor Robot::armRight = RampingMotor(ARM_PORT_RIGHT,
                                            pros::E_MOTOR_GEARSET_36,
                                            true,
                                            DRIVE_RAMP_SCALE);

//*************
// Intake motor
//*************
RampingMotor Robot::intakeLeft  = RampingMotor(INTAKE_PORT_1,
                                               pros::E_MOTOR_GEARSET_18,
                                               true,
                                               DRIVE_RAMP_SCALE);
RampingMotor Robot::intakeRight = RampingMotor(INTAKE_PORT_2,
                                               pros::E_MOTOR_GEARSET_18,
                                               false,
                                               DRIVE_RAMP_SCALE);


// returns the sign of the value
inline int sign(double value) {
    return (int) (value / abs(value));
}


//*********************************
// Drive train function definitions
//*********************************
void Robot::forward(int velocity) {
    if (abs(velocity) >= DRIVE_MINIMUM_VOLTAGE) {
        frontLeft.rampedMove(velocity);
        frontRight.rampedMove(velocity);
        backLeft.rampedMove(velocity);
        backRight.rampedMove(velocity);
    }
}
void Robot::strafe(int velocity) {
    if (abs(velocity) >= DRIVE_MINIMUM_VOLTAGE) {
        frontLeft.rampedMove(velocity);
        frontRight.rampedMove(-velocity);
        backLeft.rampedMove(-velocity);
        backRight.rampedMove(velocity);
    }
}
void Robot::rotate(int velocity) {
    if (abs(velocity) >= DRIVE_MINIMUM_VOLTAGE) {
        frontLeft.rampedMove(velocity);
        frontRight.rampedMove(-velocity);
        backLeft.rampedMove(velocity);
        backRight.rampedMove(-velocity);
    }
}
inline void remapCircleToSquare(int &x, int &y) {
    static const float R = 127 * std::sqrt(2); // sqrt(127^2 * 127^2) = 127 * sqrt(2)

    float r = std::sqrt(x*x + y*y);

    float max = std::max(x, y);
    
    float maxX = x / max * 127;
    float maxY = y / max * 127;

    float newX = r/R * maxX;
    float newY = r/R * maxY;

    x = newX;
    y = newY;
}
void Robot::drive(int forwardVal, int strafeVal, int rotateVal) {
    // remapCircleToSquare(forwardVal, strafeVal);

    int fl = forwardVal + strafeVal + rotateVal,
        fr = forwardVal - strafeVal - rotateVal,
        bl = forwardVal - strafeVal + rotateVal,
        br = forwardVal + strafeVal - rotateVal;

    frontLeft.rampedMove(fl);
    frontRight.rampedMove(fr);
    backLeft.rampedMove(bl);
    backRight.rampedMove(br);
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
    double averageMagnitude =
            (abs(frontLeft.getMotor()->get_position())
           + abs(frontRight.getMotor()->get_position())
           + abs(backLeft.getMotor()->get_position())
           + abs(backRight.getMotor()->get_position())) / 4;

    int signProduct =
            sign(frontLeft.getMotor()->get_position())
          * sign(frontRight.getMotor()->get_position())
          * sign(backLeft.getMotor()->get_position())
          * sign(backRight.getMotor()->get_position());

    return signProduct * averageMagnitude;
}
double Robot::getDriveVelocity() {
    double averageMagnitude =
            (abs(frontLeft.getMotor()->get_actual_velocity())
           + abs(frontRight.getMotor()->get_actual_velocity())
           + abs(backLeft.getMotor()->get_actual_velocity())
           + abs(backRight.getMotor()->get_actual_velocity())) / 4;

    int signProduct =
            sign(frontLeft.getMotor()->get_actual_velocity())
          * sign(frontRight.getMotor()->get_actual_velocity())
          * sign(backLeft.getMotor()->get_actual_velocity())
          * sign(backRight.getMotor()->get_actual_velocity());

    return signProduct * averageMagnitude;
}

//***********************************
// Tray actuator function definitions
//***********************************
void Robot::actuateTray(int velocity) {
    actuator.rampedMove(velocity);
}

//**************************
// Arm function definitions
//**************************
void Robot::armVelocity(int velocity) {
    if (armBottomLimiter.get_value() == HIGH && velocity < 0)
        return;

    armLeft.rampedMove(velocity);
    armRight.rampedMove(velocity);
}

//****************************
// Intake function definitions
//****************************
void Robot::intakeVelocity(int velocity) {
    intakeLeft.rampedMove(velocity);
    intakeRight.rampedMove(velocity);
}
