#include "sensorTemperatureHandler.h"
#include <DallasTemperature.h>

extern DallasTemperature sensorTemperature;

float readTemnperature() {
    sensorTemperature.requestTemperatures();
    float temperature = sensorTemperature.getTempCByIndex(0);
    return temperature;
}