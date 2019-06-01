#include <LiquidCrystal.h> // includes the LiquidCrystal Library

int Contrast = 20;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

void setup() {
  analogWrite(13,Contrast);
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
}

void loop() {
  lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("SUBSCRIBE to "); // Prints string "Distance" on the LCD
  delay(10);
  lcd.setCursor(0,1);
  lcd.print("SANTIAPPS!");
//  lcd.print("Millis: ");
//  lcd.print(millis());
  delay(10);
}
