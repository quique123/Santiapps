#include <avr/sleep.h>
#include <avr/wdt.h>
//ADDED FOR AVR RESET

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//UNO 0 - ESP
//UNO 1 - ESP
//UNO 7 - NRF-CE
//UNO 8 - NRF-CSN
//UNO 9 -
//UNO10 -
//UNO11 - NRF-MOSI
//UNO12 - NRF-MISO
//UNO13 - NRF-SCK
//UNOGND- ESP-GND
//UNO3.3- RAIL - NRF-3.3V - ESP3.3V
//UNO5V - LCD-Vcc
//UNOGND- NRF-GND
//UNOGND- LCD-GND
//UNO A4- LCD-SDA
//UNO A5- LCD-SCL

#include "ThingSpeak.h"
#include "WiFiEsp.h"
#include "secrets.h"

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiEspClient  client;

#define CE_PIN 7
#define CSN_PIN 8
const byte thisSlaveAddress[5] = {'R','x','A','A','A'};
RF24 radio(CE_PIN, CSN_PIN);
struct package{
  float humedad = 0;
  float temperatura = 0;
};
typedef struct package Package;
Package dataReceived;
bool newData = false;

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2,3); // RX, TX
#define ESP_BAUDRATE  19200
#else
#define ESP_BAUDRATE  115200
#endif

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
String myStatus = "";

void setup() {
  //Initialize serial and wait for port to open
  Serial.begin(115200);  // Initialize serial
  
  //NRF
  setupRadio();

  // sleep for a total of 240 seconds (8x225=240)
  int i;
  for (i = 0; i <225; i++){  
    myWatchdogEnable (0b100001);  // 8 seconds
  }
}

void setupRadio(){
  pinMode(10,OUTPUT);
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();

  lcd.init();
  lcd.init();
  lcd.backlight();
}

void loop() {
    getData();
    showData();
}

void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
}

void showData() {
    if (newData == true) {
        Serial.print("Data received ");
        Serial.println(dataReceived.humedad);
        Serial.println(dataReceived.temperatura);
        lcd.setCursor(1,0);
        lcd.print(dataReceived.humedad);
        lcd.setCursor(1,1);
        lcd.print(dataReceived.temperatura);
        newData = false;
        postDataCloud();
    }
}

void postDataCloud(){
  //setup esp wifi stuff
  // initialize serial for ESP module  
  setEspBaudRate(ESP_BAUDRATE);
  
  while (!Serial) {
    ; 
  }

  Serial.print("Searching for ESP8266..."); 
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  Serial.println("found it!");
   
  ThingSpeak.begin(client);  // Initialize ThingSpeak



  
 // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, dataReceived.humedad);
  ThingSpeak.setField(2, dataReceived.temperatura);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }  
}

void setEspBaudRate(unsigned long baudrate){
  long rates[6] = {115200,74880,57600,38400,19200,9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for(int i = 0; i < 6; i++){
    Serial1.begin(rates[i]);
    delay(100);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(100);  
  }
  Serial1.begin(baudrate);
}
ISR(WDT_vect) {
  wdt_disable();  // disable watchdog
}
void myWatchdogEnable(const byte interval) {  
  MCUSR = 0;                          // reset various flags
  WDTCSR |= 0b00011000;               // see docs, set WDCE, WDE
  WDTCSR =  0b01000000 | interval;    // set WDIE, and appropriate delay
  wdt_reset();
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
  sleep_mode();            // now goes to Sleep and waits for the interrupt
} 
