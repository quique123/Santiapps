int lightValue = 0;
int rainValue = 0;
int humValue = 0;
int rain;
int light;
int humidity;
int solePin = 7;
int sensorhumidity = A1;
int sensorlight = A2;
int sensorrain = A0;

void setup() {
  pinMode(solePin, OUTPUT);
  Serial.begin(9600); 
}

void loop() {
  lightValue = analogRead(sensorlight);
  rainValue = analogRead(sensorrain);
  humValue = analogRead(sensorhumidity);
  delay(1500);

  rain = (100 * rainValue) / 1024;
  Serial.print("rain:");
  Serial.print(rain);
  Serial.println(" C");
  light = (100.0 * lightValue) / 1024;
  Serial.print("light:");
  Serial.print(light);
  Serial.println("%");
  humidity = (100.0 * humValue) / 1024;
  Serial.print("humidity:");
  Serial.println(humidity);
  Serial.println("=======================================================================");
  // ----------------------------------------------------
  // Chequeo si debo regar
  // ----------------------------------------------------
  if (rain < -10) {
        Serial.println("Rain is NOT practically, MUST ADD TIMER...");
    if( humidity > 70 && light > 70 ) {
      digitalWrite(solePin, HIGH);
      Serial.println("HUM is LOW and LIGHT is LOW therefore Debe regar");
    } else {
      digitalWrite(solePin, LOW);
    }
    
  } else {
    digitalWrite(solePin, LOW);
    Serial.println("rain is LOW so NO debe regar");
  }
  
  Serial.println("=======================================================================");

}
