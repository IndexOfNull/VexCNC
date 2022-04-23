#include "Machine.hpp"

Machine::Machine() {
    
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

    if ((*commandIterator).commandType() != CommandType::ReferenceLast) {
        lastExplicitCommand = lastCommand;
    }

    return *commandIterator;
}

void Machine::callCurrent() {
    Command currentCommand = (*commandIterator); //will produce a copy that we can mock

    if (currentCommand.commandType() == CommandType::ReferenceLast) {
        std::map<char, std::string> lastExplicitArgs = (*lastExplicitCommand).args();
        for (auto it = lastExplicitArgs.begin(); it != lastExplicitArgs.end(); it++) {
            if (!currentCommand.hasParameter(it->first)) { //If the current command does not have an argument that the last explicit command did
                currentCommand.updateParameter(it->first, it->second);
            }
        }
    }

    
    //call handler
}