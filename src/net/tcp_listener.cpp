#include "httpserver/net/tcp_listener.hpp"
#include "httpserver/util/error.hpp"
#include <unistd.h>

#include <fcntl.h>      
#include <sys/socket.h>

namespace
{
void setNoSigpipe(int fd)
{
    int yes = 1;
    if (::setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &yes, sizeof yes) == -1)
        httpserver::util::throwSystemError("setsockopt(SO_NOSIGPIPE)");
}
void setCloexec(int fd)
{
    if (::fcntl(fd, F_SETFD, FD_CLOEXEC) == -1)
        httpserver::util::throwSystemError("fcntl(FD_CLOEXEC)");
}
} 
namespace httpserver::net{

    TcpListener::TcpListener(const Address& adrr,int backlog) : sock_{create_socket()}{
        int yes=1;
        if(setsockopt(sock_.fd(),SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes)<0){
            util::throwSystemError("socket options failed");      
        }
        if(bind(sock_.fd(),adrr.data(),adrr.size())==-1){
            util::throwSystemError("bind listoner socket failed");
        }
        if(listen(sock_.fd(),backlog)==-1){
            util::throwSystemError("liston failed");
        }
    }
    Socket TcpListener::create_socket()  {
        int fd=socket(AF_INET,SOCK_STREAM,0);
        if(fd==-1){
            util::throwSystemError("listoner socket creation failed");
        }
        Socket temp(fd);
        setCloexec(fd);
        return Socket(std::move(temp));
    }
    TcpStream TcpListener::accept() const {
        Socket peersoc;
        Address peeradd;
        for(;;){
            auto len=peeradd.size();
            int cfd=::accept(sock_.fd(),peeradd.data(),&len);
            if(cfd>=0){
                peersoc=Socket(cfd);
                setCloexec(cfd);
                setNoSigpipe(cfd);
                break;
            }
            else if (errno == EINTR || errno == ECONNABORTED)
                continue; 
            else{
                util::throwSystemError("something wrong with accept");
            }    
        }
        return TcpStream{std::move(peersoc),peeradd};
    }
    Address TcpListener::localAddress() const
    {
        Address local;                       
        socklen_t len = local.size();       
        if (::getsockname(sock_.fd(), local.data(), &len) == -1)
            util::throwSystemError("getsockname");
        return local;
    }

}