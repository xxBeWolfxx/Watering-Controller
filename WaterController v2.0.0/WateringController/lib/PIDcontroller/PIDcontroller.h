#ifndef PIDcontroller_h
#define PIDcontroller_h

#include "Arduino.h"

class PIDcontroller
{
private:
    int typeOfController;

    int Pcontroller = 0;
    int Icontroller = 0;
    int Dcontroller = 0;

    int error = 0;
    int errorPriv = 0;

    int sampleTime = 0;
    int sampleTimePriv = 0;

public:
    void SetValuePI(int P, int I);
    void SetValuePID(int P, int I, int D);

    PIDcontroller();
    ~PIDcontroller();
}