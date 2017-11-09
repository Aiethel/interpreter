cmake_minimum_required(VERSION 3.5)
project(interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -g -Wall -Wextra -pedantic")

include_directories(bricks)

set(SOURCES parser.hpp lexer.hpp test-lexer.cpp ast.hpp scope.hpp interpreter.hpp bricks/brick-types bricks/brick-assert)

add_executable(test_interpreter ${SOURCES})

add_custom_command(TARGET test_interpreter POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/test ${CMAKE_BINARY_DIR}/test)
