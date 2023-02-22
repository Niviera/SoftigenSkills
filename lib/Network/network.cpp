#include <Arduino.h>
#include "network.h"
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>


//Funktionen
void Network::setUPWiFi(){
    Connection.autoConnect(psetupName);
}

bool Network::connectToWifi(){
    /*
        Stellt die Verbindung zum Wlan mit dem, integrierten Wlan modul, her.
        Sollte es nicht innerhalb der Zeit X klappen wird es als fehlgeschlagen angesehen und ein false returned,
        ansonsten true.
    */
    Serial.println("Versuche mit "+ String(WIFI_NETWORK) + " zu Verbinden");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

    unsigned int startMessure = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startMessure < timeoutMS )
    {
        Serial.print(".");
        delay(100);
    }

    if(WiFi.status() != WL_CONNECTED){
        Serial.printf("\nVerbindung fehlgeschlagen.");
        return false;

    }else{
        Serial.printf("\nVerbindung erfolgreich.");
        return true;
    }
}

void Network::disconnectFromWifi(){
    WiFi.disconnect();
}

std::string Network::getCurrentTime(int *pminutes, int *phours){
    
    /* Messages for debugging */
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

float Network::getCurrentWeatherConditions(std::string *pweatherForecast, std::string *pweatherDescription, int *weatherid){
    /* Messages for debugging */
if (Client.connect("api.openweathermap.org", 80)) {
    //Client.println("GET /data/2.5/weather?q=" + city + ",DE&units=metric&lang=en&APPID=" + papi_Key);
    //Client.println("Host: api.openweathermap.org");
    //Client.println("Connection: close");
    //Client.println();
} else {
    //Serial.println("connection failed");
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
  temp[index] = '\0';
  *(pweatherDescription) = temp;

  /* Auserten  der weather id */
  int weatherID = doc["weather"][0]["id"];
  switch ((weatherID/100)) {  
    case 2:
        *(pweatherForecast) = "Thunder";
        *(weatherid) = 2;                                              
    case 3: 
        *(pweatherForecast) = "Drizzle";//"Nieselreg."; 
        *(weatherid) = 3;
        break;
    case 5: 
        *(pweatherForecast) = "Rain";//"Regen"; 
        *(weatherid) = 5;
        break;
    case 6: 
        *(pweatherForecast) = "Snow";//"Schnee"; 
        *(weatherid) = 6;
        break;
    case 7: 
        *(pweatherForecast) = "Fog";//"Nebel"; 
        *(weatherid) = 7;
        break;
    case 8: 
        *(pweatherForecast) = "Clouds";//"Wolken"; 
        *(weatherid) = 8;
        break;
    default: 
        *(pweatherForecast) = "Unknown";//"Unbekannt"; 
        *(weatherid) = 800;
        break;                              
  } 
  if (weatherID == 800){
        *(weatherid) = 800;
        *(pweatherForecast) = "Clear";//"Klar"; 
  } 
  
  
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