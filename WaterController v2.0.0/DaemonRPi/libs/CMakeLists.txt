add_library(
        libs
        src/Database.cpp
        src/Database.h
        src/WebsocketService.cpp
        src/WebsocketService.h src/Configuration.cpp src/Configuration.h src/Record.cpp src/Record.h src/ListenerWebsocket.h src/ListenerWebsocket.cpp src/Logger.cpp src/Logger.h)
target_include_directories(libs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")