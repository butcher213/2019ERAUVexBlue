#include <vector>
#include "Robot.hpp"

//*********************************
// Drive train constant definitions
//*********************************
const int Robot::MOVE_FORWARD = 127; // TODO check value correct
const int Robot::MOVE_BACKWARD = -Robot::MOVE_FORWARD;
const int Robot::STRAFE_RIGHT = 127; // TODO check value correct
const int Robot::STRAFE_LEFT = -Robot::STRAFE_RIGHT;
const int Robot::ROTATE_CW = 127; // TODO check value correct
const int Robot::ROTATE_CCW = -Robot::ROTATE_CW;

//***********************************
// Tray actuator constant definitions
//***********************************
const int Robot::TRAY_RAISE = 127/4; // TODO check value correct
const int Robot::TRAY_LOWER = -Robot::TRAY_RAISE;

//**************************
// Lift constant definitions
//**************************
const int Robot::LIFT_RAISE = 127/2; // TODO check value correct
const int Robot::LIFT_LOWER = -Robot::LIFT_RAISE;

//****************************
// Intake constant definitions
//****************************
const int Robot::INTAKE_PULL = 127; // TODO check value correct
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
std::vector<float> analogValueDisp(int left_x, int left_y){
    float r;
    float newX;
    float newY;
    float theta1;
    float theta1d;
    float theta2;
    float theta2d;
    float moveVal_1;
    float moveVal_2;

    pros::Motor motor4 (4); // black
    pros::Motor motor1 (1); // black
    pros::Motor motor3 (3); // blue
    pros::Motor motor2 (2); // blue

	float max = std::max(std::abs(left_x), std::abs(left_y));

	if(((left_y != 0) && (left_x != 0)) || (max !=0)){
		r = sqrt((left_x)*(left_x) + (left_y)*(left_y));

		float maxX = left_x / max;
		float maxY = left_y / max;

		newX = r * maxX;
		newY = r * maxY;
	}
	else if(left_x == 0 && left_y == 0){
		newX = 0;
		newY = 0;
	}

    r = sqrt((newX)*(newX) + (newY)*(newY));

    //1st quad
    if((newX>0 && newY>0) || (newX == 0 && newY > 0) || (newY == 0 && newX > 0)){
		// black wheels
		moveVal_1 = r;

		// blue wheels
		theta2 = (30*(3.14/180)) - (atan(newY/newX));
		theta2d = (theta2*(180/3.14));
		if(theta2 > 0){
			moveVal_2 = -r*(sin(theta2)/(sin(30*(3.14/180))));
		}
		else{
			moveVal_2 = r*(sin(theta2)/sin(-60*(3.14/180)));
		}
		if(newY == 0){
			moveVal_1 = r;
			moveVal_2 = -r;
		}
		else if(newX == 0){
			moveVal_1 = r;
			moveVal_2 = r;
		}
    }

    //2nd quad
    else if((newX<0 && newY>0)){
		// black wheels
		theta1 = (120*(3.14/180)) - (atan(newY/fabs(newX))+(3.14/2));
		theta1d = (theta1*(180/3.14));
		if(theta1 > 0){
			moveVal_1 = -r*(sin(theta1)/(sin(30*(3.14/180))));
		}
		else{
			moveVal_1 = r*(sin(theta1)/sin(-60*(3.14/180)));
		}

		// blue wheels
		moveVal_2 = r;
    }

    // 3rd quad
    else if((newX<0 && newY<0) || (newX == 0 && newY < 0) || (newY == 0 && newX < 0)){
		// black wheels
		moveVal_1 = -1*r;

		// blue wheels
		theta2 = (210*(3.14/180)) - (atan((newY)/(newX))+(180)*(3.14/180));
		theta2d = (theta2*(180/3.14));
		if(theta2 > 0){
			moveVal_2 = r*(sin(theta2)/sin(30*(3.14/180)));
		}
		else{
			moveVal_2 = -r*(sin(theta2)/sin(-60*(3.14/180)));
		}
		if(newY == 0){
			moveVal_1 = -r;
			moveVal_2 = r;
		}
		else if(newX == 0){
			moveVal_1 = -r;
			moveVal_2 = -r;
		}
    }

    // 4th quad
    else if((newX>0 && newY<0)){
		// black wheels
		theta1 = (330*(3.14/180)) - (atan((newY)/(newX))+(360)*(3.14/180));
		theta1d = (theta1*(180/3.14));
		if(theta1 > 0){
			moveVal_1 = -r*(sin(theta1)/sin(60*(3.14/180)));
		}
		else{
			moveVal_1 = r*(sin(theta1)/sin(-30*(3.14/180)));
		}

		// blue wheels
		moveVal_2 = -1*r;
    }
    else{
		moveVal_1 = 0;
		moveVal_2 = 0;
    }

    if(moveVal_1 > 100){
		moveVal_1 = 100;
    }
    else if(moveVal_1 < -100){
		moveVal_1 = -100;
    }
    if(moveVal_2 > 100){
		moveVal_2 = 100;
    }
    else if(moveVal_2 < -127){
		moveVal_2 = -127;
    }

    std::vector<float> returnVector;
	float motorVal_1 = moveVal_1;  // black port 1
	float motorVal_2 = moveVal_2; // blue port 2
	float motorVal_3 = moveVal_2;  // blue port 3
	float motorVal_4 = moveVal_1; // black port 4

    returnVector.push_back(motorVal_1);
    returnVector.push_back(motorVal_2);
    returnVector.push_back(motorVal_3);
    returnVector.push_back(motorVal_4);

	return returnVector;
}
void Robot::drive(int forwardVal, int strafeVal, int rotateVal) {
    std::vector<float> motVals = analogValueDisp(strafeVal, forwardVal);
    frontLeft.move(motVals[0] + rotateVal);
    frontRight.move(motVals[1] - rotateVal);
    backLeft.move(motVals[2] + rotateVal);
    backRight.move(motVals[3] - rotateVal);
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
// Lift function definitions
//**************************
void Robot::liftVelocity(int velocity) {
    liftRight.move(velocity);
    liftLeft.move(velocity);
}

//****************************
// Intake function definitions
//****************************
void Robot::intakeVelocity(int velocity) {
    intakeLeft.move(velocity);
    intakeRight.move(velocity);
}
