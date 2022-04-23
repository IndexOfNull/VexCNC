#include "DriveSystem.hpp"
#include "pros/apix.h"
#include <stdlib.h>
#include <math.h>

DriveSystem::DriveSystem(pros::Motor *motor_l, pros::Motor *motor_r, pros::Motor *motor_carriage, pros::Motor *motor_head) {
    leftMotor = motor_l;
    rightMotor = motor_r;
    carriageMotor = motor_carriage;
    headMotor = motor_head;
}

DriveSystem::~DriveSystem() {

}

//Velocity dictates which end is found
void DriveSystem::findEnd(int16_t velocity) {    

    /*leftMotor->move_relative(100, velocity);
    rightMotor->move_relative(100, velocity);
    pros::delay(1000);*/

    leftMotor->move_velocity(-velocity);
    rightMotor->move_velocity(-velocity);
    pros::delay(200);

    bool leftDone = false;
    bool rightDone = false;
    uint16_t leftZeroCount = 0;
    uint16_t rightZeroCount = 0;
    bool leftStarted = false;
    bool rightStarted = false;
    double leftAvg = abs(velocity);
    double rightAvg = abs(velocity);
    while (true) {
        
        leftAvg = leftAvg - (leftAvg/4) + (abs(leftMotor->get_actual_velocity()) / 4);
        rightAvg = rightAvg - (rightAvg/4) + (abs(rightMotor->get_actual_velocity()) / 4);
        
        double leftVel = abs(leftMotor->get_actual_velocity());
        double rightVel = abs(rightMotor->get_actual_velocity());

        std::cout << leftVel << ": " << leftStarted << ", " << rightVel << ": " << rightStarted << std::endl;

        /*if (leftVel > 0) {
            leftStarted = true;
        }
        if (rightVel > 0) {
            rightStarted = true;
        }*/

        if (leftAvg < 1) {//((leftVel < 2 && leftStarted) || leftZeroCount > 15) {
            leftMotor->move_velocity(0);
            std::cout << "Left motor hit" << std::endl;
            leftDone = true;
        }

        if (rightAvg < 1) {//((rightVel < 2 && rightStarted) || rightZeroCount > 15) {
            rightMotor->move_velocity(0);
            std::cout << "Right motor hit" << std::endl;
            rightDone = true;
        }

        if (leftDone && rightDone) {
            break;
        }

        pros::delay(10);
    }
}

void DriveSystem::home(int16_t velocity) {

    for (size_t i = 0; i < 2; i++) {
        findEnd(velocity);
        leftMotor->tare_position();
        rightMotor->tare_position();
    }

}

void DriveSystem::autoCalibrate(double distanceInMm, int16_t velocity) {
    home(velocity);
    findEnd(-velocity);

    double leftPos = leftMotor->get_position();
    double rightPos = rightMotor->get_position();
    double avg = (leftPos + rightPos) / 2;

    millimeterToEncoderConst = (avg / distanceInMm);

}