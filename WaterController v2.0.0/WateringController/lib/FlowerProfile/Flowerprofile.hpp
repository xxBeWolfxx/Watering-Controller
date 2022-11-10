#ifndef FLOWERPROFILE_HPP
#define FLOWERPROFILE_HPP

#include <stdint.h>
#include <cstring>
#include <Arduino.h>

class Flowerprofile
{
private:
    uint8_t idealHumidity;
    int8_t idealTemperature;
    uint8_t delta;
    uint8_t interval;
    uint32_t lastTimeMeasurment;


public:

    int8_t measurmentHumidity[20];
    int8_t measurmentTemperature[20];


    Flowerprofile(uint8_t idealHumidity, int8_t idealTemperature, uint8_t delta, uint8_t interval);

    ~Flowerprofile();
};


 



























#endif