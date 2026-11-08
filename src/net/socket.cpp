#include "httpserver/net/socket.hpp"
#include<unistd.h>
#include<cassert>
namespace httpserver::net{
Socket::Socket(int fd) noexcept : fd_{fd}{}
Socket::Socket(Socket&& other) noexcept : fd_{other.fd_}{
    other.fd_=-1;
}
Socket& Socket::operator=(Socket&& other) noexcept{
    if(this==&other){
        return *this;
    }
    if(fd_!=-1) ::close(fd_);
    fd_=other.fd_;
    other.fd_=-1;
    return *this;
}
Socket::~Socket() {
     if(fd_!=-1) ::close(fd_);
}
int Socket::fd() const noexcept {
    assert(valid());
    return fd_;
}
int Socket::release() noexcept{
    int ret=fd_;
    fd_=-1;
    return ret;
}
bool Socket::valid() const noexcept {
    return fd_>-1;
}
}
