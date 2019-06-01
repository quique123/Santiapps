#include <LiquidCrystal.h> // includes the LiquidCrystal Library

int Contrast=20;

//==
#include "DHT.h"

#define DHTPIN 11
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

//==

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

void setup() {
  analogWrite(13,Contrast);
  //==
  dht.begin();
  //==
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
}

void loop() {
  //==
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  //==
  
  lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("Humidity: "); 
  lcd.print(h);
  lcd.print("%\t");
  lcd.setCursor(0,1); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("Temperat: "); 
  lcd.print(t);
  lcd.println("C");
  delay(10);
}
