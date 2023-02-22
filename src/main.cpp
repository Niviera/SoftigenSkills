#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>
#include "temperature.h"
#include "display.h"
#include "network.h"
#include "led.h"
#include "globalDefine.h"

#define STADT "Oldenburg"
#define MAXSTATUSINFORMATIONS 4
#define TEMPSENSOR_PIN 0
#define LED_GREEN_PIN 12
#define LED_RED_PIN 13
#define LED_BLUE_PIN 14

#define LED_PIN_STRIP 15 
#define NUM_LEDS    2   
#define BRIGHTNESS  70
#define LED_TYPE    WS2811


/* Globale Variablen */
Display OledDisplay;
Network Connection(STADT);
Led Light(LED_GREEN_PIN, LED_BLUE_PIN, LED_RED_PIN, NUM_LEDS);
Temperature Temperatur(TEMPSENSOR_PIN);

/* Für fast LED*/
CRGB effectLED[NUM_LEDS];
typedef void(Led::*LedLightEffect)();   // vorbereitung um die lichteffecte in die Klasse Led auszulagern
void(*fptr)();
int weatherid;
bool newFunction = true;

/* Handle OledDisplay time */
unsigned long lastTimeCheck = 0;
unsigned long lastTimeWeatherChecked = 0;
unsigned long lastTimeSwitchedInformation = 0;
unsigned long lastRGBLedSwitched = 0;
int8_t oledInfo = 0;

/* Variables for Time */
int minutes = 0;
int hours = 0;
bool newDay = false;
std::string day = "";

/* Variables for Temp */
float roomtemp = 0;
float temperatur = 0;
std::string weatherCondition = "";
std::string weatherDiscription = "hey";


void calcTime();
void effect_thunder();     
void effect_rain();
void effect_fog();
void effect_snow();
void effect_cloudy();
void effect_clear_sky();
void effect_drizzle();

void setup() {
  Serial.begin(9600);
  /* Fast LED */
  FastLED.addLeds<WS2812B, LED_PIN_STRIP, GRB>(effectLED, NUM_LEDS); // LED instanziieren
  for (int i = 0; i < NUM_LEDS; i++){
    effectLED[i] = CRGB::Black;
  }
  FastLED.show();
  effectLED[0] = CRGB::Red;                             
  FastLED.setBrightness(255);
  FastLED.show();

  /* Verbinde mit potenzielles OLED pannel */
  if(!OledDisplay.connectDisplay()) {
    debugln("Kein Display gefunden");
  }
  
  /* Verbinde mit Wlan */
  OledDisplay.writeStatusMSG("Verbinde  mit WLAN");
  Connection.connectToWifi();
  
  /* Hole initial Daten aus dem Netz */  
  temperatur = Connection.getCurrentWeatherConditions(&weatherCondition, &weatherDiscription, &weatherid);
  day = Connection.getCurrentTime(&minutes,&hours);
  OledDisplay.printWeather(weatherCondition, weatherDiscription);
  lastTimeWeatherChecked = -3600000;
  Connection.disconnectFromWifi();

  /* Input Pins: */
  pinMode (TEMPSENSOR_PIN, INPUT);
  /* Output Pins: */
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);

  /* Raumtemp LED */
  roomtemp = Temperatur.readTemperature();
  analogWrite(LED_BLUE_PIN, Light.blue(&roomtemp));
  analogWrite(LED_GREEN_PIN, Light.green(&roomtemp));
  analogWrite(LED_RED_PIN, Light.red(&roomtemp));
}

void loop() {
  /* get weather Data every 30min and set corret led effect function */
  if (millis() - lastTimeWeatherChecked >= 1800000) {  
    /* Hole aktuelle Wetterdaten */
    Connection.connectToWifi();        
    temperatur = Connection.getCurrentWeatherConditions(&weatherCondition, &weatherDiscription, &weatherid);
    lastTimeWeatherChecked = millis();                           
    OledDisplay.printTempeture(&temperatur, weatherCondition);
    Connection.disconnectFromWifi();
    newFunction = true;
  }
  /* Change every minute the display information */
  if(millis() - lastTimeSwitchedInformation >= 60000){
    lastTimeSwitchedInformation = millis();
    switch (oledInfo)
    {
    case 0:
      oledInfo++;
      debugln("Stadt & Temperatur:");
      OledDisplay.printTempeture(&temperatur, STADT);
      break;
    case 1:
      oledInfo++;
      debugln("Uhr und Tag");
      calcTime();
      OledDisplay.printTime(&minutes,&hours,day);
      break;
    case 2:
    oledInfo++;
    debugln("Raumtemp");
    roomtemp = Temperatur.readTemperature();
    OledDisplay.printTempeture(&roomtemp, "Roomtemp.:");
    analogWrite(LED_BLUE_PIN, Light.blue(&roomtemp));
    analogWrite(LED_GREEN_PIN, Light.green(&roomtemp));
    analogWrite(LED_RED_PIN, Light.red(&roomtemp));
    break;
    case 3:
      oledInfo++;
      debugln("Wetter:");
      OledDisplay.printWeather(weatherCondition, weatherDiscription);
      
      oledInfo = oledInfo % MAXSTATUSINFORMATIONS;         
    }
  }
  /* Abfrage ob etwas auf dem Serial Port liegt */
  if(Serial.available()){
    weatherid = Serial.parseInt();    // vorsicht es werden negative Zahlen nicht berücksichtig!
    newFunction = true;
    String resett = Serial.readString(); // einfacher schlichter resett um alles auzlusen und zu verwerfen.
  }

  /* Auswertung welche LED-Funktion ausgeführt wird */
  if(newFunction){
    newFunction = false;
    switch (weatherid) {  
      case 2:
        fptr = &effect_thunder;    
        break;                                        
      case 3: 
        fptr = &effect_drizzle;
        break;
      case 5: 
        fptr = &effect_rain;
        break;
      case 6: 
        fptr = &effect_snow;
        break;
      case 7: 
        fptr = &effect_fog;
        break;
      case 8: 
        fptr = &effect_cloudy;
        break;                          
    } 
    if (weatherid == 800){
        fptr = &effect_clear_sky;
    } 

  }

  /* rufe effect funtion auf */
   fptr();
}

void calcTime(){
  /* Uhrzeit ist nicht perfekt diese wird immer hinter her hängen!*/
  minutes += ((millis() - lastTimeCheck)/60000);
  if(minutes >= 60){ 
    minutes = minutes % 60;
    hours++;
  }
  hours = hours % 24;
  if(hours == 0 && newDay){
    Connection.connectToWifi();
    day = Connection.getCurrentTime(&minutes,&hours);
    Connection.disconnectFromWifi();
    newDay = false;
  }
  if(hours >= 1){
    newDay = true;
  }
  lastTimeCheck = millis();
}

void effect_thunder() {
    FastLED.setBrightness(255);
    int activeLed;
  
  /* Blitze */
  for(int i = 5; i >= 0; i--){
    for(int x = 0; x < NUM_LEDS; x++){
      activeLed = rand() % 101;
      if(activeLed >= 65){
        effectLED[x] = CRGB::White;
      }else{
        effectLED[x] = CRGB::Black;
      }
    }
    FastLED.show();
    delay(400);
  }

  /* Alle Leds schwarz */
  for(int x = 0; x < NUM_LEDS; x++){
    effectLED[x] = CRGB::Black;
  }
  FastLED.show();
  delay(250);

  /* Donner */
  for(int i = 0; i < 2; i++){
    for(int x = 0; x < NUM_LEDS; x++){
      effectLED[x] = CRGB::White;
    }
    FastLED.show();
    delay(250);
    for(int x = 0; x < NUM_LEDS; x++){
      effectLED[x] = CRGB::Black;
    }
    FastLED.show();
    delay(250);
  }
}

void effect_rain(){
  debugln("Licht effect_rain");
  int active = 0;
  int rain = 50;
  for(int i = 0; i < 5; i++){
    for(int x = 0; x < NUM_LEDS; x++){
      active = rand() % 101;
      if(active >= rain){
        effectLED[x] = CRGB::BlueViolet;
      }else{
         effectLED[x] = CRGB::DarkSlateGrey; 
      }
    }
    FastLED.show();
    delay(500);
  }
}

void effect_fog(){
  /* 
    Dieser LED effect soll erzielen das es Nebelig ist und es zwischendurch einen leuchtturn durchbrochen wird.
  */
  int ligthTower;
  int activeLed = 0;
  int chance = 50;
  for(int i = 0; i < 10; i++){
    /* Alle Leds grau */
    for(int x = 0; x < NUM_LEDS; x++){
      effectLED[x] = CRGB::DarkSlateGrey;
    }
    FastLED.show();
    delay(500);
    
    /* Leuchturm effect */
    ligthTower = rand() % 101;
    if(ligthTower >= chance){
      chance += 9;
      for(int x = 0; x < NUM_LEDS; x++){
        if(x == activeLed){
          activeLed++;
          effectLED[x] = CRGB::White;
        }else{
          effectLED[x] = CRGB::DarkSlateGrey;
        }
        FastLED.show();
        delay(250);
      }
      activeLed = 0;
    }
  }
}

void effect_snow(){
  /*
    Dieser LED effect soll ein schnee effect erzielen jede active weiße LED reduziert die wahrscheinlichkeit das
    eine weitere an ist.
  */
  int snow = 0;
  int drizzle
   = 40;
  for(int i = 0; i < 10; i++){
    for(int x = 0; x < NUM_LEDS; x++){
      snow = rand() % 101;
      if(snow >= drizzle){
        drizzle
         += 5;
        effectLED[x] = CRGB::GhostWhite;
      }else{
         effectLED[x] = CRGB::Black; 
      }
    }
    FastLED.show();
    delay(500);
  }
}

void effect_cloudy(){
  /*
    Diese funtkion bildet wolken nach mit gelegentlichen lichtbrüchen
  */
  int active = 0;
  int lightBreak = 90;
  for(int i = 0; i < 5; i++){
    for(int x = 0; x < NUM_LEDS; x++){
      active = rand() % 101;
      if(active >= lightBreak){
        effectLED[x] = CRGB::White;
      }else{
        effectLED[x] = CRGB::DarkGrey;
      }
    }
    FastLED.show();
    delay(200);
  }
  
}

void effect_clear_sky(){
  /*
    Dieser effect soll eine heller werdende Sonne darstellen
  */
  for(int i = 0; i < NUM_LEDS-1; i++){
      effectLED[i] = CRGB::Black;
  } 
    effectLED[NUM_LEDS-1] = CRGB::Orange;
  for(int i = 0; i <= 255;i++){
    FastLED.setBrightness(i);
    FastLED.show();
    delay(100);
  }
}

void effect_drizzle(){
  /*
    Dieser LED effect soll ein regen effect erzielen jede active weiße LED.
  */
  int rain = 0;
  int drizzle = 40;
  for(int i = 0; i < 10; i++){
    for(int x = 0; x < NUM_LEDS; x++){
      rain = rand() % 101;
      if(rain >= drizzle){
        effectLED[x] = CRGB::BlueViolet;
      }else{
         effectLED[x] = CRGB::Blue; 
      }
    }
    FastLED.show();
    delay(500);
  }
}