#include "httpserver/net/tcp_listener.hpp"
#include "iostream"
using namespace httpserver::net;

int main(){
   TcpListener t(Address::any(8080));
   TcpStream x=t.accept();
   std::cout<<x.peer().toString()<<"\n";
   std::string out;
   while (true)
   {
    x>>out;
    std::cout<<out;
    std::string input;
    std::cin>>input;
    x<<input<<"\n";
   }

}