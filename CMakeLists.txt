# ---------- Project setup ----------
cmake_minimum_required(VERSION 3.20)
project(httpserver LANGUAGES CXX)

# ---------- Language standard ----------
set(CMAKE_CXX_STANDARD 17)            # -std=c++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # error if the compiler can't do C++17
set(CMAKE_CXX_EXTENSIONS OFF)         # c++17, not gnu++17: standard C++ only
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # build/compile_commands.json for your editor

# Default to Debug if no build type was given
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ---------- Shared settings for every target ----------
# A function so warnings + sanitizers are written once, applied everywhere
function(httpserver_configure target)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Sanitizers need the flag at BOTH compile and link time
        target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
endfunction()

# ---------- Core library: all real code lives here ----------
add_library(httpserver_core STATIC
    src/net/socket.cpp
    src/net/address.cpp
    src/net/tcp_listener.cpp
    src/net/tcp_stream.cpp
)
target_include_directories(httpserver_core PUBLIC include)  # PUBLIC: apps inherit it
httpserver_configure(httpserver_core)

# ---------- Executables: thin mains that use the library ----------
add_executable(server apps/server/main.cpp)
target_link_libraries(server PRIVATE httpserver_core)
httpserver_configure(server)

add_executable(client apps/client/main.cpp)
target_link_libraries(client PRIVATE httpserver_core)
httpserver_configure(client)