/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:24:56 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/14 13:24:57 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../../includes/Includes.hpp"
# include "../../includes/Defines.hpp"

// Classe Client responsável por guardar informações do cliente conectado,
// dentro do Servidor será um vector que manterá atualizado todos os clientes
// Foi necessário fazer vários getters e setters para funcionar além de
// destructors e constructors personalizados.

class Client {
    public:
        Client();
        ~Client();
		Client(const Client& copy);
		Client& operator=(const Client& copy);


        std::string	getBuffer(void);
        sockaddr_in	*getClientAddrPointer(void);
        socklen_t	*getClientAddrLenPointer(void);
        int			getClientSocket(void) const;
        ssize_t     getBytesRead(void) const;

        void        setClientSocket(int clientSocket);
        void        setClientAddrLen(socklen_t _client_addr_len);
        void        setBytesRead(ssize_t bytesRead);
        void        setRequestBuffer(std::string _requestBuffer);
    private:
        int			_clientSocket;
        socklen_t	_client_addr_len;
        sockaddr_in	_client_addr;
        std::string	_requestBuffer1;
        ssize_t		_bytesRead;
};

#endif