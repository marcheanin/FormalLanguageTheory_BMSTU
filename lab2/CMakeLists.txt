cmake_minimum_required(VERSION 3.22)
project(lab2)

find_package(Boost 1.8.1 REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

message(STATUS ${Boost_LIBRARIES})

set(CMAKE_CXX_STANDARD 20)

add_executable(lab2 main.cpp)
target_link_libraries(lab2 ${Boost_LIBRARIES} boost_regex)
