#ifndef GLOBAL_H
#define GLOBAL_H

ESP8266WebServer server(80);				// The Webserver
Ticker tkSecond;

boolean AdminEnabled = true;		    // Enable Admin Mode for a given Time
bool LastPost = false;
String PostResult="";

volatile long lPulseLength = 0;
volatile unsigned long lPulseCounter = 0;
volatile unsigned long last_millis;
volatile long ResetWattCounter = 0;
volatile long AdminTimeOutCounter = 0;       // Counter for Disabling the AdminMode
volatile long RebootTimecCounter = 0;
volatile int PVOutputCounter = 0;            // Counter to keep track of time
volatile unsigned long timestamp=0;

const int LED_PIN = 2;              // Thing's onboard, blue LED
const int PULSE_PIN = 14;
const int FLASH_PIN = 0;

static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; 
#define LEAP_YEAR(Y) ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )

struct strConfig {
	String ssid;
	String password;
	byte  IP[4];
	byte  Netmask[4];
	byte  Gateway[4];
	boolean dhcp;
	String PVoutputServerName;
  long SystemId;
  String PVoutputApiKey;
  long PostEvery;
  String TZdbServerName;
  long Latitude;
  long Longitude;
  unsigned long PulseCount;
  String TZdbApiKey;
  unsigned long timestamp;
  long Pulsesperkwh;
	String DeviceName;
}   config;

#endif
