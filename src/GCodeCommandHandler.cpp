#include "GCodeRunner.hpp"
#include "gcode/Command.hpp"
#include "gcode/ParameterMap.hpp"

bool GCodeRunner::handleCommand(Command *command) {
    std::cout << "Handling command: " << command->interpretedString() << std::endl;
    switch (command->getCommandType()) {
        case CommandType::G:
            return handleGCommand(command);
            break;
        case CommandType::Debug:
            return handleDebugCommand(command);
            break;
        case CommandType::Meta:
            return handleMetaCommand(command);
            break;
        case CommandType::Tool:
            return handleToolCommand(command);
            break;
    }
    return false;
}

bool GCodeRunner::handleGCommand(Command *command) {
    ParameterMap *params = command->getParameterMap();
    double x = params->getParameterAsDouble('X');
    double y = params->getParameterAsDouble('Y');
    double z = params->getParameterAsDouble('Z');
    double f = params->getParameterAsDouble('F');
    double p = params->getParameterAsDouble('P');
    double s = params->getParameterAsDouble('S');

    // TODO: fix issue involving implicit and explicit parameters.

    bool hasX = params->hasParameter('X');
    bool hasY = params->hasParameter('Y');
    bool hasZ = params->hasParameter('Z');

    if (f != ParameterMap::nparam) {
        driveSystem->setFeedrate(f);
    }

    switch (command->getCommandNumber()) {
        case 0:
        case 1:
            switch (driveSystem->positionMode) {
                case Absolute:
                    if (hasX) {
                        driveSystem->setTargetX(x);
                    }
                    if (hasY) {
                        driveSystem->setTargetY(y);
                    }
                    if (hasZ) {
                        driveSystem->setTargetZ(z);
                    }
                    break;
                case Relative:
                    if (hasX) {
                        driveSystem->setRelativeTargetX(x);
                    }
                    if (hasY) {
                        driveSystem->setRelativeTargetY(y);
                    }
                    if (hasZ) {
                        driveSystem->setRelativeTargetZ(z);
                    }
                    break;
            }

            driveSystem->directMoveToTarget(false);
            return true;
        case 4: // Dwell
            if (params->hasParameter('P')) {
                pros::delay(p);
            } else if (params->hasParameter('S')) {
                pros::delay(s*1000);
            } else {
                // TODO: error
                return false;
            }
            return true;
        case 20:
            driveSystem->setUnitMode(Inches);
            return true;
        case 21:
            driveSystem->setUnitMode(Millimeters);
            return true;
        case 28:
            if (!(hasX && hasY && hasZ)) {
                driveSystem->home(10);
                return true;
            }
            if (hasY) {
                driveSystem->homeY(10);
            }
            if (hasX) {
                driveSystem->homeX(10);
            }
            if (hasZ) {
                driveSystem->homeZ(10);
            }
            return true;
        default:
            return false;
    }

}

bool GCodeRunner::handleDebugCommand(Command *command) {

}

bool GCodeRunner::handleMetaCommand(Command *command) {
    switch (command->getCommandNumber()) {
        case 2:
            ended = true;
            return true;
        case 111:
            driveSystem->stopAll();
            ended = true;
            return true;
        default:
            return false;
    }
}

bool GCodeRunner::handleToolCommand(Command *command) {

}