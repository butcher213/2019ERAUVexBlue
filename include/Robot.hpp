#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "main.h"

//******************
// Drive train ports
//******************
#define MOTOR_FRONT_LEFT_PORT     (1)
#define MOTOR_FRONT_RIGHT_PORT    (1)
#define MOTOR_BACK_LEFT_PORT      (1)
#define MOTOR_BACK_RIGHT_PORT     (1)
#define ENCODER_COUNTS_PER_INCH   (0)
#define ENCODER_COUNTS_PER_DEGREE (0)
#define ENCODER_TARGET_RANGE      (5)

//*******************
// Tray actuator port
//*******************
#define MOTOR_TRAY_ACTUATOR_PORT (1)

//**********
// Lift port
//**********
#define LIFT_PORT (1)

//************
// Intake port
//************
#define INTAKE_PORT_1 (1)
#define INTAKE_PORT_2 (1)


// This class holds functions and constants related to a robot
class Robot {
private:
    //*******************
    // Drive train motors
    //*******************
    pros::Motor frontLeft = pros::Motor(MOTOR_FRONT_LEFT_PORT);
    pros::Motor frontRight = pros::Motor(MOTOR_FRONT_RIGHT_PORT);
    pros::Motor backLeft = pros::Motor(MOTOR_BACK_LEFT_PORT);
    pros::Motor backRight = pros::Motor(MOTOR_BACK_RIGHT_PORT);

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
    pros::Motor intakeLeft = pros::Motor(INTAKE_PORT_1);
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
    void forwardDistance(double inches);
    void strafeDistance(double inches);
    void rotateDegrees(double degrees);
    double getDriveEncoderValue();

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
