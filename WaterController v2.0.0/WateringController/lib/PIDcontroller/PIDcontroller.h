#ifndef PIDcontroller_h
#define PIDcontroller_h

#include "Arduino.h"

#define MAXVALUE 255
#define SAMPLETIME 0.02

class PIDcontroller {
private:
    int typeOfController = 0;

    float Pcontroller = 0;
    float Icontroller = 0;
    float Dcontroller = 0;

    float error = 0;
    float errorPriv = 0;

    float cumIController = 0;

    float sampleTime = 0;
    float sampleTimePriv = 0;
    int outputValue = 0;

public:
    void SetValuePI(int P, int I);
    void SetValuePID(int P, int I, int D);

    int PIDwork(int setValue);
    float PUnit();
    float PIUnit(int sample);
    float PIDUnit(int sample);

    PIDcontroller();
    ~PIDcontroller();
};

#endif