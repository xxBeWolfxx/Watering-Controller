#include "Flowerprofile.hpp"


Flowerprofile::Flowerprofile(uint8_t idealHumidity, int8_t idealTemperature, uint8_t delta, uint8_t interval){
    this->idealHumidity = idealHumidity;
    this->idealTemperature = idealTemperature;
    this->delta = delta;
    this->interval = interval;
    lastTimeMeasurment = millis();
    memset(this->measurmentHumidity, -1, sizeof(this->measurmentHumidity));
    memset(this->measurmentTemperature, -127, sizeof(this->measurmentTemperature));    
}

Flowerprofile::~Flowerprofile()
{
}