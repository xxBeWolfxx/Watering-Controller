#include "PIDcontroller.h"

PIDcontroller::PIDcontroller()
{
}

void PIDcontroller::SetValuePI(int P, int I)
{
    this->Pcontroller = P;
    this->Icontroller = I;
}

void PIDcontroller::SetValuePID(int P, int I, int D)
{
    this->Pcontroller = P;
    this->Dcontroller = D;
    this->Icontroller = I;
}

int PIDcontroller::PIDwork(int setValue)
{
    this->sampleTime = millis();
    float tempSample = (this->sampleTime - this->sampleTimePriv) / 1000;
    this->sampleTimePriv = this->sampleTime;
    this->error = setValue - this->outputValue;

    switch (typeOfController)
    {
    case 0:
        return 0;
        break;

    case 1:
        this->outputValue = PUnit();
        break;
    case 2:
        this->outputValue = PIUnit(tempSample);
        break;

    case 3:
        this->outputValue = PIDUnit(tempSample);
        break;
    }

    return 1;
}

float PIDcontroller::PUnit()
{
    return this->error * this->Pcontroller;
}

float PIDcontroller::PIUnit(int sample)
{
    this->cumIController += this->error * sample;
    return PUnit() + this->cumIController * this->Icontroller;
}

float PIDcontroller::PIDUnit(int sample)
{

    return PUnit() + PIUnit(sample) + (this->error - this->errorPriv) * this->Dcontroller / sample;
}