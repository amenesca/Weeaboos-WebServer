/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:37:17 by femarque          #+#    #+#             */
/*   Updated: 2024/01/05 16:37:27 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgiHandler.hpp"
#include "../srcs/Socket/Socket.hpp"

cgiHandler::cgiHandler() {}

//cgiHandler::cgiHandler(std::string	body) : _body(body), _pid() {}

/*cgiHandler::cgiHandler(const cgiHandler& obj) : _env(obj._env), _body(obj._body) {}

cgiHandler &cgiHandler::operator=(cgiHandler const &rhs) {
    if (this != &rhs) {
		this->_body = rhs._body;
		this->_env = rhs._env;
	}
	return (*this);
}*/

cgiHandler::~cgiHandler() {}

void cgiHandler::getMethods()
{
	//ainda tentando descobrir mais sobre
	const char *requestMethod = getenv("REQUEST_METHOD");
	 if (requestMethod == NULL) {
        std::cerr << "Error: REQUEST_METHOD not set." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (strcmp(requestMethod, "GET") == 0) {
        /*std::cout << "Content-type: text/html" << std::endl << std::endl;
        std::cout << "<html><body><h1>GET Request</h1></body></html>";*/
    } else if (strcmp(requestMethod, "POST") == 0) {
        /*std::cout << "Content-type: text/html" << std::endl << std::endl;
        std::cout << "<html><body><h1>POST Request</h1></body></html>";*/
    } else if (strcmp(requestMethod, "PUT") == 0) {
        //blablabla
    } else if (strcmp(requestMethod, "DELETE") == 0) {
        //blablabla
    }
	//OUTROS METODOS
}


void cgiHandler::initEnv()
{

}

void cgiHandler::configCgi(int clientSocket)
{
	std::vector<const char*> envp;
	//const char **argv;
	int status;
	const char *scriptPath = "../../cgi-bin/index.py";
	envp[0] = "CONTENT_TYPE=text/html",
    envp[1] = "CONTENT_LENGTH=1024",
    envp[2] = "QUERY_STRING=nao_faço_a_menor_ideia",
    envp[3] = "REQUEST_METHOD=GET",
    envp[4] = "SCRIPT_NAME=/cgi-bin/index.py",
    envp[5] = "SERVER_NAME=seila",
    envp[6] = "SERVER_PORT=18000",
    envp[7] = NULL;
	_pid = fork();
	if (_pid == -1)
	{
		std::cerr << "Error on fork" << std::endl;
	}
	else if (_pid == 0)
	{
		dup2(clientSocket, STDOUT_FILENO);
        close(clientSocket);
		//const char *scriptPath = "../../cgi-bin/index.py";
		/*argv = new const char*[2];
        argv[0] = scriptPath;
        argv[1] = NULL;*/
		 std::vector<const char*> envpArray = envp;
		execve(scriptPath, const_cast<char* const*>(envpArray.data()), NULL);
		exit(EXIT_FAILURE);
	}
	else
	{
    	waitpid(_pid, &status, 0);
		close(clientSocket);
	}
}