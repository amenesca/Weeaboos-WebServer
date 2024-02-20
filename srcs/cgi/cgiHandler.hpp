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

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "../../includes/Includes.hpp"
# include "../../includes/Defines.hpp"
class cgiHandler {
    private:
        pid_t                               _pid;
    public:
        cgiHandler();
        ~cgiHandler();

        std::string configCgi(int clientSocket, char **envp);
};

#endif