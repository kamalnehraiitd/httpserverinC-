#include<arpa/inet.h>
#include<stdexcept>
#include<cassert>
#include<httpserver/net/address.hpp>
#include "httpserver/util/error.hpp"
namespace httpserver::net{
    Address::Address() noexcept {
        addr_.sin_len=sizeof(addr_);
        addr_.sin_family=AF_INET;
    }
    Address Address::any(uint16_t port) noexcept{
        Address output;
        output.addr_.sin_port=htons(port);
        output.addr_.sin_addr.s_addr=htonl(INADDR_ANY);
        return output;
    }
    Address Address::loopback(uint16_t port) noexcept{
        Address output;
        output.addr_.sin_port=htons(port);
        output.addr_.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
        return output;
    }
    Address Address::fromIpv4(std::string_view ip,uint16_t port){
        std::string ipcp{ip};
        Address output;
        output.addr_.sin_port=htons(port);
        int capture=inet_pton(AF_INET,ipcp.c_str(),&output.addr_.sin_addr);
        if(capture==1){
            return output;
        }
        if(capture==-1){
            util::throwSystemError("inet_pton");
        }
        throw std::invalid_argument("invalid IPv4 adress");
    }
    Address::Address(const sockaddr_in& other) noexcept : addr_{other}{
        
    }
    std::uint16_t Address::port() const noexcept{
        return ntohs(addr_.sin_port);
    }
    std::string Address::ip() const {
        char buf[INET_ADDRSTRLEN];
        const char* ret=inet_ntop(AF_INET,&addr_.sin_addr,buf,sizeof(buf));
        assert(ret!=nullptr);
        return std::string(buf);
    }
    std::string Address::toString()const
    {
        return ip()+":"+std::to_string(port());
    }   
    bool operator==(const Address& a, const Address& b) noexcept{
        return (a.addr_.sin_family==b.addr_.sin_family) &&( a.addr_.sin_port==b.addr_.sin_port)&&(a.addr_.sin_addr.s_addr==b.addr_.sin_addr.s_addr);
    }
    bool operator!=(const Address& a, const Address& b) noexcept{
        return !(a==b);
    }
    const sockaddr* Address::data() const noexcept{
        return reinterpret_cast<const sockaddr*>(&addr_);
    }
    sockaddr* Address::data() noexcept{
        return reinterpret_cast<sockaddr*>(&addr_);
    }
    socklen_t Address::size() const noexcept {
        return static_cast<socklen_t>(sizeof(addr_));
    }
}
