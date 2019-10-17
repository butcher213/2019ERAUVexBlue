#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "main.h"

//******************
// Drive train ports
//******************
#define MOTOR_FRONT_LEFT_PORT  (1)
#define MOTOR_FRONT_RIGHT_PORT (2)
#define MOTOR_BACK_LEFT_PORT   (3)
#define MOTOR_BACK_RIGHT_PORT  (4)

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

//*******************
// Tray actuator port
//*******************
#define MOTOR_TRAY_ACTUATOR_PORT (5)

//**********
// Lift port
//**********
#define LIFT_PORT (6)

//************
// Intake port
//************
#define INTAKE_PORT_1 (7)
#define INTAKE_PORT_2 (8)


// This class holds functions and constants
//  related to a robot
class Robot {
private:
    //*******************
    // Drive train motors
    //*******************
    pros::Motor frontLeft  = pros::Motor(MOTOR_FRONT_LEFT_PORT, pros::E_MOTOR_GEARSET_18, false);
    pros::Motor frontRight = pros::Motor(MOTOR_FRONT_RIGHT_PORT, pros::E_MOTOR_GEARSET_18, true);
    pros::Motor backLeft   = pros::Motor(MOTOR_BACK_LEFT_PORT, pros::E_MOTOR_GEARSET_18, false);
    pros::Motor backRight  = pros::Motor(MOTOR_BACK_RIGHT_PORT, pros::E_MOTOR_GEARSET_18, true);

    //*********************
    // Tray actuator motors
    //*********************
    pros::Motor actuator = pros::Motor(MOTOR_TRAY_ACTUATOR_PORT);

    //***********
    // Lift motor
    //***********
    pros::Motor lift = pros::Motor(LIFT_PORT);

    //*************
    // Intake motor
    //*************
    pros::Motor intakeLeft  = pros::Motor(INTAKE_PORT_1);
    pros::Motor intakeRight = pros::Motor(INTAKE_PORT_2);

    //****************
    // Singleton robot
    //****************
    static Robot robot_singleton;

    // Private constructor allows instantiation
    //  only within class
    Robot() {}

public:
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
    // Lift constants
    //***************
    static const int LIFT_RAISE;
    static const int LIFT_LOWER;

    //*****************
    // Intake constants
    //*****************
    static const int INTAKE_PULL;
    static const int INTAKE_PUSH;


    // Returns the robot singlton
    static Robot singleton();


    //**********************
    // Drive train functions
    //**********************
    void forward(int velocity);
    void strafe(int velocity);
    void rotate(int velocity);
    void drive(int forwardVal, int strafeVal, int rotateVal);

    //*****************************
    // Autonomous control functions
    //*****************************
    void forwardDistance(double inches);
    void strafeDistance(double inches);
    void rotateDegrees(double degrees);

    //***********************
    // Drive sensor functions
    //***********************
    double getDriveEncoderValue();
    double getDriveVelocity();

    //************************
    // Tray actuator functions
    //************************
    void actuateTray(int velocity);

    //***************
    // Lift functions
    //***************
    void liftVelocity(int velocity);

    //*****************
    // Intake functions
    //*****************
    void intakeVelocity(int velocity);
};

#endif // _ROBOT_HPP_
