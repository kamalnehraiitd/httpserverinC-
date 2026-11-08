#include "httpserver/net/tcp_stream.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <cerrno>
#include <cstddef>

namespace
{

constexpr std::size_t kRecvChunk = 16 * 1024;


ssize_t recvSome(int fd, char* buf, std::size_t len)
{
    for (;;)
    {
        ssize_t n = ::recv(fd, buf, len, 0);
        if (n == -1 && errno == EINTR)
            continue;
        return n;
    }
}


int sendAll(int fd, const char* buf, std::size_t len)
{
    while (len > 0)
    {
        ssize_t n = ::send(fd, buf, len, 0);
        if (n == -1)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        buf += n;
        len -= static_cast<std::size_t>(n);
    }
    return 0;
}

} 

namespace httpserver::net
{

TcpStream& TcpStream::operator>>(std::string& received)
{
    received.clear();
    if (!*this)                
        return *this;

    received.resize(kRecvChunk);
    ssize_t n = recvSome(sock_.fd(), received.data(), received.size());

    if (n > 0)
    {
        received.resize(static_cast<std::size_t>(n));
    }
    else if (n == 0)
    {
        eof_ = true;           
        received.clear();
    }
    else
    {
        ec_ = std::error_code(errno, std::generic_category());  
        received.clear();
    }
    return *this;
}

TcpStream& TcpStream::operator<<(std::string_view data)
{
    if (ec_)                   
        return *this;

    if (sendAll(sock_.fd(), data.data(), data.size()) == -1)
        ec_ = std::error_code(errno, std::generic_category());
    return *this;
}
bool TcpStream::receiveExact(char* buf, std::size_t len)
{
    if (!*this)
        return false;

    std::size_t got = 0;
    while (got < len)
    {
        ssize_t n = recvSome(sock_.fd(), buf + got, len - got); 
        if (n > 0)
        {
            got += static_cast<std::size_t>(n);
        }
        else if (n == 0)
        {
            if (got == 0)
                eof_ = true;   
            else
                ec_ = std::make_error_code(std::errc::connection_reset);  
            return false;
        }
        else
        {
            ec_ = std::error_code(errno, std::generic_category());
            return false;
        }
    }
    return true;
}

void TcpStream::shutdownWrite()
{
    if (ec_)
        return;
    if (::shutdown(sock_.fd(), SHUT_WR) == -1)
        ec_ = std::error_code(errno, std::generic_category());
}
} 