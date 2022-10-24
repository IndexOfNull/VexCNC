#include "DriveSystem.hpp"
#include "pros/apix.h"
#include <stdlib.h>
#include <deque>
#include <math.h>

#define MOTOR_TARGET_TOLERANCE 10 // How close can a motor be before we unhalt execution?
#define CRASH_EFFICIENCY_SAMPLE_SIZE 3 // How many efficiency values do we sample to determine when to stop
#define CRASH_VELOCITY_THRESHOLD 1 // What must our velocity subceed to count a motor as stopped?

// Initializes the drive system and configures member motors. Do not modify motor encoder units after initialization!
DriveSystem::DriveSystem(pros::Motor *motor_l, pros::Motor *motor_r, pros::Motor *motor_carriage, pros::Motor *motor_head, double x_length, double y_length, double z_length) {
    leftMotor = motor_l;
    rightMotor = motor_r;
    carriageMotor = motor_carriage;
    headMotor = motor_head;

    leftMotor->set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS);
    rightMotor->set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS);
    carriageMotor->set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS);
    headMotor->set_encoder_units(pros::motor_encoder_units_e::E_MOTOR_ENCODER_COUNTS);

    leftMotor->set_brake_mode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
    leftMotor->set_brake_mode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
    leftMotor->set_brake_mode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
    leftMotor->set_brake_mode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);

    xTrackLength = x_length;
    yTrackLength = y_length;
    zTrackLength = z_length;
}

DriveSystem::~DriveSystem() {

}

void DriveSystem::stopAll() {
    carriageMotor->brake();
    headMotor->brake();
    leftMotor->brake();
    rightMotor->brake();
}

//The velocity's sign (+ or -) dictates which end is found
void DriveSystem::findCarriageEnd(int16_t velocity) {    

    leftMotor->move_velocity(velocity);
    rightMotor->move_velocity(velocity);
    pros::delay(600);

    double avgBeginningEfficiency = 0;
    for (size_t i = 0; i < CRASH_EFFICIENCY_SAMPLE_SIZE; i++) {
        avgBeginningEfficiency += (leftMotor->get_efficiency() + rightMotor->get_efficiency()) / 2;
        pros::delay(10);
    }
    avgBeginningEfficiency /= CRASH_EFFICIENCY_SAMPLE_SIZE;

    double efficiencyAvg = 100;
    while (efficiencyAvg > avgBeginningEfficiency * 0.33) {
        //std::cout << abs(leftMotor->get_actual_velocity()) << ", " << abs(rightMotor->get_actual_velocity()) << std::endl;
        //std::cout << leftMotor->get_efficiency() << ", " << rightMotor->get_efficiency() << std::endl;
        efficiencyAvg = (abs(leftMotor->get_efficiency()) + abs(rightMotor->get_efficiency())) / 2;
        //std::cout << efficiencyAvg << " > " << avgBeginningEfficiency * 0.33 << std::endl;
        pros::delay(2);
    }

    leftMotor->brake();
    rightMotor->brake();
}

void DriveSystem::runUntilCrash(pros::Motor *motor, int16_t velocity) {
    motor->move_velocity(velocity);
    pros::delay(600);

    double avgBeginningEfficiency = 0;
    for (size_t i = 0; i < CRASH_EFFICIENCY_SAMPLE_SIZE; i++) {
        avgBeginningEfficiency += motor->get_efficiency();
        pros::delay(10);
    }
    avgBeginningEfficiency /= CRASH_EFFICIENCY_SAMPLE_SIZE;

    //std::cout << avgBeginningEfficiency << std::endl;
    while (motor->get_efficiency() > (avgBeginningEfficiency * 0.33)) {
        //std::cout << motor->get_efficiency() << " > " << avgBeginningEfficiency*(0.33) << std::endl;
        pros::delay(20);
    }

    motor->brake();
}


void DriveSystem::homeY(int16_t velocity) {
    // Move away from home to get better efficiency readings
    leftMotor->move_velocity(abs(velocity));
    rightMotor->move_velocity(abs(velocity));
    pros::delay(1000);
    leftMotor->brake();
    rightMotor->brake();
    pros::delay(1000);

    findCarriageEnd(-abs(velocity));
    pros::delay(2000);
    leftMotor->tare_position();
    rightMotor->tare_position();
}


void DriveSystem::homeX(int16_t velocity) {
    //Same efficiency stuff as in homeY
    carriageMotor->move_velocity(abs(velocity));
    pros::delay(1000);
    carriageMotor->brake();
    pros::delay(1000);

    runUntilCrash(carriageMotor, -abs(velocity));
    pros::delay(2000);

    carriageMotor->move_velocity(10);
    pros::delay(1000);
    carriageMotor->brake();
    pros::delay(1000);

    carriageMotor->tare_position();
}

void DriveSystem::homeZ(int16_t velocity) {
    // TODO: implement Z homing
    pros::lcd::clear();
    pros::lcd::set_text(0, "Touch pen 1 to paper, then press the middle button.");
    while ((pros::lcd::read_buttons() && 2) != 1) {
        pros::delay(10);
    }
    headMotor->tare_position();
    pros::delay(100);
    headMotor->move_velocity(10);
    pros::delay(1000);
    headMotor->brake();
    pros::lcd::clear();
}

void DriveSystem::home(int16_t velocity) {
    homeY(velocity);
    homeX(velocity);
    homeZ(velocity);
}

void DriveSystem::calibrateHead() {
    pros::lcd::clear();
    pros::lcd::set_text(0, "Lower pen 2 to paper, then press the middle button.");
    while ((pros::lcd::read_buttons() && 2) != 1) { // Logical AND with a bitfield to see if middle button pressed
        pros::delay(10);
    }
    pros::lcd::set_text(0, "Waiting...");
    pros::delay(4000); //Keep it from immediately skipping the next step

    headMotor->tare_position();
    pros::lcd::clear();
    pros::lcd::set_text(0, "Lower pen 1 to paper, then press the middle button.");
    while ((pros::lcd::read_buttons() && LCD_BTN_CENTER) != 1) {
        pros::delay(10);
    }

    millimeterToEncoderConsts[headMotor->get_port()] = zTrackLength / headMotor->get_position();
    std::cout << "Head mm->enc: " << millimeterToEncoderConsts[headMotor->get_port()] << std::endl;
    headMotor->tare_position();
    pros::lcd::clear();
}

void DriveSystem::autoCalibrate(int16_t velocity) { // does distance HAVE to be mm (or can it be current units?)

    homeY(velocity); // Will also tare positions 
    findCarriageEnd(velocity);
    pros::delay(3000);
    double leftPos = leftMotor->get_position();
    double rightPos = rightMotor->get_position();

    homeX(velocity);
    runUntilCrash(carriageMotor, velocity);
    pros::delay(3000);
    double carriagePos = carriageMotor->get_position();
    homeX(velocity);

    std::cout << "Autocalibrate: Left: " << leftPos << ", Right: " << rightPos << ", Carriage: " << carriagePos << std::endl;

    millimeterToEncoderConsts[leftMotor->get_port()] = yTrackLength / leftPos;
    millimeterToEncoderConsts[rightMotor->get_port()] = yTrackLength / rightPos;
    millimeterToEncoderConsts[carriageMotor->get_port()] = xTrackLength / carriagePos;

    std::cout << "Left mm->enc: " << millimeterToEncoderConsts[leftMotor->get_port()] << std::endl;
    std::cout << "Right mm->enc: " << millimeterToEncoderConsts[rightMotor->get_port()] << std::endl;
    std::cout << "Carriage mm->enc: " << millimeterToEncoderConsts[carriageMotor->get_port()] << std::endl;
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

void DriveSystem::setRelativeTargetX(double rel_position) {
    targetXEncoderU += unitToEncoder(carriageMotor, rel_position);
}

void DriveSystem::setRelativeTargetY(double rel_position) {
    targetXEncoderU += unitToEncoder(leftMotor, rel_position);
}

void DriveSystem::setRelativeTargetZ(double rel_position) {
    targetXEncoderU += unitToEncoder(headMotor, rel_position);
}

void DriveSystem::directMoveToTarget(bool async) {
    float deltaXEncoder = abs(targetXEncoderU - carriageMotor->get_position());
    float deltaYEncoder = abs(targetYEncoderU - leftMotor->get_position());
    float deltaZEncoder = abs(targetZEncoderU - headMotor->get_position());
    float deltaXUnit = encoderToUnit(carriageMotor, deltaXEncoder);
    float deltaYUnit = encoderToUnit(leftMotor, deltaYEncoder);
    float deltaZUnit = encoderToUnit(headMotor, deltaZEncoder);

    float distance = sqrt(pow(deltaXUnit, 2) + pow(deltaYUnit, 2) + pow(deltaZUnit, 2));
    float time = distance / feedrate;

    //Convert deltas into encoder steps per minute for each motor
    float velocityX = (deltaXEncoder / time) / getEncodersPerRevolution(carriageMotor);
    float velocityYL = (deltaYEncoder / time) / getEncodersPerRevolution(leftMotor);
    float velocityYR = (deltaYEncoder / time) / getEncodersPerRevolution(rightMotor);
    float velocityZ = (deltaZEncoder / time) / getEncodersPerRevolution(headMotor);

    
    std::cout << std::endl << "Moving to target X: " << targetXEncoderU << ", Y: " << targetYEncoderU << ", Z: " << targetZEncoderU << " in " << time*60 << " seconds" << std::endl;
    std::cout << "Velocity: (" << velocityX << ", " << velocityYL << ", " << velocityZ << ")" << std::endl;
    std::cout << "Deltas: (" << deltaXEncoder << ", " << deltaYEncoder << ", " << deltaZEncoder << ")" << std::endl;
    std::cout << "Distance: " << distance << std::endl;
    


    carriageMotor->move_absolute(targetXEncoderU, velocityX);
    rightMotor->move_absolute(targetYEncoderU, velocityYR);
    leftMotor->move_absolute(targetYEncoderU, velocityYL);
    headMotor->move_absolute(targetZEncoderU, velocityZ);

    if (!async) { // All motors must be within tolerated range before we unblock execution
        pros::delay(time*60*1000 + 200);
    }
    
}

// TODO: fix move commands to use direct move system

void DriveSystem::moveY(double abs_position, bool async) {
    setTargetY(abs_position);
    rightMotor->move_absolute(targetYEncoderU, feedrate);
    leftMotor->move_absolute(targetYEncoderU, feedrate);

    if (!async) {
        waitForTarget(leftMotor, targetYEncoderU, MOTOR_TARGET_TOLERANCE);
        waitForTarget(rightMotor, targetYEncoderU, MOTOR_TARGET_TOLERANCE);
    }
};

void DriveSystem::moveX(double abs_position, bool async) {
    setTargetX(abs_position);
    carriageMotor->move_absolute(targetXEncoderU, feedrate);

    if (!async) {
        waitForTarget(carriageMotor, targetXEncoderU, MOTOR_TARGET_TOLERANCE);
    }
};

void DriveSystem::moveZ(double abs_position, bool async) {
    setTargetZ(abs_position);
    headMotor->move_absolute(targetZEncoderU, feedrate);
    if (!async) {
        waitForTarget(headMotor, targetZEncoderU, MOTOR_TARGET_TOLERANCE);
    }
};