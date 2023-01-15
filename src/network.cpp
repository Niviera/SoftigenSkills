#include <Arduino.h>
#include "network.h"
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

//Funktionen
void Network::setUPWiFi(){
    Connection.autoConnect(psetupName);
}

std::string Network::getCurrentTime(int *pminutes, int *phours){
    /* Messages for debugging */
    Serial.println("Connect to 'pool.ntp.org' to get the Time");
    std::string days[] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag", };
    /* 
        get Time
        Berlin = UTC+1
        Offsett = +1 * 60 * 60 = 3600sec
    */
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsettSec);
    timeClient.begin();
    timeClient.update();
    int day = timeClient.getDay();
    *(pminutes) = timeClient.getMinutes();
    *(phours) = timeClient.getHours();
    timeClient.end();

    return days[day];
}

float Network::getCurrentWeatherConditions(std::string *pweatherForecast, std::string *pweatherDescription){
    /* Messages for debugging */
    Serial.print("connecting to "); 
    Serial.println("api.openweathermap.org");

if (Client.connect("api.openweathermap.org", 80)) {
    Client.println("GET /data/2.5/weather?q=" + city + ",DE&units=metric&lang=de&APPID=" + papi_Key);
    Client.println("Host: api.openweathermap.org");
    Client.println("Connection: close");
    Client.println();
} else {
    Serial.println("connection failed");
    *(pweatherForecast) = "FetchFail";
    return -99.99;
}

    /* Calc with https://arduinojson.org/v6/assistant/#/step1 */
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, Client);
  Client.stop();
  /* Nehme den UTC offsett der Stadt */
  utcOffsettSec = doc["timezone"];
  
  /* TODO Aufwendiges auswerten nochmal drüberschauen geht bestimmt geschickter */
  std::string temp = doc["weather"][0]["description"];
  int index = 0;
  while(temp[index] != ' ' && temp[index] != '\0'){
    index++;
  }
  temp[0] += 0x20;
  temp[index] = '\0';
  *(pweatherDescription) = temp;

  /* Auserten  der weather id */
  int weatherID = doc["weather"][0]["id"];
  switch ((weatherID/100)) {                                                
    case 3: 
        *(pweatherForecast) = "Nieselreg."; 
        break;
    case 5: 
        *(pweatherForecast) = "Regen"; 
        break;
    case 6: 
        *(pweatherForecast) = "Schnee"; 
        break;
    case 7: 
        *(pweatherForecast) = "Nebel"; 
        break;
    case 8: 
        *(pweatherForecast) = "Wolken"; 
        break;
    default: 
        *(pweatherForecast) = "Unbekannt"; 
        break;                              
  } if (weatherID == 800) *(pweatherForecast) = "  klar"; 
  
  return (float)doc["main"]["temp"];
}

// Konstruktoren / Destructoren
Network::Network(/* args */)
{
}

Network::Network(String city){
    this->city = city;
}

Network::~Network()
{
}