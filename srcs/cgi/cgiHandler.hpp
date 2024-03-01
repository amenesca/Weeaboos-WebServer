/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:35:28 by femarque          #+#    #+#             */
/*   Updated: 2024/03/01 13:07:07 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "../../includes/Includes.hpp"
# include "../../includes/Defines.hpp"
# include "../RequestParser/RequestParser.hpp"
# include "../Client/Client.hpp"

class cgiHandler {
    private:
        pid_t                               _pid;
	public:
        cgiHandler();
        ~cgiHandler();

        std::string configCgi(int clientSocket, char **envp);
        std::string postCgi(RequestParser postRequest, Client client);
		std::string getScriptFilename(const std::string& requestURI);
		std::vector<char*> createEnv(std::map<std::string, std::string> requestHeaders, RequestParser request, Client client);
};

#endif