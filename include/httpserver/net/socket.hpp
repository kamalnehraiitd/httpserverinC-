#pragma once
namespace httpserver::net{
class Socket
{
public:
    Socket()=default;
    explicit Socket(int fd) noexcept;
    Socket(const Socket&)=delete;
    Socket& operator=(const Socket&)=delete;
    Socket& operator=(Socket&&) noexcept;
    Socket(Socket&&) noexcept;
    ~Socket();
    int fd() const noexcept ;
    [[nodiscard]] int release() noexcept ;
    bool valid() const  noexcept;

private:
    int fd_{-1};
};
}




