#include "PIDcontroller.h"

PIDcontroller::PIDcontroller()
{
}

void PIDcontroller::SetValuePI(int P, int I)
{
    this->Pcontroller = P;
    this->Icontroller = I;
}