#pragma once

#include "httpserver/net/address.hpp"
#include "httpserver/net/socket.hpp"

#include <string>
#include <string_view>
#include <system_error>
#include <utility>

namespace httpserver::net
{

class TcpStream
{
public:
    const Address& peer() const noexcept { return peer_; }
    int fd() const noexcept { return sock_.fd(); }

    const std::error_code& error() const noexcept { return ec_; }
    bool eof() const noexcept { return eof_; }
    explicit operator bool() const noexcept { return !ec_ && !eof_; }

    TcpStream& operator<<(std::string_view data);
    TcpStream& operator>>(std::string& received);
    [[nodiscard]] bool receiveExact(char* buf, std::size_t len);
    void shutdownWrite();
private:
    friend class TcpListener;

    TcpStream(Socket sock, Address peer) noexcept
        : sock_{std::move(sock)}, peer_{std::move(peer)}
    {
    }

    Socket sock_;
    Address peer_;
    std::error_code ec_{};   
    bool eof_{false};
};

} 