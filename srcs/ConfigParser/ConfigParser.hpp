#ifndef CONFIGPARSER_CPP
#define CONFIGPARSER_CPP

#include "../../includes/Includes.hpp"

class ConfigParser {
    public:
        ConfigParser();
        ~ConfigParser();

        int setConfigFilePath(std::string configFilePath);
        
        std::string getConfigFilePath() const;
    private:
        std::string configFilePath;
};

#endif