#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <string>
class Network
{
private:
    /* data */
    WiFiManager Connection;
    WiFiClient Client;

    const char *psetupName = "ESP8266_Lucas";
    const char *papi_Key = "d130e80c75f31c607e96dcad97f70618";

    const char *WIFI_NETWORK = "Homesurfing";
    const char *WIFI_PASSWORD = "WRFGJLM6zf7AjrWkoeXfaWwdKYuYuWaR";
    const unsigned int timeoutMS = 20000;

    String city = "Oldenburg";
    int utcOffsettSec = 0;
    
    
public:
    Network(/* args */);
    Network(String city);
    ~Network();
    bool connectToWifi();
    void disconnectFromWifi();
    void setUPWiFi();
    float getCurrentWeatherConditions(std::string *pweatherForecast, std::string *pweatherDescription, int *weatherid);
    std::string getCurrentTime(int *pminutes, int *phours);

};


