#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

#include "DriveSystem.hpp"
#include "pros/apix.h"
#include <map>

enum UnitMode {
    Millimeters = 0,
    Inches = 1
};

//TODO: Add wait for target reached.
class DriveSystem {

    public:

        static const size_t nunit = 0;

        DriveSystem(pros::Motor *motor_l, pros::Motor *motor_r, pros::Motor *motor_carriage, pros::Motor *motor_head);
        ~DriveSystem();

        std::map<pros::Motor *, double> millimeterToEncoderConsts;
        
        void home(int16_t velocity);
        void findCarriageEnd(int16_t velocity);

        //Sets motor velocities and halts until they stop registering movement.
        //void runUntilCrash(pros::Motor* motor, int16_t velocity);

        void autoCalibrate(double distanceInMm, int16_t velocity);

        //TODO: Ensure velocity is in right units (make sure moves are linearly interpolated to take equal times to complete)

        // Sets the target X to the specified position (in current units), but does not move to it
        void setTargetX(double abs_position);
        // Sets the target Y to the specified position (in current units), but does not move to it
        void setTargetY(double abs_position);
        // Sets the target Z to the specified position (in current units), but does not move to it
        void setTargetZ(double abs_position);

        // Linearly moves to the set target 
        void directMoveToTarget(bool async);

        void moveY(double abs_position);
        void moveX(double abs_position);
        void moveZ(double abs_position);
        void moveY(double abs_position, bool async);
        void moveX(double abs_position, bool async);
        void moveZ(double abs_position, bool async);

        // Converts encoder counts into the drive system's current unit. Requires a calibrated motor to be supplied; returns DriveSystem::nunit when uncalibrated.
        double encoderToUnit(pros::Motor *motor, double encoderUnits) {
            if (millimeterToEncoderConsts.find(motor) != millimeterToEncoderConsts.end()) {
                return nunit;
            }

            switch (unitMode) {
                case Millimeters:
                    return encoderUnits * millimeterToEncoderConsts[motor];
                    break;
                case Inches:
                    return encoderUnits * millimeterToEncoderConsts[motor] / 25.4;
                    break;
            }
            return 0;
        };

        // Converts the drive system's current unit into encoder counts. Requires a calibrated motor to be supplied; returns DriveSystem::nunit when uncalibrated.
        double unitToEncoder(pros::Motor *motor, double unit) {
            if (millimeterToEncoderConsts.find(motor) != millimeterToEncoderConsts.end()) {
                return nunit;
            }

            switch (unitMode) {
                case Millimeters:
                    return unit / millimeterToEncoderConsts[motor];
                    break;
                case Inches:
                    return unit / millimeterToEncoderConsts[motor] * 25.4;
                    break;
            }
            return 0; //this should never happen but the compiler complains if I don't do this
        }

        // Converts the drive system's current unit per second into an RPM value for a specific motor. Returns DriveSystem::nunit if the supplied motor is uncalibrated.
        double unitPerSecondToRPM(pros::Motor *motor, double unit) {
            if (millimeterToEncoderConsts.find(motor) != millimeterToEncoderConsts.end()) {
                return nunit;
            }

            return unitToEncoder(motor, unit) * 60; // (encoder counts / s) * (60 s / min)
        }

        // Updates the feedrate (uses current unit mode)
        void setFeedrate(double rate) {
            feedrate = rate;
        }

        //double millimeterToEncoderConst;
        
        // Switch to inches or millimeters, recalculating internal state as necessary.
        void setUnitMode(UnitMode mode) {
            if (mode == unitMode) {
                return;
            }

            switch (mode) {
                case Millimeters:
                    feedrate = feedrate * 25.4; //inches to mm
                    break;
                case Inches:
                    feedrate = feedrate / 25.4;
                    break;
            }
        }

    private:

        // All values treated as encoder counts

        UnitMode unitMode = Millimeters;

        pros::Motor *leftMotor;
        pros::Motor *rightMotor;
        pros::Motor *carriageMotor;
        pros::Motor *headMotor;

        double feedrate; // in current units (not encoder units; will be converted on-the-fly)

        double targetYEncoderU; //Main carriage (two rails)
        double targetXEncoderU; //Main head carraige (one)
        double targetZEncoderU; //Pen actuator

        void waitForTarget(pros::Motor *motor, uint16_t tolerance) {
            double goal = motor->get_target_position();
            while (!((motor->get_position() < goal + tolerance) && (motor->get_position() > goal - tolerance))) {
                pros::delay(2);
            }
        }

};

#endif