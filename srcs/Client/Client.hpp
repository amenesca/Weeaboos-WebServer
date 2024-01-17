#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../../includes/Classes.hpp"
# include "../../includes/Includes.hpp"
# include "../../includes/Defines.hpp"

class Client {
    public:
        Client();
        ~Client();

        u_int8_t    **getBufferAddress(void);
        int         getClientSocket() const;
        socklen_t   getClientAddrLen() const;
        SA          getClientAddr() const;
        ssize_t     getBytesRead() const;
    private:
        int			_clientSocket;
        socklen_t   _client_addr_len;
        SA          _client_addr;
        u_int8_t	*_requestBuffer;
        ssize_t     _bytesRead;
};

#endif