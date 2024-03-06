/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:35:28 by femarque          #+#    #+#             */
/*   Updated: 2024/03/05 23:53:18 by femarque         ###   ########.fr       */
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
        pid_t               _pid;
        std::vector<char*>  _env;
        int                 _request_pipe[2];
        void noBlockingFD(int *pipe1, int *pipe2);
	public:
        cgiHandler();
        ~cgiHandler();

        std::vector<char*> createEnv(std::map<std::string, std::string> requestHeaders, RequestParser request, Client client);
        std::string configCgi(Client client, RequestParser request);
        int postCgi(RequestParser postRequest, Client client);
		std::string getScriptFilename(const std::string& requestURI);};
#endif