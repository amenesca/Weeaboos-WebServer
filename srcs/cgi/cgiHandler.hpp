/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:35:28 by femarque          #+#    #+#             */
/*   Updated: 2024/01/05 18:08:30 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

#include "../../includes/Includes.hpp"
#include "../../includes/Defines.hpp"
#include "../../includes/Classes.hpp"

class Socket;
class cgiHandler {
    private:
        /*std::map<std::string, std::string>	_env;
        std::map<std::string, std::string>	_headers;*/
		//std::string							_body;
        pid_t                               _pid;
        //std::string							_method;
    public:
        cgiHandler();
        //cgiHandler(std::map<std::string, std::string> env, std::string	body);
        /*cgiHandler(const cgiHandler& obj);
        cgiHandler &operator=(cgiHandler const &rhs);*/
        ~cgiHandler();

        std::string configCgi(int clientSocket, char **envp);
        //void getMethods();
};

#endif