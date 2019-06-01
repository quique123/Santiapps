//DHT SM
//#1 define input pin in order to read the signal pin from the dht11, process its data
#define DHTPIN 11
#define DHTTYPE DHT11   // DHT 11 

//#2 import dht library (a library is simply a bunch more code that lets you say things in a more natural language in order to perform commands).
#include "DHT.h"

//#3 Define the dht object that comes in the library imported above
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  // This bit allows us to write stuff to a window called Serial Monitor
    Serial.begin(9600); 
  // This bit actually initiates or creates the dht11 object defined above and imported from the library
    dht.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  //This bit creates variables and reads the dht11 object parameters in order to store their values in our variables, repeatedly as the loop() function is intended
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //This bit just checks if the data coming in is not garbage, because if it is, we wont dont anything further with it:
    if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else { //You can also comment after a valid line of code.  So this is what will run if the data coming in is actually valid data...
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
  }
}
