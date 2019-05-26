int ledpin = 2; //This is a comment.  This defines pin 2 on the board as "ledpin" variable we can use in code for easy reference
int pirpin=3; //This sets pin 3 to the label or variable pirpin
int pinState = LOW; //This creates a variable called pinState and sets it to LOW as initial value
int val = 0; //This creates a variable called val and sets it to 0 as initial value

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //This calls the begin function of the Serial object, which is the object connected to the serial port on the board.  We can now write to and read from it.
  Serial.println("OK"); //This  is a print command to the serial.  When the program execution reaches this stage it will print "OK" to the Serial Monitor.
  delay(5000); //This adds a delay to the program execution.  After booting up, execution will stop completely for 5 seconds, 5000 milliseconds.
}

void loop() {
  val = digitalRead(pirpin); //This reads the pirpin (2) and takes its value (High or Low) and stores it in val
  if (val == HIGH) { //This evaluates the value of val
    digitalWrite(ledpin,HIGH); //Turn LED ON
    if (pinState == LOW) { //This evaluates pinState (which was set to LOW in the beginning)
      Serial.println("HIGH"); //It prints the word HIGH to the SM (Serial Monitor)
      pinState = HIGH; //It updates the pinState to HIGH because we have set it to HIGH
    }
  } else {
    digitalWrite(ledpin,LOW); //Turn LED OFF
    if (pinState == HIGH){ //Check if pinState = HIGH
      Serial.println("LOW"); //Write LOW to the SM
      pinState = LOW; //Update pinState to LOW
    }
  }
} //Repeat loop eternally
