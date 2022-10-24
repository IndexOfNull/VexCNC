#include "Command.hpp"
#include "StringUtils.hpp"
#include <iostream>
#include <sstream>

Command::Command(std::string *command) {
    rawCommand = command;
};

Command::~Command() {

};

void Command::parse() {
    std::string workingCommand = *rawCommand;

    if (workingCommand.empty()) {
        return;
    }

    char firstLetter = workingCommand.at(0);
    switch (firstLetter) {
        case 'G':
            commandType = CommandType::G;
            break;
        case 'M':
            commandType = CommandType::Meta;
            break;
        case 'T':
            commandType = CommandType::Tool;
            break;
        case 'D':
            commandType = CommandType::Debug;
            break;
        default:
            commandType = CommandType::ReferenceLast;
            break;
    }

    size_t lastSemicolonPos;
    if ((lastSemicolonPos = workingCommand.find_first_of(";")) != std::string::npos) {
        workingCommand.erase(lastSemicolonPos);
    }

    workingCommand = trim(workingCommand);

    // Split the command into args (letter->value)
    std::list<std::string> components = splitStr(workingCommand, " ");
    for (auto it = components.begin(); it != components.end(); it++) {
        if ((*it).size() > 0) {
            char paramKey = (*it).at(0);
            if (!parameters.hasParameter(paramKey)) {
                parameters.updateParameter(paramKey, (*it).substr(1));
            }
        }
    }

    if (commandType != CommandType::ReferenceLast) {
        commandNumber = parameters.getParameterAsInt(firstLetter);
        parameters.eraseParameter(firstLetter); // Remove the actual command from the param list after extracting the commandNumber
    }

    // std::cout << interpretedString() << std::endl;
    // std::cout << argsMap << std::endl;
}

std::string Command::interpretedString() {
    std::stringstream output;
    std::map<char, std::string> argsMap = parameters.getMap();
    if (commandType != CommandType::ReferenceLast) {
        output << commandToStr(commandType) << commandNumber;
        if (argsMap.size() > 0) {
            output << " ";
        }
    }

    for (auto it = argsMap.begin(); it != argsMap.end(); it++) {
        if (it != argsMap.begin()) {
            output << " ";
        }
        output << it->first << it->second;
    }

    return output.str();
}