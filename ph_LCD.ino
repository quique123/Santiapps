#include <LiquidCrystal.h> // includes the LiquidCrystal Library

int Contrast = 20;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
void setup() {
 Serial.begin(9600);
 Serial.println("Hi");

 analogWrite(13,Contrast);
 lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
}
 
void loop() {
 for(int i=0;i<10;i++) { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++){
  for(int j=i+1;j<10;j++){
   if(buf[i]>buf[j]){
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + 21.34;
 //Serial.print("sensor = ");
 Serial.println(phValue);

 lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
 lcd.print("Kombucha pH "); // Prints string "Distance" on the LCD
 lcd.print(phValue); // Prints string "Distance" on the LCD
 lcd.setCursor(0,1); // Sets the location at which subsequent text written to the LCD will be displayed
 lcd.print("IPA Beer pH:"); // Prints string "Distance" on the LCD
 lcd.print(phValue+2); // Prints string "Distance" on the LCD

 delay(20);
}
