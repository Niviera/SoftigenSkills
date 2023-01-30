#include <Arduino.h>
#include "temperature.h"


/* Funtions */
float Temperature::readTemperature(){
    /* 
        TODO: Fix temp Curve
    */
    int sensorValue = analogRead(this->tempPin);
    float voltage = (float)sensorValue * 2.8;
    voltage /= 1024.0;
    float temp = (voltage - .5) * 100;
    return temp;
}

/* Konstruktor & Destructor */
Temperature::Temperature(short int pinTemp)
{
    tempPin = pinTemp;
}

Temperature::~Temperature()
{
}