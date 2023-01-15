/*
    Es wird ein TMP36 verwendet!
    Überlegung vielleicht noch versuchen etwas für den DS18B20 Sensor zu schreiben
*/
class Temperature
{
private:
    /* data */
    short int tempPin;
public:
    Temperature(short int pinTemp);
    ~Temperature();
    float readTemperature();
};

