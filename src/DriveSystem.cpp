#include "DriveSystem.hpp"
#include "pros/apix.h"
#include <stdlib.h>
#include <math.h>

#define MOTOR_TARGET_TOLERANCE 5

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

    millimeterToEncoderConst = (distanceInMm / avg);
}

void DriveSystem::moveY(double abs_position, int16_t velocity, bool async) {
    currentYEncoderU = currentUnitToEncoder(abs_position);
    rightMotor->move_absolute(currentYEncoderU, currentUnitToEncoder(velocity));
    leftMotor->move_absolute(currentYEncoderU, currentUnitToEncoder(velocity));

    if (async) {
        waitForTarget(leftMotor, MOTOR_TARGET_TOLERANCE);
        waitForTarget(rightMotor, MOTOR_TARGET_TOLERANCE);
    }
};

void DriveSystem::moveY(double abs_position, int16_t velocity) {
    moveY(abs_position, velocity, true);
}


void DriveSystem::moveX(double abs_position, int16_t velocity, bool async) {
    currentXEncoderU = currentUnitToEncoder(abs_position);
    carriageMotor->move_absolute(currentXEncoderU, currentUnitToEncoder(velocity));

    if (async) {
        waitForTarget(carriageMotor, MOTOR_TARGET_TOLERANCE);
    }
};

void DriveSystem::moveX(double abs_position, int16_t velocity) {
    moveX(abs_position, velocity, true);
}

void DriveSystem::moveZ(double abs_position, int16_t velocity, bool async) {
    headMotor->move_absolute(currentZEncoderU, currentUnitToEncoder(velocity));
    if (async) {
        waitForTarget(headMotor, MOTOR_TARGET_TOLERANCE);
    }
};

void DriveSystem::moveZ(double abs_position, int16_t velocity) {
    headMotor->move_absolute(currentZEncoderU, currentUnitToEncoder(velocity));
};