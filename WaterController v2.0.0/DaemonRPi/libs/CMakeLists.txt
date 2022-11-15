add_library(
        libs
        src/Database.cpp
        src/Database.h)
target_include_directories(libs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")