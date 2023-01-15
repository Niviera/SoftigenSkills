#include "led.h"

// konstruktor & destructor
led::led(/* args */)
{
}

led::~led()
{
}

int led::red(float temp){
    
    /* Für Bereich -unendlich - 5C*/
    if(temp < -5){
        return 25;
    }
    if(temp >= -5 && temp < 0){
        return int(-5 * temp);
    }
    if(temp >= 0 && temp < 5){
        return 0;
    }

    /* Für bereich 5C - 10C */
    if(temp >= 5 && temp < 10){
        return 0;
    }

    /* Für bereich 10C - 15C */
    if(temp >= 10 && temp < 15){
        return int(10.8 * temp) - 44;   //118
    }

    /* Für Bereich 15 bis 20 */
    if(temp >= 15 && temp < 20){
        //return int(-8 * temp) + 238; //78 
        return int(1.2 * temp) + 100; //124   
    }

    /* Für bereich 20 bis 25 */
    if(temp >= 20 && temp < 25){
        return int(-24.8 * temp) + 620; //0
    }

    /* Für bereich 25 bis 30 */
    if(temp >= 25 && temp < 30){
        return int(51 * temp) - 1275; //255
    }

    /* für bereich 30 - unendlich*/
    if(temp >= 30){
        return 255;
    }

    return 0;
}

int led::green(float temp){
    /* Für Bereich -unendlich - 5C*/
    if(temp < -5){
        return 204;
    }
    if(temp >= -5 && temp < 0){
        return int(-30.6 * temp) + 51;
    }
    if(temp >=0 && temp < 5){
        return 51;
    }

    /* Für Bereich 5C bis 10C */
    if(temp >= 5 && temp < 10){
        return int(31 * temp) - 104;
        //206
    }

    /* Für Bereich 10C Bis 15C */
    if(temp >= 10 && temp < 15){
        return int(9.8 * temp) + 108;   //255        
    }

    /* Für Bereich 15 bis 20 */
    if(temp >= 15 && temp < 20){
        //return int(-3.4 * temp) + 306; //238
        return int(-0.6 * temp) + 264; //252
    }

    /* Für bereich 20 - 25 */
    if(temp >= 20 && temp <25){
        return 255;
    }

    /* Für bereich 25 - 30 */
    if(temp >= 25 && temp < 30){
        return int(-23 * temp) + 830; //140
    }

    /* Für bereich 30 bis 35 */
    if(temp >= 30 && temp < 30){
        return int(-28 * temp) + 980; //0
    }

    return 0;
}

int led::blue(float temp){   
    /* Für Bereich -unendlich - 5C*/
    if(temp < 5){
        return 255;
    }

    /* Für bereich 5C bis 10C */
    if(temp >= 5 && temp < 10){
        return int(-9.2 * temp) + 301;
        //209
    }

    /* Für bereich 10C - 15C */
    if(temp >= 10 && temp < 15){
        return int(-9.6 * temp) + 294; //160
        
    }

    /* Für Bereich 15 bis 20 */
    if(temp >= 15 && temp < 20){
        //return int(-2.4 * temp) + 196; //148
        return int(-29.6 * temp) + 592; //0
    }

    /* Für bereich 20 bis 25 */
    if(temp >= 20 && temp < 25){
        return 0;
    }

    /* Für bereich 25 bis 30 */
    if(temp >= 25 && temp < 30){
        return 0;
    }

    /* Für bereich 30 bis 35 */
    if(temp >= 30){
        return 0;
    }
    return 0;
}