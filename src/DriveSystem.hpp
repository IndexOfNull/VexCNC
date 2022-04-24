#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

#include "DriveSystem.hpp"
#include "pros/apix.h"

enum UnitMode {
    Millimeters = 0,
    Inches = 1,
    RawEncoderUnits = 2
};

//TODO: Add wait for target reached.
class DriveSystem {

    public:

        DriveSystem(pros::Motor *motor_l, pros::Motor *motor_r, pros::Motor *motor_carriage, pros::Motor *motor_head);
        ~DriveSystem();
        
        void home(int16_t velocity);
        void findEnd(int16_t velocity);
        void autoCalibrate(double distanceInMm, int16_t velocity);

        //TODO: Ensure velocity is in right units (make sure moves are linearly interpolated to take equal times to complete)

        // Sets the target X, but does not move to it
        void setTargetX(double abs_position);
        // Sets the target Y, but does not move to it
        void setTargetY(double abs_position);
        // Sets the target Z, but does not move to it
        void setTargetZ(double abs_position);

        // Linearly moves to the set target 
        void directMoveToTarget(bool async);
        void moveY(double abs_position);
        void moveX(double abs_position);
        void moveZ(double abs_position);

        void moveY(double abs_position, bool async);
        void moveX(double abs_position, bool async);
        void moveZ(double abs_position, bool async);

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
                    break;
            }
            return 0; //this should never happen but the compiler complains if I don't do this
        }

        double getFeedrate() {
            return encoderToCurrentUnit(feedrate);
        }

        void setFeedrate(double feedrate) {
            currentUnitToEncoder(feedrate);
        }

        double millimeterToEncoderConst;
        double millimeterPerSecondToRPMConst;
        UnitMode unitMode = Millimeters;

    private:

        // All values internally stored as encoder units ()

        pros::Motor *leftMotor;
        pros::Motor *rightMotor;
        pros::Motor *carriageMotor;
        pros::Motor *headMotor;

        double targetYEncoderU; //Main carriage (two rails)
        double targetXEncoderU; //Main head carraige (one)
        double targetZEncoderU; //Pen actuator

        double feedrate;

        void waitForTarget(pros::Motor *motor, uint16_t tolerance) {
            double goal = motor->get_target_position();
            while (!((motor->get_position() < goal + tolerance) && (motor->get_position() > goal - tolerance))) {
                pros::delay(2);
            }
        }

};

#endif