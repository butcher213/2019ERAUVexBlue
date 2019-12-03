#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "main.h"
#include "../../g_inc/RampingMotor.cpp"

//******************
// Drive train ports
//******************
#define MOTOR_FRONT_LEFT_PORT  (1)
#define MOTOR_FRONT_RIGHT_PORT (2)
#define MOTOR_BACK_LEFT_PORT   (3)
#define MOTOR_BACK_RIGHT_PORT  (10)

//*******************
// Tray actuator port
//*******************
#define MOTOR_TRAY_ACTUATOR_PORT (5)
#define TRAY_LOWER_LIMITER_PORT ('B')

//*********
// Arm port
//*********
#define ARM_PORT_LEFT (6)
#define ARM_PORT_RIGHT (7)

//************
// Intake port
//************
#define INTAKE_PORT_1 (8)
#define INTAKE_PORT_2 (9)

//********************
// Drive train defines
//********************
#define ENCODER_COUNTS_PER_INCH   (0)
#define ENCODER_COUNTS_PER_DEGREE (0)
#define ENCODER_TARGET_RANGE      (5)
// Minimun voltage agnitude to make the robot
//  move when applied to all drive motors
#define DRIVE_MINIMUM_VOLTAGE     (0) /* TODO: Adjust this voltage */
// The time that the robot reverses direction
//  to brake based on the velocity. This value
//  is multiplied by the value returned by
//  getDriveVelocity()
#define DRIVE_VELOCITY_TO_BRAKE_TIME_MS (100) /* TODO Adjust this */
#define DRIVE_RAMP_SCALE (127 / 10)

// Limit switch under arm that prevents arm
//  stall/break when lowering
#define ARM_BOTTOM_LIMITER_PORT ('A')


// This class holds functions and constants
//  related to a robot
class Robot {
private:
    // Private constructor allows instantiation
    //  only within class
    Robot() {}

public:
    //*******************
    // Drive train motors
    //*******************
    static RampingMotor frontLeft;
    static RampingMotor frontRight;
    static RampingMotor backLeft;
    static RampingMotor backRight;

    //*********************
    // Tray actuator motors
    //*********************
    static RampingMotor actuator;

    //***********
    // Arm motor
    //***********
    static RampingMotor armLeft;
    static RampingMotor armRight;

    //*************
    // Intake motor
    //*************
    static RampingMotor intakeLeft;
    static RampingMotor intakeRight;


    //**********************
    // Drive train constants
    //**********************
    static const int MOVE_FORWARD;
    static const int MOVE_BACKWARD;
    static const int STRAFE_RIGHT;
    static const int STRAFE_LEFT;
    static const int ROTATE_CW;
    static const int ROTATE_CCW;

    //************************
    // Tray actuator constants
    //************************
    static const int TRAY_RAISE;
    static const int TRAY_LOWER;

    //***************
    // Arm constants
    //***************
    static const int ARM_RAISE;
    static const int ARM_LOWER;

    //*****************
    // Intake constants
    //*****************
    static const int INTAKE_PULL;
    static const int INTAKE_PUSH;

    //**************
    // Robot sensors
    //**************
    static pros::ADIDigitalIn armBottomLimiter;
    static pros::ADIDigitalIn trayLowerLimiter;


    //**********************
    // Drive train functions
    //**********************
    static void forward(int velocity);
    static void strafe(int velocity);
    static void rotate(int velocity);
    static void drive(int forwardVal, int strafeVal, int rotateVal);

    //*****************************
    // Autonomous control functions
    //*****************************
    static void forwardDistance(double inches);
    static void strafeDistance(double inches);
    static void rotateDegrees(double degrees);

    //***********************
    // Drive sensor functions
    //***********************
    static double getDriveEncoderValue();
    static double getDriveVelocity();

    //************************
    // Tray actuator functions
    //************************
    static void actuateTray(int velocity);

    //***************
    // Arm functions
    //***************
    static void armVelocity(int velocity);

    //*****************
    // Intake functions
    //*****************
    static void intakeVelocity(int velocity);
};

#endif // _ROBOT_HPP_
