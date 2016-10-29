
# CMake version
cmake_minimum_required (VERSION 2.6)

project(exact_trie C CXX)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Show the compile process
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(Boost_USE_STATIC_LIBS ON) 
set(CMAKE_C_FLAGS "-g -Wall -Werror -std=gnu99")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=gnu++11")
set(CMAKE_CXX_COMPILER "g++")

aux_source_directory(. DIR_SRCS)
add_executable(exact_trie ${DIR_SRCS})


