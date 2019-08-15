/////////////////////////////////////////////////////////////////////////////////////////////////////FROM WORKING RECEIVE
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////AAAAAAAAAAAAA


#include <SoftwareSerial.h>
#define RX 10
#define TX 11
String AP = "WIFI_NAME";       // CHANGE ME
String PASS = "WIFI_PASSWORD"; // CHANGE ME
String API = "YOUR_API_KEY";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266Radio(RX,TX); 

/////////////////////////////////////////////////////////////////////////////////////////////////////FROM WORKING RECEIVE
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
/////////////////////////////////////////////////////////////////////////////////////////////////////AAAAAAAAAAAAA

//Must move code to a wifi-startup()<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Will run everytime mcu wakes up
void setup() {
  Serial.begin(9600);
  
/////////////////////////////////////////////////////////////////////////////////////////////////////FROM WORKING RECEIVE
  //setup radio receive stuff
  setupRadio();
/////////////////////////////////////////////////////////////////////////////////////////////////////AAAAAAAAAAAAA
  //setup esp for posting to cloud
  esp8266Radio.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
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
/////////////////////////////////////////////////////////////////////////////////////////////////////FROM WORKING RECEIVE
    //get data from radio first...
    getData();
    showData();
/////////////////////////////////////////////////////////////////////////////////////////////////////AAAAAAAAAAAAA
}

void sendCommand(String command, int maxTime, char readReplay[]) {//<<<<<<<<<<<<<<<<<<<helper f(x)
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266Radio.println(command);//at+cipsend
    if(esp8266Radio.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
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
 String getData = "GET /update?api_key="+ API +"&"+ field1 +"="+String(dataReceived.humedad)+"&"+ field2 +"="+String(dataReceived.temperatura);
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266Radio.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}
