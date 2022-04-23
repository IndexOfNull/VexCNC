#ifndef COMMAND_H
#define COMMAND_H

#include <map>
#include <string>
#include <list>

enum CommandType {
    G,
    Meta, //M
    Tool, //T
    Debug, //D
    ReferenceLast
};

inline const char* commandToStr(CommandType v) {
    switch (v) {
        case G: return "G";
        case Meta: return "M";
        case Tool: return "T";
        case Debug: return "D";
        case ReferenceLast: return "R";
        default: return "";
    }
}

class Command {

    static const size_t nparam = 0;

    public:

        Command(std::string *command);
        ~Command();

        void parse();

        CommandType commandType();
        void setNumberParameter(char param, float value);
        void updateParameter(char param, std::string value);
        void eraseParameter(char param);
        void getParameter(char param);
        float getParameterAsFloat(char param);
        signed int getParameterAsInt(char param);
        bool hasParameter(char param) {
            return argsMap.find(param) != argsMap.end();
        }
        std::string getParameterAsString(char param);
        std::string *rawString() {
            return rawCommand;
        }
        std::map<char, std::string> args() {
            return argsMap;
        }
        std::string interpretedString();

    private:

        CommandType type;
        signed short commandNumber;
        std::string *rawCommand;
        std::map<char, std::string> argsMap;

};

#endif