cmake_minimum_required(VERSION 3.5)
project(week1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -g -Wall -Wextra -pedantic")

set(SOURCES parser.hpp lexer.hpp test-lexer.cpp ast.hpp scope.hpp interpreter.hpp bricks/brick-types bricks/brick-assert)

add_executable(week1_test ${SOURCES})
