#ifndef VIRTUALSERVER_HPP
# define VIRTUALSERVER_HPP

#include "../../includes/Includes.hpp"
#include "../Utils/Utils.hpp"
#include "../../includes/Classes.hpp"

class VirtualServer {
	public:
		VirtualServer();
		~VirtualServer();

		void setPort(int port);
		void setServerName(std::string serverName);
		void setBodySize(std::string bodySize);
		void setLocation(std::string location);
		void setRoot(std::string root);
		void setIndex(std::vector<std::string> index);

		int getPort(void) const;
		std::string getServerName(void) const;
		std::string getBodySize(void) const;
		std::string getLocation(void) const;
		std::string getRoot(void) const;
		std::vector<std::string> getIndex(void) const;
	private:
		int _port;
		std::string _serverName;
		std::string _bodySize;
		std::string _location;
		std::string _root;
		std::vector<std::string> _index;
};

#endif