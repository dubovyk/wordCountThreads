cmake_minimum_required(VERSION 3.7)
project(lab2_2)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


set(SOURCE_FILES main.cpp inc/t_measure.h inc/threaded_counter.h src/threaded_counter.cpp)
add_executable(lab2_2 ${SOURCE_FILES})


include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(lab2_2 ${Boost_LIBRARIES})