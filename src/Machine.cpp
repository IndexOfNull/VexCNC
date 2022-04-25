#include "Machine.hpp"
#include "DriveSystem.hpp"

Machine::Machine(DriveSystem *system) {
    driveSystem = system;
}

Machine::~Machine() {

}

Command Machine::commandAt(size_t index) {
    auto front = commands->begin();
    std::advance(front, index);
    return *front;
}

Command Machine::advance() {
    Command *lastCommand = &(*commandIterator);
    commandIterator++;
    currentCommandIndex++;

    if ((*commandIterator).getCommandType() != CommandType::ReferenceLast) {
        lastExplicitCommand = lastCommand;
    }

    return *commandIterator;
}

void Machine::callCurrent() {
    Command currentCommand = (*commandIterator); //will produce a copy that we can mock

    if (currentCommand.getCommandType() == CommandType::ReferenceLast) {
        std::map<char, std::string> lastExplicitArgs = (*lastExplicitCommand).args();
        for (auto it = lastExplicitArgs.begin(); it != lastExplicitArgs.end(); it++) {
            if (!currentCommand.hasParameter(it->first)) { //If the current command does not have an argument that the last explicit command did
                currentCommand.updateParameter(it->first, it->second);
            }
        }
    }
    
    handleCommand(&currentCommand);
}