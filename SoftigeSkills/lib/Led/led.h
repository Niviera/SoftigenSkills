class Led
{
private:
    /* data */
    int redValue = 0;
    int greenValaue = 0;
    int blueValue = 0;
    int bluePin = 0;
    int redPin = 0;
    int greenPin = 0;

    int NUM_LEDS = 0;

public:
    Led(/* args */);
    Led(int green, int blue, int red, int numLeds);
    ~Led();
    int red(float *ptemperatur);
    int green(float *ptemperatur);
    int blue(float *ptemperatur);

};


