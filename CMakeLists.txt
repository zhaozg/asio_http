cmake_minimum_required(VERSION 3.4.3)
project(asio_http)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ASIO_HTTP_BUILD_EXAMPLES "Build asio_http examples" ON)
#option(ASIO_HTTP_BUILD_TESTS "Build asio_http tests" OFF)

add_definitions(-DASIO_STANDALONE -DASIO_SEPARATE_COMPILATION)

include_directories(asio_http PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/vendor/asio/asio/include")

add_library(asio_http src/http_parser.c vendor/asio/asio/src/asio.cpp)

install(DIRECTORY include/ DESTINATION include)
install(TARGETS asio_http
		  RUNTIME DESTINATION bin
		  LIBRARY DESTINATION lib
		  ARCHIVE DESTINATION lib)

if(ASIO_HTTP_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

if(ASIO_HTTP_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
