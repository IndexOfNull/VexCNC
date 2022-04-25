#include "DriveSystem.hpp"
#include "pros/apix.h"
#include <stdlib.h>
#include <deque>
#include <math.h>

#define MOTOR_TARGET_TOLERANCE 5 // How close can a motor be before we unhalt execution?
#define CRASH_ROLLING_AVERAGE_WINDOW 5 // How many values do we include in a rolling velocity average when intentionally crashing a motor?
#define CRASH_VELOCITY_THRESHOLD 2 // What must our velocity subceed to count a motor as stopped?

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
void DriveSystem::findCarriageEnd(int16_t velocity) {    
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

// void DriveSystem::runUntilCrash(pros::Motor *motor, int16_t velocity) {
//     motor->move_velocity(velocity);

//     pros::delay(100);

//     std::deque<double> values;
//     for (size_t i = 0; i < CRASH_ROLLING_AVERAGE_WINDOW; i++) {
//         values.push_front(velocity);
//     }

//     double avg = velocity;
//     while (avg > CRASH_VELOCITY_THRESHOLD) {
//         values.pop_front();
//         values.push_back(motor->get_actual_velocity());

//         avg = 0;
//         for (auto it = values.begin(); it != values.end(); it++) {
//             avg += *it * (1/values.size());
//         }
//         pros::delay(2);
//     }
// }

//Will home the carriage. Sign of velocity is disregarded.
void DriveSystem::home(int16_t velocity) {

    findCarriageEnd(abs(velocity));
    leftMotor->tare_position();
    rightMotor->tare_position();

}

void DriveSystem::autoCalibrate(double distanceInMm, int16_t velocity) { // does distance HAVE to be mm (or can it be current units?)
    home(velocity); // Will also tare positions 
    findCarriageEnd(-velocity);

    // TODO: Add homing of gantry (X-axis)

    double leftPos = leftMotor->get_position();
    double rightPos = rightMotor->get_position();

    millimeterToEncoderConsts[leftMotor] = distanceInMm / leftPos;
    millimeterToEncoderConsts[rightMotor] = distanceInMm / rightPos;
}

void DriveSystem::setTargetX(double abs_position) {
    targetXEncoderU = unitToEncoder(carriageMotor, abs_position);
}

void DriveSystem::setTargetY(double abs_position) {
    targetYEncoderU = unitToEncoder(leftMotor, abs_position); // We arbitrarily use the left motor here (right and left should have similar conversion constants)
}

void DriveSystem::setTargetZ(double abs_position) {
    targetZEncoderU = unitToEncoder(headMotor, abs_position);
}

void DriveSystem::directMoveToTarget(bool async) {
    double deltaXEncoder = abs(targetXEncoderU - carriageMotor->get_position());
    double deltaYEncoder = abs(targetYEncoderU - leftMotor->get_position());
    double deltaZEncoder = abs(targetZEncoderU - headMotor->get_position());
    double deltaXUnit = encoderToUnit(carriageMotor, deltaXEncoder);
    double deltaYUnit = encoderToUnit(leftMotor, deltaYEncoder);
    double deltaZUnit = encoderToUnit(headMotor, deltaZEncoder);

    double distance = sqrt(pow(deltaXUnit, 2) + pow(deltaYUnit, 2) + pow(deltaZUnit, 2));
    double time = distance / feedrate;

    double velocityX = unitPerSecondToRPM(carriageMotor, deltaXUnit / time);
    double velocityY = unitPerSecondToRPM(leftMotor, deltaYUnit / time);
    double velocityZ = unitPerSecondToRPM(headMotor, deltaZUnit / time);

    carriageMotor->move_absolute(targetZEncoderU, velocityX);
    rightMotor->move_absolute(targetYEncoderU, velocityY);
    leftMotor->move_absolute(targetYEncoderU, velocityY);
    headMotor->move_absolute(targetZEncoderU, velocityZ);

    if (async) { // All motors must be within tolerated range before we unblock execution
        waitForTarget(leftMotor, MOTOR_TARGET_TOLERANCE);
        waitForTarget(rightMotor, MOTOR_TARGET_TOLERANCE);
        waitForTarget(headMotor, MOTOR_TARGET_TOLERANCE);
        waitForTarget(carriageMotor, MOTOR_TARGET_TOLERANCE);
    }
}

// TODO: fix move commands to use feedrate properly

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