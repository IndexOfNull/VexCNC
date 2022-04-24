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
            /*if (it == components.begin() && !(commandType == CommandType::ReferenceLast)) { //For non-reference commands, parse the first parameter as the command number
                commandNumber = std::atoi((*it).substr(1).data());
            } else if (argsMap.find(paramKey) == argsMap.end()) { //if it's not the first parameter and it hasn't been placed in the map yet
                argsMap[paramKey] = (*it).substr(1);
            }*/

            if (argsMap.find(paramKey) == argsMap.end()) {
                argsMap[paramKey] = (*it).substr(1);
            }
        }
    }

    if (commandType != CommandType::ReferenceLast) {
        commandNumber = std::atoi(argsMap[firstLetter].data());
        eraseParameter(firstLetter); // Remove the actual command from the param list after extracting the commandNumber
    }

    std::cout << interpretedString() << std::endl;
    // std::cout << argsMap << std::endl;
}

CommandType Command::commandType() {
    return commandType;
}

void Command::updateParameter(char param, std::string value) {
    argsMap[param] = value;
}

std::string Command::getParameterAsString(char param) {
    if (argsMap.find(param) != argsMap.end()) {
        return argsMap[param];
    } else {
        return "";
    }
}

float Command::getParameterAsFloat(char param) {
    std::string paramStr = getParameterAsString(param);
    if (paramStr.size() > 0) {
        return std::atof(paramStr.data());
    } else {
        return Command::nparam;
    }
}

double Command::getParameterAsDouble(char param) {
    std::string paramStr = getParameterAsString(param);
    if (paramStr.size() > 0) {
        return std::atof(paramStr.data());
    } else {
        return Command::nparam;
    }
}


signed int Command::getParameterAsInt(char param) {
    std::string paramStr = getParameterAsString(param);
    if (paramStr.size() > 0) {
        return std::atoi(paramStr.data());
    } else {
        return Command::nparam;
    }
}

void Command::eraseParameter(char param) {
    if (argsMap.find(param) != argsMap.end()) {
        argsMap.erase(param);
    }
}

std::string Command::interpretedString() {
    std::stringstream output;
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