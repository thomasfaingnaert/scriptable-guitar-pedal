cmake_minimum_required(VERSION 3.10)
project(scriptable-guitar-pedal)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(Ne10 REQUIRED)

add_executable(scriptable-guitar-pedal src/main.cpp)
target_link_libraries(scriptable-guitar-pedal ${NE10_LIBRARIES})
include_directories("${CMAKE_SOURCE_DIR}/include" ${NE10_INCLUDE_DIRS})
