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
    switch (command->getCommandNumber()) {
        case 0:
        case 1:
            auto x = command->getParameterAsFloat('X');
            auto y = command->getParameterAsFloat('Y');
            auto z = command->getParameterAsFloat('Z');
    }
}

bool Machine::handleDebugCommand(Command *command) {

}

bool Machine::handleMetaCommand(Command *command) {

}

bool Machine::handleToolCommand(Command *command) {

}