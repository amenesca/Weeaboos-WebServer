/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:37:17 by femarque          #+#    #+#             */
/*   Updated: 2024/03/05 23:54:03 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgiHandler.hpp"

cgiHandler::cgiHandler() {}

cgiHandler::~cgiHandler() {
    for (std::vector<char*>::iterator it = _env.begin(); it != _env.end(); ++it) {
        delete[] *it;
    }
    _env.clear();
}

std::string cgiHandler::getScriptFilename(const std::string& requestURI) {
	size_t lastSlashPos = requestURI.find_last_of("/");
	if (lastSlashPos == std::string::npos)
		return (requestURI);
	return (requestURI.substr(lastSlashPos + 1));
}

std::vector<char*> cgiHandler::createEnv(std::map<std::string, std::string> requestHeaders, RequestParser request, Client client) {
	std::string clientIp;
	std::string clientPort;
	
	clientIp = inet_ntoa(client.getClientAddrPointer()->sin_addr);
	clientPort = ntohs(client.getClientAddrPointer()->sin_port);

	_env.push_back(strdup(("CONTENT_TYPE=" + requestHeaders["Content-Type"]).c_str()));
	_env.push_back(strdup(("CONTENT_LENGTH=" + requestHeaders["Content-Length"]).c_str()));
	_env.push_back(strdup(("REQUEST_URI=" + request.getUri()).c_str()));
	_env.push_back(strdup(("SCRIPT_NAME=" + request.getUri().substr(1)).c_str()));
	_env.push_back(strdup(("SCRIPT_FILENAME=" + getScriptFilename(request.getUri())).c_str()));

	_env.push_back(strdup(("REMOTE_ADDR=" + clientIp + ":" + clientPort).c_str()));
	_env.push_back(strdup(("SERVER_NAME=" + requestHeaders["Host"]).c_str()));
	_env.push_back(strdup(("SERVER_PORT=" + request.getPortNumber()).c_str()));

	_env.push_back(strdup("AUTH_TYPE=Basic"));
	_env.push_back(strdup("REQUEST_METHOD=POST"));
	_env.push_back(strdup("REDIRECT_STATUS=200"));
	_env.push_back(strdup("DOCUMENT_ROOT=./"));
	_env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
	_env.push_back(strdup(("PATH_INFO=" + request.getUri().substr(1)).c_str()));
	_env.push_back(strdup("PATH_TRANSLATED=.//"));
	_env.push_back(strdup(("QUERY_STRING=" + request.getQueryString()).c_str()));
	_env.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
	_env.push_back(strdup("SERVER_SOFTWARE=AMANIX"));
	_env.push_back(NULL);

	return _env;
}

std::string cgiHandler::configCgi(Client client, RequestParser request)
{
	
	std::vector<char*> headerEnv = createEnv(request.getHeaders(), request, client);
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
		close(pipe_fd[0]); // Feche o descritor de arquivo de leitura após a criação do pipe
		dup2(pipe_fd[1], STDOUT_FILENO); // Redirecione a saída padrão para o pipe
		close(pipe_fd[1]); // Feche o descritor de arquivo de escrita após a duplicação

        // Escreva os dados de entrada no pipe
        std::string inputData = request.getQueryString();
		std::cout << "InputData: " << inputData << std::endl;
        write(pipe_fd[1], inputData.c_str(), inputData.size());
        close(pipe_fd[1]); // Feche o descritor de arquivo de escrita após escrever os dados

        if (execve(argv[1], const_cast<char* const*>(argv.data()), const_cast<char* const*>(headerEnv.data())) == -1) {
			std::cerr << "Error on execve: " << strerror(errno) << std::endl;
		}
    }
	else
	{
		close(pipe_fd[1]);
		waitpid(_pid, &status, 0);
		close(pipe_fd[0]); 
	}
	return 0;
}

int cgiHandler::postCgi(RequestParser postRequest, Client client) {
	char* argv[2];
	std::string path;
	path = postRequest.getUri().substr(1);

	std::vector<char*> headerEnv = createEnv(postRequest.getHeaders(), postRequest, client);

	argv[0] = strdup(path.c_str());
	argv[1] = NULL;

	int response_pipe[2];

	if (pipe(response_pipe) == -1)
    {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
    	return 1;
    }
	if (pipe(_request_pipe) == -1)
    {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
    	return 1;
    }
	noBlockingFD(_request_pipe, response_pipe);
	_pid = fork();

	if (_pid == -1)
	{
		std::cerr << "Error on fork: " << strerror(errno) << std::endl;
		return 1;
	}
	else if (_pid == 0)
	{
		std::cout << "ENTROU NO ELSE IF DO PID\n";
		close(_request_pipe[1]);
		std::cout << "DEPOIS DO CLOSE _REQUEST_PIPE[1]\n";
		close(response_pipe[0]);
		std::cout << "DEPOIS DO CLOSE RESPONSE_PIPE[0]\n";
		dup2(_request_pipe[0], STDIN_FILENO);
		std::cout << "DEPOIS DO DUP2 _REQUEST_PIPE[0]\n";
		close(_request_pipe[0]);
		std::cout << "DEPOIS DO CLOSE _REQUEST_PIPE[0]\n";
		dup2(response_pipe[1], STDOUT_FILENO);
		std::cout << "DEPOIS DO DUP2 RESPONSE_PIPE[1]\n";
		close(response_pipe[1]);
		std::cout << "PATH: " << path << std::endl;
		std::cout << "ANTES DO EXECVE\n";
		if (execve(path.c_str(), argv, _env.data()) == -1) {
			std::cerr << "Error on execve: " << strerror(errno) << std::endl;
			exit(1); // Add exit statement to handle execve failure
		}
		free(argv[0]);
		exit(0); // Add exit statement after successful execve
		std::cout << "SAIU DO ELSE IF DO PID\n";
		return 0;
	}
	else
	{
		close(_request_pipe[0]);
		close(response_pipe[1]);
		return 1;
	}
	return 0;
}

void cgiHandler::noBlockingFD(int *pipe1, int *pipe2){
    fcntl(pipe1[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(pipe1[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(pipe2[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(pipe2[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
}