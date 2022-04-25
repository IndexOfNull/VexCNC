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

    // TODO: Parameter lookback

    handleCommand(&*commandIterator);
}