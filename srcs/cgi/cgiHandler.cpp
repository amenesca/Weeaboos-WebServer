/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:37:17 by femarque          #+#    #+#             */
/*   Updated: 2024/03/07 17:26:49 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgiHandler.hpp"

cgiHandler::cgiHandler() {
	_pid = 0;
	_request_pipe[0] = 0;
	_request_pipe[1] = 0;
	_env = std::vector<char*>();
}

cgiHandler::cgiHandler(RequestParser request) {
	_request = request;
}

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

std::vector<char*> cgiHandler::createEnv(std::map<std::string, std::string> requestHeaders, Client client) {
	std::string clientIp;
	std::string clientPort;
	
	clientIp = inet_ntoa(client.getClientAddrPointer()->sin_addr);
	clientPort = ntohs(client.getClientAddrPointer()->sin_port);
	std::cout << "URI: " << _request.getUri().substr(1).c_str() << std::endl;
	_env.push_back(strdup(("CONTENT_TYPE=" + requestHeaders["Content-Type"]).c_str()));
	_env.push_back(strdup(("CONTENT_LENGTH=" + requestHeaders["Content-Length"]).c_str()));
	_env.push_back(strdup(("REQUEST_URI=" + _request.getUri()).c_str()));
	_env.push_back(strdup(("SCRIPT_NAME=" + _request.getUri().substr(1)).c_str()));
	_env.push_back(strdup(("SCRIPT_FILENAME=" + getScriptFilename(_request.getUri())).c_str()));

	_env.push_back(strdup(("REMOTE_ADDR=" + clientIp + ":" + clientPort).c_str()));
	_env.push_back(strdup(("SERVER_NAME=" + requestHeaders["Host"]).c_str()));
	_env.push_back(strdup(("SERVER_PORT=" + _request.getPortNumber()).c_str()));

	_env.push_back(strdup("AUTH_TYPE=Basic"));
	_env.push_back(strdup("REQUEST_METHOD=POST"));
	_env.push_back(strdup("REDIRECT_STATUS=200"));
	_env.push_back(strdup("DOCUMENT_ROOT=./"));
	_env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
	_env.push_back(strdup("PATH_INFO="));
	_env.push_back(strdup("PATH_TRANSLATED=.//"));
	_env.push_back(strdup("QUERY_STRING="));
	_env.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
	_env.push_back(strdup("SERVER_SOFTWARE=AMANIX"));
	_env.push_back(NULL);

	return _env;
}
std::string cgiHandler::extrairQueryString(const std::string& uri) {
    std::string queryString;

    // Procurar pelo caractere '?' na URI
    size_t pos = uri.find('?');

    // Se encontrarmos o caractere '?', extrair a query string
    if (pos != std::string::npos) {
        queryString = uri.substr(pos + 1); // Extrair a substring após o '?'
    }

    return queryString;
}

int cgiHandler::getCgi()
{
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

	_pid = fork();

	if (_pid == -1)
	{
		std::cerr << "Error on fork: " << strerror(errno) << std::endl;
		return 1;
	}
	else if (_pid == 0)
	{
		char* argv[2];
		std::string path;
		path = _request.getUri().substr(1);
		argv[0] = strdup(path.c_str());
		if (argv[0] == NULL)
		{
			std::cerr << "Error on strdup: " << strerror(errno) << std::endl;
			return 1;
		}
		argv[1] = NULL;

		if (access(path.c_str(), X_OK) == -1) {
			std::cerr << "Error on access: " << strerror(errno) << std::endl;
			free(argv[0]);
			exit(1);
		}
		if (close(response_pipe[0]) == -1) {
  			std::cerr << "Error on close: " << strerror(errno) << std::endl;
			free(argv[0]);
  			exit(1);
		}
		if (dup2(response_pipe[1], STDOUT_FILENO) == -1) {
  			std::cerr << "Error on dup2: " << strerror(errno) << std::endl;
			free(argv[0]);
  			exit(1);
		}
		if (close(response_pipe[1]) == -1) {
  			std::cerr << "Error on close: " << strerror(errno) << std::endl;
			free(argv[0]);
  			exit(1);
		}
		if (execve(path.c_str(), argv, _env.data()) == -1) {
			std::cerr << "Error on execve: " << strerror(errno) << std::endl;
			perror("execvp");
			free(argv[0]);
			exit(1);
		}
		free(argv[0]);
		return 0;
	}
	else
	{
		close(response_pipe[1]);
		return 1;
	}
	return 0;
}

int cgiHandler::postCgi(Client client)
{
	createEnv(_request.getHeaders(), client);
	/*for (std::vector<char*>::iterator it = _env.begin(); it != _env.end(); ++it) {
		std::cout << *it << std::endl;
	}*/
	int response_pipe[2];
	std::cout << "QUERRY STRING: " << extrairQueryString(_request.getUri()) << std::endl;

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

	antiBlock(_request_pipe, response_pipe);
	if (!writePipes(_request.getBody())) {
        return 1;
	}

	_pid = fork();

	if (_pid == -1)
	{
		std::cerr << "Error on fork: " << strerror(errno) << std::endl;
		return 1;
	}
	else if (_pid == 0)
	{
		char* argv[2];
		std::string path;
		path = _request.getUri().substr(1);
		argv[0] = strdup(path.c_str());
		if (argv[0] == NULL)
		{
			std::cerr << "Error on strdup: " << strerror(errno) << std::endl;
			return 1;
		}
		argv[1] = NULL;
		if (access(path.c_str(), X_OK) == -1) {
			std::cerr << "Error on access: " << strerror(errno) << std::endl;
			free(argv[0]);
			exit(1);
		}
		if (close(_request_pipe[1]) == -1) {
  			std::cerr << "Error on close: " << strerror(errno) << std::endl;
			free(argv[0]);
  			exit(1);
		}
		if (dup2(_request_pipe[0], STDIN_FILENO) == -1) {
  			std::cerr << "Error on dup2: " << strerror(errno) << std::endl;
			free(argv[0]);
  			exit(1);
		}
		if (close(_request_pipe[0]) == -1) {
  			std::cerr << "Error on close: " << strerror(errno) << std::endl;
			free(argv[0]);
  			exit(1);
		}
		std::cout << "CHEGOU ANTES DO CLOSE\n";
		if (close(response_pipe[0]) == -1) {
  			std::cerr << "Error on close: " << strerror(errno) << std::endl;
			free(argv[0]);
  			exit(1);
		}
		std::cout << "CHEGOU ANTES DO DUP2\n";
		std::cout << response_pipe[1] << "\n" ;
		if (dup2(response_pipe[1], STDOUT_FILENO) == -1) {
  			std::cerr << "Error on dup2: " << strerror(errno) << std::endl;
			free(argv[0]);
  			exit(1);
		}
		std::cout << "PASSOU DO DUP2\n";
		if (close(response_pipe[1]) == -1) {
  			std::cerr << "Error on close: " << strerror(errno) << std::endl;
			free(argv[0]);
  			exit(1);
		}
		if (execve(path.c_str(), argv, _env.data()) == -1) {
			std::cerr << "Error on execve: " << strerror(errno) << std::endl;
			perror("execvp");
			free(argv[0]);
			exit(1);
		}
		free(argv[0]);
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

void cgiHandler::antiBlock(int *pipe1, int *pipe2)
{
    if (fcntl(pipe1[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cerr << "Error on fcntl: " << strerror(errno) << std::endl;
	}
	if (fcntl(pipe1[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cerr << "Error on fcntl: " << strerror(errno) << std::endl;
	}
	if (fcntl(pipe2[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cerr << "Error on fcntl: " << strerror(errno) << std::endl;
	}
	if (fcntl(pipe2[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cerr << "Error on fcntl: " << strerror(errno) << std::endl;
	}
}

bool        cgiHandler::writePipes(std::vector<std::string> body)
{
    size_t  bytesWritten;
    int bytes;

    bytesWritten = 0;
    while (bytesWritten < body.size())
    {
        bytes = write(_request_pipe[1], body[bytesWritten].c_str(), body[bytesWritten].size());
        if (bytes == -1)
        {
            return (false);
        }
        bytesWritten += bytes;
    }
    return (true);
}