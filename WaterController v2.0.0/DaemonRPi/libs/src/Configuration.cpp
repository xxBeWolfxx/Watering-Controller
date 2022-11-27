//
// Created by arkadiusz on 27.11.22.
//

#include "Configuration.h"

Configuration::Configuration(string filename) {
    this->filename = filename;
    this->file.open(this->filename.c_str(), std::ios::in | std::ios::out);

    if (this->file.good() == true){
        cout << "Everything is good" << endl;
    }
    else{
        cout << "File dosen't exist" << endl;
        ofstream MyFile(filename.c_str());
        MyFile << "IP: 192.168.0.100\nPORT: 1000";
        MyFile.close();
    }
    this->file.close();

}

void Configuration::ReadSetting() {
    uint8_t counter = 0;
    this->file.open(this->filename.c_str(), std::ios::in | std::ios::out);
    if(this->file.good()){
        while(getline(this->file, this->buffer)){
            switch(counter){
                case 0: {
                    char temp[18] = {0,};
                    sscanf(this->buffer.c_str(), "IP: %s", temp);
                    this->config.ipAddress = string(temp);
                    break;
                }
                case 1: {
                    sscanf(this->buffer.c_str(), "PORT: %d", &this->config.port);
                    break;
                }
            }
            counter++;
        }
    }

}

Configuration::~Configuration() {
    this->file.close();
}

