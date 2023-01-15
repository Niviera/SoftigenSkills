#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string>

class Display
{
private:
    /* data */
    Adafruit_SSD1306 *pDisplayOLED;
    void createOutputRow(char *pstartMsg, char *poutputMsg, int *plength);
public:
    // Funktionen:
    void printTempeture(float *ptemperatur, std::string upupperRow);
    void printTime(int *minutes, int *hours, std::string day);
    void printWeather(std::string weather, std::string description);
    void writeStatusMSG(std::string message);
    bool connectDisplay();
    int displayXCoordinate(int textLNG);
    // Konstruktoren & Destructoren
    Display(/* args */);
    ~Display();
};


