#include "WiFi.h"
#include "conn.h"

connection::connection(/* args */){

    }
connection::~connection(){

}

bool connection::connectToWifi(){
    /*
        Stellt die Verbindung zum Wlan mit dem, integrierten Wlan modul, her.
        Sollte es nicht innerhalb der Zeit X klappen wird es als fehlgeschlagen angesehen und ein false returned,
        ansonsten true und alle nötigen Daten aka IP-Adresse  und MAC-Adresse werden auf dem SerialPort geschrieben.
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
        Serial.println("Verbindung fehlgeschlagen.");
        return false;

    }else{
        Serial.println("Verbindung erfolgreich.");
        Serial.print("Netzwerk: ");
        Serial.println(WIFI_NETWORK);
        Serial.print("IPv4-Adresse: ");
        Serial.println(WiFi.localIP());
        Serial.print("MAC-Adresse: ");
        Serial.println(WiFi.macAddress());
        return true;
    }
    

}