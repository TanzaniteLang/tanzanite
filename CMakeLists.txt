cmake_minimum_required(VERSION 3.26)
project(tanzanite VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -O0 -g -Wall -Wextra -pedantic")

FILE(GLOB_RECURSE PARSER ./parser/*.cpp ./parser/*.hpp)
FILE(GLOB_RECURSE LEXER ./lexer/*.cpp ./lexer/*.hpp)

add_library(parser STATIC ${PARSER})
add_library(lexer STATIC ${LEXER})

include_directories(./parser/)
include_directories(./lexer/)

add_executable(test test.cpp)
target_link_libraries(parser lexer)
target_link_libraries(test lexer parser)
