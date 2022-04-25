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

enum ParameterType {
    Explicit, // Specified by the current command
    Implicit, // Specified by some external source (not filled by command parser)
    Either // Used for filtering. Retrieves explicit param and falls back to implicit.
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

        // Update or insert the given value for an explicit parameter.
        void updateExplicitParameter(char param, std::string value) {
            paramMap[param] = value;
        };

        // Update or insert the given value for an implicit parameter.
        void updateImplicitParameter(char param, std::string value) {
            implicitParamMap[param] = value;
        };

        // Remove the selected parameter from the explicit parameter map.
        void eraseExplicitParameter(char param) {
            if (paramMap.find(param) != paramMap.end()) {
                paramMap.erase(param);
            }
        }

        // Remove the selected parameter from the implicit parameter map.
        void eraseImplicitParameter(char param) {
            if (implicitParamMap.find(param) != implicitParamMap.end()) {
                implicitParamMap.erase(param);
            }
        }

        // Retrieves the selected parameter as a string. Returns Command::nparam if the parameter does not exist.
        std::string getParameterAsString(char param, ParameterType type = Either);

        // Retrieves and parses the selected parameter as a float. Returns Command::nparam if the parameter does not exist.
        float getParameterAsFloat(char param, ParameterType type = Either);

        // Retrieves and parses the selected parameter as a double. Returns Command::nparam if the parameter does not exist.
        double getParameterAsDouble(char param, ParameterType type = Either);

        // Retrieves and parses the selected parameter as a double. Returns Command::nparam if the parameter does not exist.
        signed int getParameterAsInt(char param, ParameterType type = Either);

        // Returns true if the given parameter was specified for this command and false otherwise.
        bool hasParameter(char param, ParameterType type = Either) {
            switch (type) {
                case Explicit:
                    return paramMap.find(param) != paramMap.end();
                case Implicit:
                    return implicitParamMap.find(param) != implicitParamMap.end();
                case Either:
                    return (paramMap.find(param) != paramMap.end()) || (implicitParamMap.find(param) != implicitParamMap.end());
                default:
                    return false;
            }
        }

        // Retrieves the command number
        signed short getCommandNumber() {
            return commandNumber;
        }

        std::string *rawString() {
            return rawCommand;
        }
        std::map<char, std::string> args() {
            return paramMap;
        }
        std::string interpretedString();

        bool handleCommand();

    private:
        CommandType commandType;
        signed short commandNumber;
        std::string *rawCommand;
        std::map<char, std::string> paramMap;
        std::map<char, std::string> implicitParamMap; // 
        
};

#endif