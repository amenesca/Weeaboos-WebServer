/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLog.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:58:13 by femarque          #+#    #+#             */
/*   Updated: 2024/03/15 15:12:47 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerLog.hpp"

ServerLog::ServerLog() {
    _stdout_fd = 0;
    _stderr_fd = 0;
}

ServerLog::~ServerLog() {
    close(_stdout_fd);
    close(_stderr_fd);
}

void ServerLog::createLog() {
    _stdout_fd = open("srcs/ServerLog/logs/stdout.log", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    _stderr_fd = open("srcs/ServerLog/logs/stderr.log", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    dup2(_stdout_fd, STDOUT_FILENO);
    dup2(_stderr_fd, STDERR_FILENO);
    close(_stdout_fd);
    close(_stderr_fd);
}