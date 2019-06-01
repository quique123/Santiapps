#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(10,11); //Rx, Tx method signature
Servo myServo;

void setup() {
  // put your setup code here, to run once:
  delay(1500); // wait for bluetooth module to start
  bluetooth.begin(9600); 
  Serial.begin(9600);//1-Bluetooth default baud is 115200
  myServo.attach(7);
  Serial.println("Ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(bluetooth.available()>0) {
    Serial.println("Waiting for data");
    int c = bluetooth.read();
    //char c = bluetooth.read();
    bluetooth.println(c);
    if(c=='1'){
      bluetooth.println("Open Wide");
      Serial.println("1");
      myServo.write(1000);
    } else if(c=='2'){
      bluetooth.println("Close");
      Serial.println("2");
      myServo.write(2000);
    }
//    if(c=='\n') { //if data is a 'newline' then set the cmd[] element to 0
//       myServo.write(12);
//       Serial.print("Servo val ");
//       Serial.println(12);
//    } else {      //if data is real data, then set the cmd[] element to that data!
//      bluetooth.println("not valid");
//    }
  }  
}
