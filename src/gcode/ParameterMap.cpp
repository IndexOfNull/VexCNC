#include "ParameterMap.hpp"
#include <string>
#include <stdio.h>

void ParameterMap::updateParameter(char param, std::string value) {
    argsMap[param] = value;
}

std::string ParameterMap::getParameterAsString(char param) {
    if (argsMap.find(param) != argsMap.end()) {
        return argsMap[param];
    } else {
        return "";
    }
}

float ParameterMap::getParameterAsFloat(char param) {
    std::string paramStr = getParameterAsString(param);
    if (paramStr.size() > 0) {
        return std::atof(paramStr.data());
    } else {
        return ParameterMap::nparam;
    }
}

double ParameterMap::getParameterAsDouble(char param) {
    std::string paramStr = getParameterAsString(param);
    if (paramStr.size() > 0) {
        return std::atof(paramStr.data());
    } else {
        return ParameterMap::nparam;
    }
}


signed int ParameterMap::getParameterAsInt(char param) {
    std::string paramStr = getParameterAsString(param);
    if (paramStr.size() > 0) {
        return std::atoi(paramStr.data());
    } else {
        return ParameterMap::nparam;
    }
}

void ParameterMap::eraseParameter(char param) {
    if (argsMap.find(param) != argsMap.end()) {
        argsMap.erase(param);
    }
}