#ifndef MACHINE_H
#define MACHINE_H

#include "Command.hpp"
#include <list>
#include <iterator>

class Machine {

    public:

        Machine();
        ~Machine();
        
        Command advance();
        void callCurrent();

        Command advanceAndCall() {
            Command r = advance();
            callCurrent();
            return r;
        };

        Command commandAt(size_t index);

        void setCommands(std::list<Command> *newCommands) {
            commandIterator = newCommands->begin();
            commands = newCommands;
        };



    private:

        std::list<Command> *commands;
        std::list<Command>::iterator commandIterator;
        size_t currentCommandIndex;
        Command * lastExplicitCommand; //the last command that wasn't of type Command::ReferenceLast


};

#endif