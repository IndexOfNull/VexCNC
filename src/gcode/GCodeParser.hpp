#ifndef GCODEPARSER__H
#define GCODEPARSER__H

#include <string>
#include <list>
#include "Command.hpp"

class GCodeParser {

    private:
        std::list<std::string> testData;
        std::list<Command> commands;

    public:

        GCodeParser();

        std::string inputData;

        void readFile(std::string name);
        void printFile();
        void printTestData();

        // Converts inputData to Command instances and populates GCodeParser::commands with the resulting commands. Parse will call Command.parse() on each new command.
        void tokenizeGCode(bool parse = true);

        std::list<Command>* getCommands() {
            return &commands;
        }

};

#endif