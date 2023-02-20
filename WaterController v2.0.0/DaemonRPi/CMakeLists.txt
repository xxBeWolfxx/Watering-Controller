cmake_minimum_required(VERSION 3.23)
project(DaemonRPi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_subdirectory(libs)
add_subdirectory(main-program)
add_subdirectory(tests)


set(Boost_INCLUDE_DIR /usr/lib/cmake/Boost-1.80.0)
include_directories(${Boost_INCLUDE_DIR})
find_package(Boost REQUIRED COMPONENTS system filesystem json)
set(Boost_USE_STATIC_LIBS ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(DaemonRPi PRIVATE Threads::Threads)

target_link_libraries(tests PRIVATE Threads::Threads)

find_package(SQLite3)
target_include_directories(DaemonRPi PRIVATE include/)
target_link_libraries(DaemonRPi LINK_PUBLIC sqlite3)

target_include_directories(tests PRIVATE include/)
target_link_libraries(tests LINK_PUBLIC sqlite3)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})