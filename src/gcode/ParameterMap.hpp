#ifndef PARAMETER_MAP_H
#define PARAMETER_MAP_H

#include <map>
#include <string>

class ParameterMap {

    public:

        static const size_t nparam = 0;

        // Update or insert the given value for a parameter.
        void updateParameter(char param, std::string value);

        // Remove the selected parameter from the parameter map.
        void eraseParameter(char param);

        // Retrieves the selected parameter as a string. Returns ParameterMap::nparam if the parameter does not exist.
        std::string getParameterAsString(char param);

        // Retrieves and parses the selected parameter as a float. Returns ParameterMap::nparam if the parameter does not exist.
        float getParameterAsFloat(char param);

        // Retrieves and parses the selected parameter as a double. Returns ParameterMap::nparam if the parameter does not exist.
        double getParameterAsDouble(char param);

        // Retrieves and parses the selected parameter as a double. Returns ParameterMap::nparam if the parameter does not exist.
        signed int getParameterAsInt(char param);

        // Returns true if the given parameter was specified for this command and false otherwise.
        bool hasParameter(char param) {
            return argsMap.find(param) != argsMap.end();
        }

        void merge(ParameterMap *second_map) {
            argsMap.merge(second_map->getMap());
        }

        // Returns a copy of the underlying 
        std::map<char, std::string> getMap() {
            return argsMap;
        }

    private:

        std::map<char, std::string> argsMap;
        

};

#endif
