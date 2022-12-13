cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(IN204_Messenger VERSION 0.1)

add_executable(Client source/client.cpp source/common.cpp)