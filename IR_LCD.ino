#include <IRremote.h>

#include <LiquidCrystal.h> // includes the LiquidCrystal Library

int Contrast = 20;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  analogWrite(13,Contrast);
  irrecv.enableIRIn(); // Start the receiver
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
}

void loop() {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.clear();
    lcd.print("YT/Santiapps: "); // Prints string "Distance" on the LCD
    lcd.setCursor(0,1);
    lcd.print("IR Code:");
    lcd.print(results.value);
    irrecv.resume(); // Receive the next value
  }

  delay(100);
}
