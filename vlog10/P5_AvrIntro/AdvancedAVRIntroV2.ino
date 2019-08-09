
#include <avr/sleep.h>
#include <avr/wdt.h>

// This program sleeps, wakes up about every second using the watchdog timer, 
// counts ten of those intervals and then does something useful.
// see http://www.gammon.com.au/power

// watchdog timer interrupt, wake from sleep each second (approx)
ISR (WDT_vect) {
} 

void setup() {

  Serial.begin(9600); //initialize serial
  Serial.println("sleep for ten...");
  Serial.flush(); //wait till done printing.
  
  // implement maximum possible power reduction
  // disable ADC

  ADCSRA = 0; 

  //set PRR Power Reduction Register (set PRADC after ADCSRA=0)
  //Bit 7 - PRTWI: Power Reduction TWI
  //Bit 6 - PRTIM2: Power Reduction Timer/Counter2
  //Bit 5 - PRTIM0: Power Reduction Timer/Counter0
  //Bit 4 - Res: Reserved bit
  //Bit 3 - PRTIM1: Power Reduction Timer/Counter1
  //Bit 2 - PRSPI: Power Reduction Serial Peripheral Interface
  //Bit 1 - PRUSART0: Power Reduction USART0
  //Bit 0 - PRADC: Power Reduction ADC

  PRR |= (1<<PRTWI)|(1<<PRTIM0)|(1<<PRTIM1)|(1<<PRTIM2)|(1<<PRSPI)|(1<<PRADC)|(1<<PRUSART0);

  /*  sleep modes on ATmega328p
   SLEEP_MODE_IDLE
   SLEEP_MODE_ADC 
   SLEEP_MODE_PWR_DOWN 
   SLEEP_MODE_PWR_SAVE
   SLEEP_MODE_STANDBY 
   SLEEP_MODE_EXT_STANDBY
   */
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);

  // clear various "reset" flags
  MCUSR = 0;
  // allow changes, disable reset
  WDTCSR = (1<<WDCE) | (1<<WDE);
  // set interrupt mode and an interval
  WDTCSR = (1<<WDIE) | (1<<WDP2)| (1<<WDP1); // set WDIE, and ~1 second delay
  wdt_reset(); // reset it

    int count=0; //sleep loop counter

  // main loop. Sleep, wake up on watchdog, check time and do something

  while(1) {

    sleep_enable();
    cli(); //time critical steps follow
    MCUCR = (1<<BODS) | (1<<BODSE);	// turn on brown-out enable select
    MCUCR = (1<<BODS);        	//Brown out off. This must be done within 4 clock cycles of above
    sei();
    sleep_cpu(); //or sleep mode
    // no need to disable sleep again?
    //sleep_disable();
    // wake up here on WDT interrupt

    count++;
    if (count > 9) { //ten intervals have passed

      count=0; //reset counter
      PRR &= ~(1<<PRUSART0); //turn USART

      Serial.println("about 10 seconds");
      Serial.flush(); //wait till done printing, and snooze

      PRR |= (1<<PRUSART0); //turn off USART                

    } //end if count
  } //end while(1)
}   // end setup()

void loop(){
} //nothing to do
