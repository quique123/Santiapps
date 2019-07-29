

// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>
#define DHTTYPE DHT11   // DHT 11 
#define DHT11_PIN 6
#define CE_PIN 7
#define CSN_PIN 8

const byte slaveAddress[5] = {'R','x','A','A','A'};

DHT dht(DHT11_PIN, DHTTYPE);

struct package
{
  float humedad = 0;
  float temperatura = 0;
};
typedef struct package Package;
Package data;

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second


void setup() {

    Serial.begin(9600);
    pinMode(10,OUTPUT);
    Serial.println("SimpleTx Starting");
    //Initialize the DHT sensor
    dht.begin(); 
  
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);
}

//====================

void loop() {
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        prevMillis = millis();
    }
}

//====================

void send() {

  //take measurement
  data.humedad = dht.readHumidity();
  data.temperatura = dht.readTemperature();
      
    bool rslt;
    rslt = radio.write( &data, sizeof(data) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    Serial.print("data.humedad");
    if (rslt) {
        Serial.println("  Acknowledge received");
        updateMessage();
    }
    else {
        Serial.println("  Tx failed");
    }
}

//================

void updateMessage() {

}
