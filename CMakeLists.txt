cmake_minimum_required(VERSION 2.6)
project(CHECKIN)

# Set project specific cmake Module location
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Sqlite3 REQUIRED)
include_directories(${SQLITE3_INCLUDE_DIR})
set(LIBS ${LIBS} ${SQLITE3_LIBRARIES})

set(CMAKE_CXX_FLAGS "-g -Wall -std=c11 -pedantic")
add_subdirectory(lib)
