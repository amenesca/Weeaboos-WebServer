/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:37:17 by femarque          #+#    #+#             */
/*   Updated: 2024/01/05 18:12:04 by femarque         ###   ########.fr       */
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

std::string cgiHandler::configCgi(int clientSocket)
{
	std::vector<const char*> envp;
	std::vector<const char*>argv;
	argv.push_back("../cgi-bin/");
	argv.push_back("../cgi-bin/index.py");
	argv.push_back(NULL);
	int status;
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
    {
        std::cerr << "Error creating pipe" << std::endl;
        return "";
    }
	envp.push_back("CONTENT_TYPE=text/html");
    envp.push_back("CONTENT_LENGTH=1024");
    envp.push_back("QUERY_STRING=nao_faço_a_menor_ideia");
    envp.push_back("REQUEST_METHOD=GET");
    envp.push_back("SCRIPT_NAME=/cgi-bin/index.py");
    envp.push_back("SERVER_NAME=seila");
    envp.push_back("SERVER_PORT=18000");
    envp.push_back(NULL);
	_pid = fork();
	if (_pid == -1)
	{
		std::cerr << "Error on fork" << std::endl;
	}
	else if (_pid == 0)
	{
		dup2(pipe_fd[0], STDIN_FILENO);
		dup2(pipe_fd[0], STDOUT_FILENO);
		execve(argv[0], const_cast<char* const*>(argv.data()), const_cast<char* const*>(envp.data()));
	}
	else
	{
    	waitpid(_pid, &status, 0);
		close(clientSocket);
	}
	return "";
}