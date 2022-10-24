#ifndef GCODERUNNER_H
#define GCODERUNNER_H

#include "gcode/Command.hpp"
#include "DriveSystem.hpp"
#include <list>
#include <iterator>

class GCodeRunner {

    public:
        GCodeRunner(DriveSystem *system);
        ~GCodeRunner();

        void setCommands(std::list<Command> *newCommands) {
            commandIterator = newCommands->begin();
            commands = newCommands;
        };

        // Returns false if the end of the command list is hit.
        bool executeCurrentCommand() {
            if (commandIterator == commands->end()) {
                return false;
            }
            handleCommand(&(*commandIterator));
            return true;
        }

        // Returns false if the end of the command list is hit.
        bool advance() {
            if ((commandIterator != commands->end()) && (++commandIterator == commands->end()) || ended) { // Return false if we are at the end or will hit the end
                return false;
            }
            return true;
        }

        void runAll() {
            while (advance()) {
                //std::cout << (*commandIterator).interpretedString() << std::endl;
                executeCurrentCommand();
            }
        }

        std::list<Command>* getCommands() {
            return commands;
        }

        std::list<Command>::iterator getCommandIterator() {
            return commandIterator;
        }

        bool handleCommand(Command *command);

    private:
        
        std::list<Command> *commands;
        std::list<Command>::iterator commandIterator;
        size_t currentCommandIndex;
        Command *lastExplicitCommand; // the last command that wasn't of type Command::ReferenceLast
        DriveSystem *driveSystem;

        bool ended = false; // A kill-switch able to be set by an M-2 command

        bool handleGCommand(Command *command);
        bool handleMetaCommand(Command *command);
        bool handleToolCommand(Command *command);
        bool handleDebugCommand(Command *command);
};

#endif