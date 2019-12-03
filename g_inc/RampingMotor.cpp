#ifndef _RAMPING_MOTOR_CPP_
#define _RAMPING_MOTOR_CPP_


#include <vector>
#include "main.h"


// Keeps the number between low and high
inline int clamp(int low, int val, int high) {
    if (val < low)
        return low;
    else if (val > high)
        return high;
    else
        return val;
}

class RampingMotor {
private:
    pros::Motor *motor;
    float currentSpeed;
    float rampScale;

public:
    RampingMotor(int port, pros::motor_gearset_e_t gearset, bool reversed, float rampScale) {
        this->motor = new pros::Motor(port, gearset, reversed);

        this->rampScale = rampScale;

        this->currentSpeed = 0;
    }

    void rampedMove(int target)  {
        target = clamp(-127, target, 127);

        float distance = target - currentSpeed;
        float direction = 0;

        if (distance != 0) {
            if (std::abs(distance) < rampScale) {
                this->currentSpeed = target;
            }
            else {
                direction = std::abs(distance) / (distance);
                this->currentSpeed += direction * rampScale;
            }
            motor->move(currentSpeed);
        }
    }

    pros::Motor *getMotor() {
        return motor;
    }
    float getCurrent() {
        return currentSpeed;
    }
};

#endif // _RAMPING_MOTOR_CPP_
