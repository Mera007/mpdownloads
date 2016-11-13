cmake_minimum_required(VERSION 3.5)
project(ADSold)

#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 --pedantic-errors -DETYPE=int ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 --pedantic-errors -DETYPE=std::string ")

set(SOURCE_FILES simpletest.cpp)
add_executable(ADSold ${SOURCE_FILES})