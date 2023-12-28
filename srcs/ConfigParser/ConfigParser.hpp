#ifndef CONFIGPARSER_CPP
#define CONFIGPARSER_CPP

#include "../../includes/Includes.hpp"

class ConfigParser {
    public:
		ConfigParser();
		~ConfigParser();

		int openReadEvaluate();

		void setConfigFilePath(std::string configFilePath);
		std::string getConfigFilePath() const;

		class OpenError : public std::exception {
			public:
				virtual const char* what() const throw();
		};

	private:
		std::string		_configFilePath;
		std::string		_configLineRead;
		std::ifstream	_configFileFstream;
};

#endif