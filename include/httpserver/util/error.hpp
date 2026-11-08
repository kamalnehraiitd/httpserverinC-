#pragma once

#include <cerrno>        
#include <system_error> 

namespace httpserver::util
{
[[noreturn]] inline void throwSystemError(const char* what)
{
    const int code = errno;  
    throw std::system_error(code, std::generic_category(), what);
}
} 