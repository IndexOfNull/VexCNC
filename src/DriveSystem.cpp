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

//The velocity's sign (+ or -) dictates which end is found
void DriveSystem::findEnd(int16_t velocity) {    
    leftMotor->move_velocity(-velocity);
    rightMotor->move_velocity(-velocity);
    pros::delay(200);

    double avg = velocity;
    while (avg > 1) {
        avg = (abs(leftMotor->get_actual_velocity()) + abs(leftMotor->get_actual_velocity())) / 2;
        pros::delay(20);
    }

    leftMotor->brake();
    rightMotor->brake();
}

//Will home the carriage. Sign of velocity is disregarded.
void DriveSystem::home(int16_t velocity) {

    findEnd(abs(velocity));
    leftMotor->tare_position();
    rightMotor->tare_position();

}

void DriveSystem::autoCalibrate(double distanceInMm, int16_t velocity) {
    home(velocity);
    findEnd(-velocity);

    double leftPos = leftMotor->get_position();
    double rightPos = rightMotor->get_position();
    double avg = (leftPos + rightPos) / 2;

    millimeterToEncoderConst = (avg / distanceInMm);

}