#ifndef COMMAND_H
#define COMMAND_H

#include <map>
#include <string>
#include <list>

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
        static const size_t nparam = 0;

        Command(std::string *command);
        ~Command();

        void parse();

        CommandType getCommandType() { 
            return commandType;
        };

        // Update or insert the given value for a parameter.
        void updateParameter(char param, std::string value);

        // Remove the selected parameter from the parameter map.
        void eraseParameter(char param);

        // Retrieves the selected parameter as a string. Returns Command::nparam if the parameter does not exist.
        std::string getParameterAsString(char param);

        // Retrieves and parses the selected parameter as a float. Returns Command::nparam if the parameter does not exist.
        float getParameterAsFloat(char param);

        // Retrieves and parses the selected parameter as a double. Returns Command::nparam if the parameter does not exist.
        double getParameterAsDouble(char param);

        // Retrieves and parses the selected parameter as a double. Returns Command::nparam if the parameter does not exist.
        signed int getParameterAsInt(char param);

        // Returns true if the given parameter was specified for this command and false otherwise.
        bool hasParameter(char param) {
            return argsMap.find(param) != argsMap.end();
        }

        // Retrieves the command number
        signed short getCommandNumber() {
            return commandNumber;
        }

        std::string *rawString() {
            return rawCommand;
        }
        std::map<char, std::string> args() {
            return argsMap;
        }
        std::string interpretedString();

        bool handleCommand();

    private:
        CommandType commandType;
        signed short commandNumber;
        std::string *rawCommand;
        std::map<char, std::string> argsMap;
        
};

#endif