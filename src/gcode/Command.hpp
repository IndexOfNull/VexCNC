#ifndef COMMAND_H
#define COMMAND_H

#include <map>
#include <string>
#include <list>
#include "ParameterMap.hpp"

enum CommandType {
    G,
    Meta,  // M
    Tool,  // T
    Debug, // D
    ReferenceLast
};

inline const char *commandToStr(CommandType v) {
    switch (v) {
        case G:
            return "G";
        case Meta:
            return "M";
        case Tool:
            return "T";
        case Debug:
            return "D";
        case ReferenceLast:
            return "R";
        default:
            return "";
    }
}

class Command {

    public:

        Command(std::string *command);
        ~Command();

        void parse();

        CommandType getCommandType() { 
            return commandType;
        };

        // Retrieves the command number
        signed short getCommandNumber() {
            return commandNumber;
        }

        std::string *rawString() {
            return rawCommand;
        }

        // Returns a pointer to the command's parameter map.
        ParameterMap* getParameterMap() {
            return &parameters;
        }

        std::string interpretedString();

        bool handleCommand();

    private:
        CommandType commandType;
        signed short commandNumber;
        std::string *rawCommand;
        ParameterMap parameters = ParameterMap();
        
};

#endif