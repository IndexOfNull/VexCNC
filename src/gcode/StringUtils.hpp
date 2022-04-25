#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <list>

const std::string WHITESPACE = " \n\r\t\f\v";

inline std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
inline std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
inline std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

inline std::list<std::string> splitStr(std::string input, std::string delimiter) {
    size_t pos = 0;
    std::list<std::string> tokens;
    std::string workingString = input;
    while ((pos = workingString.find(delimiter)) != std::string::npos) {
        std::string substr = workingString.substr(0, pos);
        tokens.push_back(substr);
        workingString.erase(0, pos + delimiter.length());
    }
    tokens.push_back(workingString); //everything after the last delimiter
    return tokens;
}

inline std::string joinStr(std::list<std::string> input, std::string delimiter) {
    std::string finalStr = "";
    /*for (size_t i = 0; i < input.size() - 1; i++) {
        finalStr += input. + delimiter;
    }*/
    size_t i;
    for (auto it = input.begin(); it != input.end(); it++) {
        if (i == input.size()-1) {
            finalStr += *it; //Do not add delimiter to last element
        } else {
            finalStr += *it + delimiter;
        }
        i++;
    }
    return finalStr;
}

#endif