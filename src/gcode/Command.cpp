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
            } else if (paramMap.find(paramKey) == paramMap.end()) { //if it's not the first parameter and it hasn't been placed in the map yet
                paramMap[paramKey] = (*it).substr(1);
            }*/

            if (paramMap.find(paramKey) == paramMap.end()) {
                paramMap[paramKey] = (*it).substr(1);
            }
        }
    }

    if (commandType != CommandType::ReferenceLast) {
        commandNumber = std::atoi(paramMap[firstLetter].data());
        eraseExplicitParameter(firstLetter); // Remove the actual command from the param list after extracting the commandNumber
    }

    std::cout << interpretedString() << std::endl;
    // std::cout << paramMap << std::endl;
}

std::string Command::getParameterAsString(char param, ParameterType type) {
    switch (type) {
        case Explicit:
            if (paramMap.find(param) != paramMap.end()) {
                return paramMap[param];
            } else {
                return "";
            }
        case Implicit:
            if (implicitParamMap.find(param) != implicitParamMap.end()) {
                return implicitParamMap[param];
            } else {
                return "";
            }
        case Either:
            if (paramMap.find(param) != paramMap.end()) {
                return paramMap[param];
            } else if (implicitParamMap.find(param) != implicitParamMap.end()) {
                return implicitParamMap[param];
            } else {
                return "";
            }
        default:
            return "";
    }
}

float Command::getParameterAsFloat(char param, ParameterType type) {
    std::string paramStr = getParameterAsString(param, type);
    if (paramStr.size() > 0) {
        return std::atof(paramStr.data());
    } else {
        return Command::nparam;
    }
}

double Command::getParameterAsDouble(char param, ParameterType type) {
    std::string paramStr = getParameterAsString(param, type);
    if (paramStr.size() > 0) {
        return std::atof(paramStr.data());
    } else {
        return Command::nparam;
    }
}


signed int Command::getParameterAsInt(char param, ParameterType type) {
    std::string paramStr = getParameterAsString(param, type);
    if (paramStr.size() > 0) {
        return std::atoi(paramStr.data());
    } else {
        return Command::nparam;
    }
}



std::string Command::interpretedString() {
    std::stringstream output;
    if (commandType != CommandType::ReferenceLast) {
        output << commandToStr(commandType) << commandNumber;
        if (paramMap.size() > 0) {
            output << " ";
        }
    }

    for (auto it = paramMap.begin(); it != paramMap.end(); it++) {
        if (it != paramMap.begin()) {
            output << " ";
        }
        output << it->first << it->second;
    }

    return output.str();
}