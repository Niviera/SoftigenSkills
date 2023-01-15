#include <Arduino.h>
#include <Wire.h>
#include <conn.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SDA 4
#define SCL 0

// Globale Variablen:
connection con;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("\nHello");

  if(!con.connectToWifi()){
    // eine fehler behandlung überlegen!
    // maybe nen resett?
  }
  //Wire.begin(SDA, SCL);
  

  /* Display verbindung */
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)){
    Serial.println("Kein Display gefunden");
  }else{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Hey");
    display.display();
  }

  
}

void loop() {
  // put your main code here, to run repeatedly:
}