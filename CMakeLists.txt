cmake_minimum_required(VERSION 3.7)
project(scriptable-guitar-pedal)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(Ne10 REQUIRED)
find_package(CivetWeb REQUIRED)
find_package(LibSndFile REQUIRED)

add_executable(scriptable-guitar-pedal src/main.cpp src/sampledata.cpp src/filtereffect.cpp src/dspmath.cpp)
target_link_libraries(scriptable-guitar-pedal ${NE10_LIBRARIES} ${CIVETWEB_LIBRARIES} ${LIBSNDFILE_LIBRARIES})
include_directories("${CMAKE_SOURCE_DIR}/include" ${NE10_INCLUDE_DIRS} ${CIVETWEB_INCLUDE_DIRS} ${LIBSNDFILE_INCLUDE_DIRS})
