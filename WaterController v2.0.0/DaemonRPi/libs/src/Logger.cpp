//
// Created by ahk on 19.06.23.
//

#include "Logger.h"

Logger::Logger(std::string path) {
    this->pathToFile = path;

}

Logger::~Logger() {

}

bool Logger::Open_log_file() {

    std::fstream log;
    bool status = this->Check_if_exist();

    if (!status){
        log.open(this->pathToFile.c_str(),  std::fstream::in | std::fstream::out | std::fstream::trunc);

        std::string announcement = "File created";
        std::string logMessage = this->Info_log(announcement);
        log << logMessage;

        log.close();
    }
    else{
        log.open(this->pathToFile.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);

        std::string announcement = "File opened";
        std::string logMessage = this->Info_log(announcement);
        log << logMessage;
        log.close();
    }



}

std::string Logger::Error_log(std::string &msg) {
    std::string date = Logger::currentDateTime();
    std::string output = "[ ERROR ] " + msg + " { " + date + " }\n";

    return output;
}

std::string Logger::Info_log(std::string &msg) {
    std::string date = this->currentDateTime();
    std::string output = "[ INFO ] " + msg + " { " + date + " }\n";

    return output;
}

std::string Logger::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

bool Logger::Check_if_exist() {
    std::fstream log(this->pathToFile.c_str());
    return log.good();
}
