add_library(
        libs
        src/Database.cpp
        src/Database.h
        src/websocketService.cpp
        src/websocketService.h)
target_include_directories(libs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")