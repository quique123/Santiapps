/*
* WiFlyHQ Example httpclient.ino
*
* This sketch implements a simple Web client that connects to a 
* web server, sends a GET, and then sends the result to the 
* Serial monitor.
*
* This sketch is released to the public domain.
*
*/

#include <WiFlyHQ.h>

#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(2,3);

/* Work around a bug with PROGMEM and PSTR where the compiler always
 * generates warnings.
 */
#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 
#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

WiFly wifly;

/* Change these to match your WiFi network */
const char mySSID[] = "my wifi";
const char myPassword[] = "my key";
void sendIndex();
void send404();
void terminal();

char buf[80];

void setup() {
   //char buf[32];

   Serial.begin(9600);
   Serial.println("Starting");
   Serial.print("Free memory: ");
   Serial.println(wifly.getFreeMemory(),DEC);

   wifiSerial.begin(9600);
   if (!wifly.begin(&wifiSerial, &Serial)) {
       Serial.println("Failed to start wifly");
	terminal();
   }
   /* Join wifi network if not already associated */
   if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
	Serial.println("Joining network");
	wifly.setSSID(mySSID);
        wifly.setPassphrase(myPassword);
	wifly.enableDHCP();

	if (wifly.join()) {
	    Serial.println("Joined wifi network");
	} else {
	    Serial.println("Failed to join wifi network");
	    terminal();
	}
   } else {
       Serial.println("Already joined network");
   }
   Serial.print("MAC: ");
   Serial.println(wifly.getMAC(buf, sizeof(buf)));
   Serial.print("IP: ");
   Serial.println(wifly.getIP(buf, sizeof(buf)));
   Serial.print("Netmask: ");
   Serial.println(wifly.getNetmask(buf, sizeof(buf)));
   Serial.print("Gateway: ");
   Serial.println(wifly.getGateway(buf, sizeof(buf)));
   // THIS CHANGES FROM CLIENT TO SERVER
   wifly.setBroadcastInterval(0);	// Turn off UPD broadcast

   wifly.setDeviceID("Wifly-WebServer");
   Serial.print("DeviceID: ");
   Serial.println(wifly.getDeviceID(buf, sizeof(buf)));
   
   if (wifly.isConnected()) {
       Serial.println("Old connection active. Closing");
	wifly.close();
   }
   
   wifly.setProtocol(WIFLY_PROTOCOL_TCP);
    if (wifly.getPort() != 80) {
        wifly.setPort(80);
	/* local port does not take effect until the WiFly has rebooted (2.32) */
	wifly.save();
	Serial.println(F("Set port to 80, rebooting to make it work"));
	wifly.reboot();
	delay(3000);
    }
    Serial.println(F("Ready"));
}

void loop() {
  //char buf[80];
  if (wifly.available() > 0) {
        /* See if there is a request */
        Serial.println("wifly.available! :-)");
	if (wifly.gets(buf, sizeof(buf))) {
	    if (strncmp_P(buf, PSTR("GET / "), 6) == 0) {
		/* GET request */
		Serial.println(F("Got GET request"));
		while (wifly.gets(buf, sizeof(buf)) > 0) {
		    /* Skip rest of request */
		}
		sendIndex();
		Serial.println(F("Sent index page"));
	    } else if (strncmp_P(buf, PSTR("POST"), 4) == 0) {
	        /* Form POST */
	        char username[16];
	        Serial.println(F("Got POST"));

		/* Get posted field value */
		if (wifly.match(F("user="))) {
		    wifly.gets(username, sizeof(username));
		    wifly.flushRx();		// discard rest of input
		    sendGreeting(username);
		    Serial.println(F("Sent greeting page"));
		}
	    } else {
	        /* Unexpected request */
		Serial.print(F("Unexpected: "));
		Serial.println(buf);
		wifly.flushRx();		// discard rest of input
		Serial.println(F("Sending 404"));
		send404();
	    }
        }
  }
}

/** Send an index HTML page with an input box for a username */
void sendIndex() {
    /* Send the header direclty with print */
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();

    /* Send the body using the chunked protocol so the client knows when
     * the message is finished.
     * Note: we're not simply doing a close() because in version 2.32
     * firmware the close() does not work for client TCP streams.
     */
    wifly.sendChunkln(F("<html>"));
    wifly.sendChunkln(F("<title>WiFly HTTP Server Example</title>"));
    wifly.sendChunkln(F("<h1>"));
    wifly.sendChunkln(F("<p>Hello</p>"));
    wifly.sendChunkln(F("</h1>"));
    wifly.sendChunkln(F("<form name=\"input\" action=\"/\" method=\"post\">"));
    wifly.sendChunkln(F("Username:"));
    wifly.sendChunkln(F("<input type=\"text\" name=\"user\" />"));
    wifly.sendChunkln(F("<input type=\"submit\" value=\"Submit\" />"));
    wifly.sendChunkln(F("</form>")); 
    wifly.sendChunkln(F("</html>"));
    wifly.sendChunkln();
}

/** Send a greeting HTML page with the user's name and an analog reading */
void sendGreeting(char *name)
{
  Serial.println(name);
  //MEVP Oct 28 2017
  if (strcmp(name, "koko") == 0) {
    digitalWrite(12,HIGH);
    Serial.println("high");
  } else {
    Serial.println("low");
    digitalWrite(12,LOW);
  }
    /* Send the header directly with print */
    wifly.println(F("HTTP/1.1 200 OK"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();

    /* Send the body using the chunked protocol so the client knows when
     * the message is finished.
     */
    wifly.sendChunkln(F("<html>"));
    wifly.sendChunkln(F("<title>WiFly HTTP Server Example</title>"));
    /* No newlines on the next parts */
    wifly.sendChunk(F("<h1><p>Hello "));
    wifly.sendChunk(name);
    /* Finish the paragraph and heading */
    wifly.sendChunkln(F("</p></h1>"));

    /* Include a reading from Analog pin 0 */
    snprintf_P(buf, sizeof(buf), PSTR("<p>Analog0=%d</p>"), analogRead(A0));
    wifly.sendChunkln(buf);

    wifly.sendChunkln(F("</html>"));
    wifly.sendChunkln();
}

/** Send a 404 error */
void send404() {
    wifly.println(F("HTTP/1.1 404 Not Found"));
    wifly.println(F("Content-Type: text/html"));
    wifly.println(F("Transfer-Encoding: chunked"));
    wifly.println();
    wifly.sendChunkln(F("<html><head>"));
    wifly.sendChunkln(F("<title>404 Not Found</title>"));
    wifly.sendChunkln(F("</head><body>"));
    wifly.sendChunkln(F("<h1>Not Found</h1>"));
    wifly.sendChunkln(F("<hr>"));
    wifly.sendChunkln(F("</body></html>"));
    wifly.sendChunkln();
}

/* Connect the WiFly serial to the serial monitor. */
void terminal() {
   while (1) {
	if (wifly.available() > 0) {
	    Serial.write(wifly.read());
	}
	if (Serial.available() > 0) {
	    wifly.write(Serial.read());
	}
   }
}
