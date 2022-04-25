#include "DriveSystem.hpp"
#include "pros/apix.h"
#include <stdlib.h>
#include <math.h>

#define MOTOR_TARGET_TOLERANCE 5

// Initializes the drive system and configures member motors. Do not modify motor encoder units after initialization!
DriveSystem::DriveSystem(pros::Motor *motor_l, pros::Motor *motor_r, pros::Motor *motor_carriage, pros::Motor *motor_head) {
    leftMotor = motor_l;
    rightMotor = motor_r;
    carriageMotor = motor_carriage;
    headMotor = motor_head;

    leftMotor->set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS);
    rightMotor->set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS);
    carriageMotor->set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS);
    headMotor->set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS);
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



    millimeterToEncoderConsts[leftMotor] = leftPos / distanceInMm;
    millimeterToEncoderConsts[rightMotor] = rightPos / distanceInMm;
}

void DriveSystem::setTargetX(double abs_position) {
    targetXEncoderU = unitToEncoder(carriageMotor, abs_position);
}

void DriveSystem::setTargetY(double abs_position) {
    targetYEncoderU = unitToEncoder(leftMotor, abs_position); // We arbitrarily use the left motor here (right and left should have the same conversion constants)
}

void DriveSystem::setTargetZ(double abs_position) {
    targetZEncoderU = unitToEncoder(headMotor, abs_position);
}

void DriveSystem::directMoveToTarget(bool async) {

}



void DriveSystem::moveY(double abs_position, bool async) {
    setTargetY(abs_position);
    rightMotor->move_absolute(targetYEncoderU, feedrate);
    leftMotor->move_absolute(targetYEncoderU, feedrate);

    if (async) {
        waitForTarget(leftMotor, MOTOR_TARGET_TOLERANCE);
        waitForTarget(rightMotor, MOTOR_TARGET_TOLERANCE);
    }
};

void DriveSystem::moveY(double abs_position) {
    moveY(abs_position, true);
}


void DriveSystem::moveX(double abs_position, bool async) {
    setTargetX(abs_position);
    carriageMotor->move_absolute(targetXEncoderU, feedrate);

    if (async) {
        waitForTarget(carriageMotor, MOTOR_TARGET_TOLERANCE);
    }
};

void DriveSystem::moveX(double abs_position) {
    moveX(abs_position, true);
}

void DriveSystem::moveZ(double abs_position, bool async) {
    setTargetZ(abs_position);
    moveZ(targetZEncoderU, feedrate);
    if (async) {
        waitForTarget(headMotor, MOTOR_TARGET_TOLERANCE);
    }
};

void DriveSystem::moveZ(double abs_position) {
    setTargetZ(abs_position);
    headMotor->move_absolute(targetZEncoderU, feedrate);
};