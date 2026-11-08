#pragma once
#include<netinet/in.h>
#include<cstdint>
#include<string>
#include<string_view>
#include<sys/socket.h>
namespace httpserver::net{
class  Address
{

public:

    Address() noexcept;
    explicit Address(const sockaddr_in&other) noexcept ;
    static Address any(uint16_t port) noexcept;
    static Address loopback(uint16_t port) noexcept;
    static Address fromIpv4(std::string_view ip,uint16_t port);
    
    std::uint16_t port() const noexcept;
    std::string ip() const ;
    std::string toString() const ;
    friend bool operator==(const Address& a,const Address& b) noexcept;
    friend bool operator!=(const Address& a,const Address& b) noexcept;

    const sockaddr* data() const noexcept;
    sockaddr* data() noexcept;
    socklen_t size() const noexcept;

private:
    sockaddr_in addr_{};
    

};
}



