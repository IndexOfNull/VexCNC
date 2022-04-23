#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

#include "DriveSystem.hpp"
#include "pros/apix.h"

enum UnitMode {
    Millimeters = 0,
    Inches = 1,
    RawEncoderUnits = 2
};

class DriveSystem {

    public:

        DriveSystem(pros::Motor *motor_l, pros::Motor *motor_r, pros::Motor *motor_carriage, pros::Motor *motor_head);
        ~DriveSystem();
        
        void home(int16_t velocity);
        void findEnd(int16_t velocity);
        void autoCalibrate(double distanceInMm, int16_t velocity);

        //TODO: Ensure velocity is in right units

        void moveY(double abs_position, int16_t velocity) {
            currentYEncoderU = currentUnitToEncoder(abs_position);
            rightMotor->move_absolute(currentYEncoderU, currentUnitToEncoder(velocity));
            leftMotor->move_absolute(currentYEncoderU, currentUnitToEncoder(velocity));
        };
        void moveX(double abs_position, int16_t velocity) {
            currentXEncoderU = currentUnitToEncoder(abs_position);
            carriageMotor->move_absolute(currentXEncoderU, currentUnitToEncoder(velocity));
        };
        void moveZ(double abs_position, int16_t velocity) {
            headMotor->move_absolute(currentZEncoderU, currentUnitToEncoder(velocity));
        };

        double encoderToCurrentUnit(double encoderUnits) {
            switch unitMode {
                case Millimeters:
                    return encoderUnits * millimeterToEncoderConst;
                    break;
                case Inches:
                    return encoderUnits * millimeterToEncoderConst / 25.4;
                    break;
                case RawEncoderUnits:
                    return encoderUnits;
                    break;
            }
        }

        double currentUnitToEncoder(double currentUnit) {
            switch unitMode {
                case Millimeters:
                    return currentUnit / millimeterToEncoderConst;
                    break;
                case Inches:
                    return currentUnit / millimeterToEncoderConst * 25.4;
                    break;
                case RawEncoderUnits:
                    return currentUnit;
            }
        }

        double millimeterToEncoderConst;
        UnitMode unitMode = Millimeters;

    private:

        pros::Motor *leftMotor;
        pros::Motor *rightMotor;
        pros::Motor *carriageMotor;
        pros::Motor *headMotor;

        double currentYEncoderU; //Main carriage (two rails)
        double currentXEncoderU; //Main head carraige (one)
        double currentZEncoderU; //Pen actuator

};

#endif