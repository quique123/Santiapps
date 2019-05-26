#include <Stepper.h>
const int stepsPerRevolution = 200; 
unsigned long previousMillis = 0;
long interval = 5000;

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  myStepper.setSpeed(70);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis(); //para uso futuro

  for (int i=0; i<10; i++) {
    Serial.println("negative");
    Serial.println(i);
     myStepper.step(stepsPerRevolution);
  }
  
  delay(2000);

  for (int a=0; a<10; a++) {
    Serial.println("++++");
    Serial.println(a);
     myStepper.step(-stepsPerRevolution);
  }
  delay(2000);

}
