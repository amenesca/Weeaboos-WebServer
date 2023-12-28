#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include "../../includes/Classes.hpp"
# include "../../includes/Defines.hpp"
# include "../../includes/Includes.hpp"

class WebServer {
    public:
        WebServer();
        ~WebServer();

        int configServer(std::string configFilePath);
    private:
        ConfigParser configs;

};

#endif