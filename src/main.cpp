#include <Arduino.h>
#include <Wire.h>
#include "temperature.h"
#include "display.h"
#include "network.h"
#include "led.h"

#define STADT "Oldenburg"
#define MAXSTATUSINFORMATIONS 3
#define TEMPSENSOR_PIN 0
#define LED_GREEN_PIN 12
#define LED_RED_PIN 13
#define LED_BLUE_PIN 14

/* Globale Variablen */
Display OledDisplay;
Network Connection(STADT);
Led Light(LED_GREEN_PIN, LED_BLUE_PIN, LED_RED_PIN);
Temperature Temperatur(TEMPSENSOR_PIN);

unsigned long lastTimeCheck = 0;
unsigned long lastTimeWeatherChecked = 0;
unsigned long lastTimeSwitchedInformation = 0;
int8_t oledInfo = 0;

/* Variables for Time */
int minutes = 0;
int hours = 0;
bool newDay = false;
std::string day = "";

/* Variables for Temp */
float temperatur = 0;
std::string weatherCondition = "";
std::string weatherDiscription = "hey";


void calcTime();

void setup() {
  Serial.begin(9600);
  /* Verbinde mit potenzielles OLED pannel */
  if(!OledDisplay.connectDisplay()) {
    Serial.println("Kein Display gefunden");
  }
  OledDisplay.writeStatusMSG("Verbinde  mit WLAN");
  /* Verbinde mit Wlan */
  Connection.setUPWiFi();
    
  /* Input Pins: */
  pinMode (TEMPSENSOR_PIN, INPUT);
  /* Output Pins: */
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);


  temperatur = Connection.getCurrentWeatherConditions(&weatherCondition, &weatherDiscription);
  day = Connection.getCurrentTime(&minutes,&hours);
  OledDisplay.printWeather(weatherCondition, weatherDiscription);
  lastTimeWeatherChecked = millis();
}

void loop() {
  /* go into if condition after every 30min */
  if (millis() - lastTimeWeatherChecked >= 1800000) {          
    temperatur = Connection.getCurrentWeatherConditions(&weatherCondition, &weatherDiscription);
    lastTimeWeatherChecked = millis();                           
    OledDisplay.printTempeture(&temperatur, weatherCondition);
  }
  /* Alle 5min die Infotauschen */
  if(millis() - lastTimeSwitchedInformation >= 60000){
    lastTimeSwitchedInformation = millis();
    switch (oledInfo)
    {
    case 0:
      oledInfo++;
      Serial.printf("\nStadt & Temperatur:");
      OledDisplay.printTempeture(&temperatur, STADT);
      break;
    case 1:
      oledInfo++;
      Serial.printf("\nUhr und Tag");
      calcTime();
      OledDisplay.printTime(&minutes,&hours,day);
      break;
    case 2:
      oledInfo++;
      Serial.printf("\nWetter:");
      OledDisplay.printWeather(weatherCondition, weatherDiscription);
      
      oledInfo = oledInfo % MAXSTATUSINFORMATIONS;     
    }
  }
  
}

void calcTime(){
  /* Uhrzeit ist nicht perfekt diese wird immer hinter her hängen!*/
  Serial.printf("\nold time: %02d:%02d", hours, minutes);
  minutes += ((millis() - lastTimeCheck)/60000);
  if(minutes >= 60){ 
    minutes = minutes % 60;
    hours++;
  }
  Serial.printf("\nnew time: %02d:%02d", hours, minutes);
  hours = hours % 24;
  if(hours == 0 && newDay){
    day = Connection.getCurrentTime(&minutes,&hours);
    newDay = false;
  }
  if(hours >= 1){
    newDay = true;
  }
  lastTimeCheck = millis();
}



