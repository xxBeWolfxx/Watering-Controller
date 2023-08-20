//
// Created by arkadiusz on 27.11.22.
//

#ifndef DAEMONRPI_CONFIGURATION_H
#define DAEMONRPI_CONFIGURATION_H

#include <fstream>
#include <iostream>

using namespace std;

struct settings{
    string ipAddress;
    unsigned short port;
};

class Configuration {

private:
    string buffer;
    fstream file;
    string filename;

public:
    settings config;
    Configuration(string filename);
    ~Configuration();

    enum codeCycle{
        STARTING,
        OPEN_WEBSOCKET,
        VALIDATION_INCOMING_MSG,
        CHECK_MEASURMENTS,
        SENDING_TO_ESP
    };


    void ReadSetting();
};


#endif //DAEMONRPI_CONFIGURATION_H
