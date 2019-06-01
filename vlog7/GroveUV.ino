
float uvindex;                        // Store UV - UV Sensor


void setup() {
  Serial.begin(9600);
  delay(3000);
}

void loop() {
  // Call both methods to see the difference
  tzviForum();
  delay(3000);
  SampleUVAlarm();
  delay(3000);
  grovewiki();
  delay(3000);
}

void tzviForum (){
  int val=0;
  val=analogRead(A0); //reading the digital signal 0-1023
  float mvolts = 0;
  mvolts = val*4.9; // 5 volts / 1024 units or, .0049 volts (4.9 mV) per unit   
  float illum = 0;
  illum = mvolts/1000*307; //illumination intensity unit: mW/m2 for the combination strength of UV light with wavelength range: 240nm~370nm    
  Serial.print("TVZI Illum is:");
  Serial.print(illum);
  Serial.print(" mW/m2 - ");
  Serial.print("250mW is about UV1");
  Serial.println();
  delay(20); 
}

void grovewiki() {  
  int sensorValue;
  long  sum=0;
  for(int i=0;i<1024;i++)// accumulate readings for 1024 times
   {  
      sensorValue=analogRead(A0);
      sum=sensorValue+sum;
      delay(2);
   }   
 long meanVal = sum/1024;  // get mean value
 Serial.print("Wiki Grove UV is:");
 Serial.print((meanVal*1000/4.3-83)/21);// get a detailed calculating expression for UV index in schematic files. 
 Serial.print("\n");
 delay(20); 
}

//My UV Sketch
void SampleUVAlarm (){
  int sensorValue;
  long  sum=0;
  for(int i=0;i<1024;i++)
   {  
      sensorValue=analogRead(A0);
      sum=sensorValue+sum;
      delay(2);
   }   
 sum = sum >> 10;
 Serial.print("SampleUVAlarm - voltage:");
 Serial.print(sum*4980.0/1023.0);
 Serial.print(" mV - ");
 delay(20);
 Serial.print("Grove UVIndex: ");
 uvindex = (307*(sum*4980.0/1023.0))/200; //mW/m2 -> W/m2
 Serial.println(uvindex); 
}
