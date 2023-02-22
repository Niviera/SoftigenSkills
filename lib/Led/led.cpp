#include "led.h"
#include <Arduino.h>
#include <FastLED.h>


// konstruktor & destructor
Led::Led(/* args */)
{
}

Led::Led(int green, int blue, int red, int numLeds){
    redPin = red;
    bluePin = blue;
    greenPin = green;

    NUM_LEDS = numLeds;



}
Led::~Led()
{
}

int Led::red(float *ptemperatur){    
    /* Für Bereich -unendlich - 10C*/
    if(*(ptemperatur) < -5.00){
        redValue = 25;
        return redValue;
    }
    if(*(ptemperatur) >= -5 && *(ptemperatur) < 0){
        redValue = int(-5 * *(ptemperatur));
        return redValue;
    }
    if(*(ptemperatur) >= 0 && *(ptemperatur) < 10){
        redValue = 0;
        return redValue;
    }
    /* Für bereich 10C - 15C */
    if(*(ptemperatur) >= 10 && *(ptemperatur) < 15){
        redValue = int(10.8 * *(ptemperatur)) - 44;   //118
        return redValue;
    }
    /* Für Bereich 15 bis 20 */
    if(*(ptemperatur) >= 15 && *(ptemperatur) < 20){
        redValue = int(1.2 * *(ptemperatur)) + 100; //124   
        return redValue;
    }
    /* Für bereich 20 bis 25 */
    if(*(ptemperatur) >= 20 && *(ptemperatur) < 25){
        redValue = int(-24.8 * *(ptemperatur)) + 620; //0
        return redValue;
    }
    /* Für bereich 25 bis 30 */
    if(*(ptemperatur) >= 25 && *(ptemperatur) < 30){
        redValue = int(51 * *(ptemperatur)) - 1275; //255
        return redValue;
    }
    redValue = 255;
    return redValue;
}

int Led::green(float *ptemperatur){
    /* Für Bereich -unendlich - 5C*/
    if(*(ptemperatur) < -5){
        greenValaue = 204;
        return greenValaue;
    }
    if(*(ptemperatur) >= -5 && *(ptemperatur) < 0){
        greenValaue =  int(-30.6 * *(ptemperatur)) + 51;
        return greenValaue;
    }
    if(*(ptemperatur) >=0 && *(ptemperatur) < 5){
        greenValaue =  51;
        return greenValaue;
    }
    /* Für Bereich 5C bis 10C */
    if(*(ptemperatur) >= 5 && *(ptemperatur) < 10){
        greenValaue =  int(31 * *(ptemperatur)) - 104; //206
        return greenValaue;
    }
    /* Für Bereich 10C Bis 15C */
    if(*(ptemperatur) >= 10 && *(ptemperatur) < 15){
        greenValaue =  int(9.8 * *(ptemperatur)) + 108;   //255   
        return greenValaue;     
    }
    /* Für Bereich 15 bis 20 */
    if(*(ptemperatur) >= 15 && *(ptemperatur) < 20){
        greenValaue =  int(-0.6 * *(ptemperatur)) + 264; //252
        return greenValaue;
    }
    /* Für bereich 20 - 25 */
    if(*(ptemperatur) >= 20 && *(ptemperatur) <25){
        greenValaue =  255;
        return greenValaue;
    }
    /* Für bereich 25 - 30 */
    if(*(ptemperatur) >= 25 && *(ptemperatur) < 30){
        greenValaue =  int(-23 * *(ptemperatur)) + 830; //140
        return greenValaue;
    }
    /* Für bereich 30 bis 35 */
    if(*(ptemperatur) >= 30 && *(ptemperatur) < 30){
        greenValaue =  int(-28 * *(ptemperatur)) + 980; //0
        return greenValaue;
    }
    greenValaue =  0;
    return greenValaue;
}

int Led::blue(float *ptemperatur){   
    /* Für Bereich -unendlich - 5C*/
    if(*(ptemperatur) < 5){
        blueValue = 255;
        return blueValue;
    }
    /* Für bereich 5C bis 10C */
    if(*(ptemperatur) >= 5 && *(ptemperatur) < 10){
        blueValue = int(-9.2 * *(ptemperatur)) + 301; //209
        return blueValue;
    }
    /* Für bereich 10C bis 15C */
    if(*(ptemperatur) >= 10 && *(ptemperatur) < 15){
        blueValue = int(-9.6 * *(ptemperatur)) + 294; //160   
        return blueValue;     
    }
    /* Für Bereich 15 bis 20 */
    if(*(ptemperatur) >= 15 && *(ptemperatur) < 20){
        blueValue = int(-29.6 * *(ptemperatur)) + 592; //0
        return blueValue;
    }
    blueValue = 0;
    return blueValue;
}

/* Wettereffekte */

