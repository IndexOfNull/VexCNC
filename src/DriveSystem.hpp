#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

#include "DriveSystem.hpp"
#include "pros/apix.h"

class DriveSystem {

    public:

        DriveSystem(pros::Motor *motor_l, pros::Motor *motor_r, pros::Motor *motor_carriage, pros::Motor *motor_head);
        ~DriveSystem();
        
        void home(int16_t velocity);
        void findEnd(int16_t velocity);

        void autoCalibrate(double distanceInMm, int16_t velocity);

        double millimeterToEncoderConst;

    private:

        pros::Motor *leftMotor;
        pros::Motor *rightMotor;
        pros::Motor *carriageMotor;
        pros::Motor *headMotor;

};

#endif