#include <WiFiManager.h>
#include <WiFiUdp.h>


class Network
{
private:
    /* data */
    WiFiManager Connection;
    WiFiClient Client;

    const char *psetupName = "ESP8266_Lucas";
    const char *papi_Key = "d130e80c75f31c607e96dcad97f70618";
    String city = "Oldenburg";
    int utcOffsettSec = 0;
    
public:
    Network(/* args */);
    Network(String city);
    ~Network();
    void setUPWiFi();
    float getCurrentWeatherConditions(std::string *pweatherForecast, std::string *pweatherDescription);
    std::string getCurrentTime(int *pminutes, int *phours);

};


