#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include "GCodeParser.hpp"
#include "StringUtils.hpp"



GCodeParser::GCodeParser() {};

void GCodeParser::readFile(std::string name) {
    inputData = "";
    std::string line;
    std::ifstream file;
    file.open(name);
    if (file.is_open()) {
        while (getline(file,line)) {
            inputData += line + "\n";
        }
        file.close();
    }
}

void GCodeParser::tokenizeGCode(bool parse) {
    std::list<std::string> lines = splitStr(inputData, "\n");
    for (auto it = lines.begin(); it != lines.end(); it++) {
        std::string line = trim(*it);
        if (line != "" || line != "\n") {
            Command command (&line);
            if (parse == true) {
                command.parse();
            }
            commands.push_back(command);
        }
    }
}

void GCodeParser::printFile() {
    std::cout << inputData << std::endl;
}

void GCodeParser::printTestData() {
    for (auto it = testData.begin(); it != testData.end(); it++) {
        std::cout << *it << std::endl;
    }
}