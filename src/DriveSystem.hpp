#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

#include "DriveSystem.hpp"
#include "pros/apix.h"
#include <map>

enum UnitMode {
    Millimeters = 0,
    Inches = 1
};

enum PositionMode {
    Absolute = 0,
    Relative = 1
};

//TODO: Add wait for target reached.
class DriveSystem {

    public:

        static const size_t nunit = 0;

        DriveSystem(pros::Motor *motor_l, pros::Motor *motor_r, pros::Motor *motor_carriage, pros::Motor *motor_head, double xTrackLength, double yTrackLength, double zTrackLength);
        ~DriveSystem();

        std::map<uint8_t, double> millimeterToEncoderConsts; // Port number to constants
        
        // Home all axis (velocity is in RPM, not current units)
        void home(int16_t velocity);

        //Will home the carriage on the X axis. Sign of velocity is disregarded.
        void homeX(int16_t velocity);

        //Will home the carriage on the Y axis. Sign of velocity is disregarded.
        void homeY(int16_t velocity);

        //Will home the pen actuator on the Z axis. Sign of velocity is disregarded.
        void homeZ(int16_t velocity);

        // Home Y axis (velocity is in RPM, not current units)
        void findCarriageEnd(int16_t velocity);

        // Run any motor and halt until its actual velocity is zero (velocity is in RPM, not current units)
        void runUntilCrash(pros::Motor *motor, int16_t velocity);

        //Sets motor velocities and halts until they stop registering movement.
        //void runUntilCrash(pros::Motor* motor, int16_t velocity);

        void autoCalibrate(int16_t velocity);

        // Manually calibrates the encoder to millimeter constants for the head. zTrackLength is the distance pen 2 travels up before pen 1 hits.
        void calibrateHead();

        void stopAll();

        //TODO: Ensure velocity is in right units (make sure moves are linearly interpolated to take equal times to complete)

        // Sets the target X to the specified position (in current units), but does not move to it
        void setTargetX(double abs_position);
        // Sets the target Y to the specified position (in current units), but does not move to it
        void setTargetY(double abs_position);
        // Sets the target Z to the specified position (in current units), but does not move to it
        void setTargetZ(double abs_position);

        // Returns the X target in current units
        void setRelativeTargetX(double rel_position);

        // Returns the raw Y target in current units
        void setRelativeTargetY(double rel_position);

        // Returns the raw Z target in current units
        void setRelativeTargetZ(double rel_position);

        // Returns the raw X target in encoder ticks
        double getRawTargetX() {
            return targetXEncoderU;
        }

        // Returns the raw Y target in encoder ticks
        double getRawTargetY() {
            return targetYEncoderU;
        }

        // Returns the raw Z target in encoder ticks
        double getRawTargetZ() {
            return targetZEncoderU;
        }

        // Linearly moves to the set target 
        void directMoveToTarget(bool async = true);

        void moveY(double abs_position, bool async = true);
        void moveX(double abs_position, bool async = true);
        void moveZ(double abs_position, bool async = true);

        // Converts encoder counts into the drive system's current unit. Requires a calibrated motor to be supplied; returns DriveSystem::nunit when uncalibrated.
        double encoderToUnit(pros::Motor *motor, double encoderUnits) {
            uint8_t port = motor->get_port();
            if (millimeterToEncoderConsts.find(port) == millimeterToEncoderConsts.end()) {
                return nunit;
            }

            switch (unitMode) {
                case Millimeters:
                    return encoderUnits * millimeterToEncoderConsts[port];
                case Inches:
                    return encoderUnits * millimeterToEncoderConsts[port] / 25.4;
            }
            return 0;
        };

        // Converts the drive system's current unit into encoder counts. Requires a calibrated motor to be supplied; returns DriveSystem::nunit when uncalibrated.
        double unitToEncoder(pros::Motor *motor, double unit) {
            uint8_t port = motor->get_port();
            if (millimeterToEncoderConsts.find(port) == millimeterToEncoderConsts.end()) {
                return nunit;
            }

            switch (unitMode) {
                case Millimeters:
                    return unit / millimeterToEncoderConsts[port];
                case Inches:
                    return (unit * 25.4) / millimeterToEncoderConsts[port];
            }
            return 0; //this should never happen but the compiler complains if I don't do this
        }

        // Returns the number of encoder units are needed to make one revolution for a specific motor.
        double getEncodersPerRevolution(pros::Motor *motor) {
            switch (motor->get_gearing()) {
                case pros::motor_gearset_e::E_MOTOR_GEARSET_06:
                    return 300;
                case pros::motor_gearset_e::E_MOTOR_GEARSET_18:
                    return 900;
                case pros::motor_gearset_e::E_MOTOR_GEARSET_36:
                    return 1800;
                default:
                    return 900;
            }
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

            unitMode = mode;
        }

        PositionMode positionMode = Absolute;

    private:

        // All values treated as encoder counts

        UnitMode unitMode = Millimeters;

        pros::Motor *leftMotor;
        pros::Motor *rightMotor;
        pros::Motor *carriageMotor;
        pros::Motor *headMotor;

        double feedrate = 1; // in current units per minute

        double targetYEncoderU; //Main carriage (two rails)
        double targetXEncoderU; //Main head carraige (one)
        double targetZEncoderU; //Pen actuator

        double xTrackLength;
        double yTrackLength;
        double zTrackLength;

        void waitForTarget(pros::Motor *motor, float target, uint16_t tolerance) {
            //double goal = motor->get_target_position();
            float currentPos = motor->get_position();
            while (!((currentPos < target + tolerance) && (currentPos > target - tolerance))) {
                if (currentPos == PROS_ERR_F) {
                    return;
                }
                currentPos = motor->get_position();
                //std::cout << currentPos << ", " << target << ", " << motor->get_target_position() << std::endl;
                pros::delay(20);
            }
        }

};

#endif