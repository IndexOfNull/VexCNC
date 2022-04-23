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

        void moveY(double abs_position, int16_t velocity);
        void moveX(double abs_position, int16_t velocity);
        void moveZ(double abs_position, int16_t velocity);

        void moveY(double abs_position, int16_t velocity, bool async);
        void moveX(double abs_position, int16_t velocity, bool async);
        void moveZ(double abs_position, int16_t velocity, bool async);

        double encoderToCurrentUnit(double encoderUnits) {
            switch (unitMode) {
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
        };

        double currentUnitToEncoder(double currentUnit) {
            switch (unitMode) {
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

        void waitForTarget(pros::Motor *motor, uint16_t tolerance) {
            double goal = motor->get_target_position();
            while (!((motor->get_position() < goal + tolerance) && (motor->get_position() > goal - tolerance))) {
                pros::delay(2);
            }
        }

};

#endif