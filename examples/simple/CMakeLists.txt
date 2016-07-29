cmake_minimum_required(VERSION 3.4.3)
project(http_server)

add_executable(http_server main.cpp)

target_link_libraries (http_server asio_http)
