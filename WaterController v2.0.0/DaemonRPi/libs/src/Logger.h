//
// Created by ahk on 19.06.23.
//

#ifndef DAEMONRPI_LOGGER_H
#define DAEMONRPI_LOGGER_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>


class Logger {

private:
    std::string pathToFile;
    static std::string currentDateTime();

public:

    Logger(std::string path);
    ~Logger();

    bool Open_log_file();
    bool Check_if_exist();

    std::string Info_log(std::string &msg);
    std::string  Error_log(std::string &msg);


};


#endif //DAEMONRPI_LOGGER_H
