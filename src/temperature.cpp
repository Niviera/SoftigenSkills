#include <Arduino.h>
#include "temperature.h"


/* Funtions */
float Temperature::readTemperature(){
    /* 
        Read the current on Pin0 an calculates the tempeture of it.
        Calculation is taken from a old Project and is made for the 
        Temp Sensor TMP36
    */
    int sensorValue = analogRead(this->tempPin);
    float voltage = (sensorValue/1025) * 5.0;
    return (voltage - .5) * 100;
}

/* Konstruktor & Destructor */
Temperature::Temperature(short int pinTemp)
{
    tempPin = pinTemp;
}

Temperature::~Temperature()
{
}