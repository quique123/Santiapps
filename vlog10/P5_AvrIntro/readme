//When run, Arduino Rebooted is logged and wdt starts counting until 8s
//since ifdef, the dog is patted and it doesnt bite (alarm) so prints
//Arduino Running and delays for 1 second
//ifdef is commented, it should reset after 8 seconds 
//and since its not defined it will never reboot 
//cause the wdt is not set to reboot/reset or interrupt, just fire.
#include <avr/wdt.h>
//#define RESETWATCHDOG

void setup(){
  wdt_disable();
	  Serial.begin(57600);
          Serial.println("");
          Serial.println ("------->Arduino Rebooted");
          Serial.println("");
          wdt_enable(WDTO_8S);
}

void loop(){
#ifdef RESETWATCHDOG
	wdt_reset(); //pat the dog
#endif
	Serial.println("Arduino Running");
	delay(1000);
}
