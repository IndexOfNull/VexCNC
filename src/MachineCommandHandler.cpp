#include "Machine.hpp"
#include "Command.hpp"

bool Machine::handleCommand(Command *command) {
    switch (command->commandType()) {
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
    double x = command->getParameterAsDouble('X');
    double y = command->getParameterAsDouble('Y');
    double z = command->getParameterAsDouble('Z');
    double f = command->getParameterAsDouble('F');

    if (f != Command::nparam) {
        driveSystem->setFeedrate(f);
    }

    switch (command->getCommandNumber()) {
        case 0:
        case 1:
            if (x != Command::nparam) {
                driveSystem->setTargetX(x);
            }
            if (y != Command::nparam) {
                driveSystem->setTargetY(y);
            }
            if (z != Command::nparam) {
                driveSystem->setTargetZ(z);
            }

            driveSystem->directMoveToTarget(false);
    }
}

bool Machine::handleDebugCommand(Command *command) {

}

bool Machine::handleMetaCommand(Command *command) {

}

bool Machine::handleToolCommand(Command *command) {

}