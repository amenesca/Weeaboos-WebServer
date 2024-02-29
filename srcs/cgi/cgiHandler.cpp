/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:37:17 by femarque          #+#    #+#             */
/*   Updated: 2024/02/29 15:48:13 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgiHandler.hpp"

cgiHandler::cgiHandler() {}

cgiHandler::~cgiHandler() {}

std::string cgiHandler::getScriptFilename(const std::string& requestURI) {
	size_t lastSlashPos = requestURI.find_last_of("/");
	if (lastSlashPos == std::string::npos)
		return (requestURI);
	return (requestURI.substr(lastSlashPos + 1));
}

std::string cgiHandler::configCgi(int clientSocket, char **envp)
{
	std::vector<const char*> argv;
	argv.push_back("/usr/bin/python3");
	argv.push_back("./cgi-bin/index.py");
	argv.push_back(NULL);
	int status;
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
    {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
    	return "";
    }
	_pid = fork();
	if (_pid == -1)
	{
		std::cerr << "Error on fork: " << strerror(errno) << std::endl;
	}
	else if (_pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDIN_FILENO);
        dup2(clientSocket, STDOUT_FILENO);
        close(pipe_fd[1]);

        // Escreva os dados de entrada no pipe
        std::string inputData = "Aqui estão os dados de entrada que você deseja passar para o script Python.";
        write(pipe_fd[1], inputData.c_str(), inputData.size());
        close(pipe_fd[1]); // Feche o descritor de arquivo de escrita após escrever os dados

        if (execve(argv[0], const_cast<char* const*>(argv.data()), envp) == -1) {
            std::cerr << "Error on execve: " << strerror(errno) << std::endl;
        }
    }
	else
	{
		close(pipe_fd[1]);
		close(clientSocket);
		waitpid(_pid, &status, 0);
		close(pipe_fd[0]); 
	}
	return "";
}

std::vector<char*> cgiHandler::createEnv(std::map<std::string, std::string> requestHeaders, RequestParser request, Client client) {
	std::vector<char*> env;
	// std::string clientIp;
	// std::string clientPort;
	
	// clientIp = inet_ntoa(client.getClientAddr().sin_addr);
	// clientPort = ntohs(client.getClientAddr().sin_port);
	(void)client;

	env.push_back(strdup(("CONTENT_TYPE=" + requestHeaders["Content-Type"]).c_str()));
	env.push_back(strdup(("CONTENT_LENGTH=" + requestHeaders["Content-Length"]).c_str()));
	env.push_back(strdup(("REQUEST_URI=" + request.getUri()).c_str()));
	env.push_back(strdup(("SCRIPT_NAME=" + request.getUri().substr(1)).c_str()));
	env.push_back(strdup(("SCRIPT_FILENAME=" + getScriptFilename(request.getUri())).c_str()));

	return env;
}

std::string cgiHandler::postCgi(char **envp, RequestParser postRequest, Client client) {
	std::vector<const char*> argv;

	std::vector<char*> headerEnv = createEnv(postRequest.getHeaders(), postRequest, client);
	
	argv.push_back("/usr/bin/python3");
	argv.push_back("./cgi-bin/index.py");
	argv.push_back(NULL);
	int status;
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
    {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
    	return "";
    }
	_pid = fork();
	if (_pid == -1)
	{
		std::cerr << "Error on fork: " << strerror(errno) << std::endl;
	}
	else if (_pid == 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		if (execve(argv[0], const_cast<char* const*>(argv.data()), envp) == -1) {
			std::cerr << "Error on execve: " << strerror(errno) << std::endl;
		}
	}
	else
	{
		close(pipe_fd[1]);
		waitpid(_pid, &status, 0);
		close(pipe_fd[0]); 
	}
	return "";
}