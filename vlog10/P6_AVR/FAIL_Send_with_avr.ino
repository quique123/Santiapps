#include <avr/wdt.h>            // library for default watchdog functions
#include <avr/interrupt.h>      // library for interrupts handling
#include <avr/sleep.h>          // library for sleep
#include <avr/power.h>          // library for power control
#include <util/delay.h>

// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>
#define DHTTYPE DHT11   // DHT 11 
#define DHT11_PIN 6
#define CE_PIN 7
#define CSN_PIN 8

///ADD AVR-1. DEFINE LED PIN FOR COND COMP
#if (defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__))
    #define UNO
    #define LED_PORT        PORTB
    #define LED_PIN         PB5
    char BOARD[]      = {"m328p"};
#endif

int nbr_sleeps=0; 
//2.1 WHAT IS THIS FOR?
volatile int f_wdt=1;

inline void     flash_led()     {LED_PORT |= (1 << LED_PIN); _delay_ms(30);
                                    LED_PORT &= ~(1 << LED_PIN);}

//3. Interrupt Service Routine - disable wdt as soon as comeback
//Some examples say wdt_reset, others wdt_disable and others have if 0 then 1 else wdt overrun...
//I think if you disable it at first in ISR, you call wdt_reset later in code before sleeping and
//that enables it again?
//Otherwise you check for it being 0 and set it to 1 and later in loop if 1, flash led, =0 and sleep
ISR(WDT_vect){
        // disables wdt upon wake up so it wont fire accidentally while we do stuff before sleep again
        wdt_disable();
        //does it get re-enabled by sleep_mode()?  wdt_reset() might be a better idea.
}

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

//6. These must be called each loop
void sleep(){
   set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
   MCUSR &= ~(1<<WDRF); //or MCUSR = 0;
   ADCSRA = 0; //turns off ADC
   power_adc_disable();
   SPCR=0;
   power_spi_disable();
   power_all_disable();
   WDTCSR |= (1<<WDCE) | (1<<WDE); //or WDTCSR = bit (WDCE) | bit (WDE);
   WDTCSR = 1<<WDP0 | 1<<WDP3;// or WDTCSR = bit (WDIE) | bit (WDP3) | bit (WDP0);
   WDTCSR |= _BV(WDIE);
   //flash_led();
   nbr_sleeps ++;
   wdt_reset();
   noInterrupts(); 
   sleep_enable();
   // turn off brown-out enable in software
   MCUCR = bit (BODS) | bit (BODSE);
   MCUCR = bit (BODS); 
   interrupts();             // guarantees next instruction executed
   sleep_mode();
   flash_led();
   sleep_disable();
   power_all_enable();
}

void loop() {
  //ORIGINAL CODE FOR TIMING THE SEND COMMAND
//    //Test condition if time to send....then send...
//    currentMillis = millis();
//    if (currentMillis - prevMillis >= txIntervalMillis) {
//        send();
//        prevMillis = millis();
//    }
    
  //Sleep related code for timing the send command
  flash_led(); //signalling about to sleep
  sleep(); //sleep suff
  if (nbr_sleeps==4) send();
  //8 sec/cycle * 450 cycles = 3600 seconds = 1hr
  //8 * 4 cycles = 32 seconds...
}

//====================

void send() {
  flash_led(); //signal about to measure and report

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
  nbr_sleeps=0;
}
