#pragma once
#include "httpserver/net/address.hpp"
#include "httpserver/net/socket.hpp"
#include "httpserver/net/tcp_stream.hpp"
#include<sys/socket.h>

namespace httpserver::net{
class TcpListener{
    public :
    explicit TcpListener(const Address& adrr,int backlog=SOMAXCONN);
    Address localAddress() const ;
    [[nodiscard]]TcpStream accept() const ;
    private :
        Socket sock_;
        static Socket create_socket() ;

};
}
