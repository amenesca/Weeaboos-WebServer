#ifndef CONFIGPARSER_CPP
#define CONFIGPARSER_CPP

#include "../../includes/Includes.hpp"
#include "../Utils/Utils.hpp"
#include "../../includes/Classes.hpp"

class VirtualServer;
class ConfigParser {
    public:
		ConfigParser();
		~ConfigParser();

		int openConfig();
		int initConfig();
		void setVServers();
		void configServer(VirtualServer* currentServer);
		std::vector<VirtualServer> getVServers(void) const;

		void setConfigFilePath(std::string configFilePath);
		std::string getConfigFilePath() const;

		class OpenError : public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class InvalidSyntax : public std::exception {
			public:
				virtual const char* what() const throw();
		};

	private:
		
		std::string		_configFilePath;
		std::ifstream	_configFileFstream;
		std::vector<VirtualServer> _vServers;
};

std::vector<std::string> split(const std::string& input);

#endif
