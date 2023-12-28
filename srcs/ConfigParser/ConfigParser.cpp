#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {

}

ConfigParser::~ConfigParser() {

}

int ConfigParser::setConfigFilePath(std::string configFilePath) {
    this->configFilePath = configFilePath;

    return 0;
}

std::string ConfigParser::getConfigFilePath() const {
    return this->configFilePath;
}