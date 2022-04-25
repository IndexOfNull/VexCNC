#include "Machine.hpp"
#include "gcode/Command.hpp"
#include "gcode/ParameterMap.hpp"

bool Machine::handleCommand(Command *command) {
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

bool Machine::handleGCommand(Command *command) {
    ParameterMap *params = command->getParameterMap();
    double x = params->getParameterAsDouble('X');
    double y = params->getParameterAsDouble('Y');
    double z = params->getParameterAsDouble('Z');
    double f = params->getParameterAsDouble('F');

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
            if (x != ParameterMap::nparam) {
                driveSystem->setTargetX(x);
            }
            if (y != ParameterMap::nparam) {
                driveSystem->setTargetY(y);
            }
            if (z != ParameterMap::nparam) {
                driveSystem->setTargetZ(z);
            }

            driveSystem->directMoveToTarget(false);
            break;
        case 20:
            driveSystem->setUnitMode(Inches);
            break;
        case 21:
            driveSystem->setUnitMode(Millimeters);
            break;
        case 28:
            if (!(hasX && hasY && hasZ)) {
                // TODO: Home X, Y, and Z
            }
            if (hasY) {
                driveSystem->home(10);
            }
            if (hasX) {
                // TODO: Home X
            }
            if (hasZ) {
                // TODO: Home z
            }
            break;
    }
}

bool Machine::handleDebugCommand(Command *command) {

}

bool Machine::handleMetaCommand(Command *command) {

}

bool Machine::handleToolCommand(Command *command) {

}