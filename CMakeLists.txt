cmake_minimum_required(VERSION 3.16)
project(adfs)

set (CMAKE_CXX_COMPILER "/usr/bin/g++-11")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic  -std=c++20  -Wall -Werror -Wno-deprecated -Wno-unused-function ")

#编译
add_subdirectory(source)