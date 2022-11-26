cmake_minimum_required(VERSION 3.23)
project(DaemonRPi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_subdirectory(libs)
add_subdirectory(main-program)


set(Boost_INCLUDE_DIR /usr/include/boost)
include_directories(${Boost_INCLUDE_DIR})
find_package(Boost COMPONENTS system filesystem REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(DaemonRPi PRIVATE Threads::Threads)

find_package(SQLite3)
target_include_directories(DaemonRPi PRIVATE include/)
target_link_libraries(DaemonRPi LINK_PUBLIC sqlite3)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})