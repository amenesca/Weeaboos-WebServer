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

cgiHandler::cgiHandler() {}

cgiHandler::~cgiHandler() {}

std::string cgiHandler::configCgi(int clientSocket, char **envp)
{
	//std::vector<const char*> envp;
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