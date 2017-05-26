#ifndef GLOBAL_H
#define GLOBAL_H

ESP8266WebServer server(80);				// The Webserver
Ticker tkSecond;

boolean AdminEnabled = true;		    // Enable Admin Mode for a given Time
bool LastPost = false;
String PostResult="";

volatile unsigned long lSolarPulseLength = 0;
volatile unsigned long lWaterPulseLength = 0;
volatile unsigned long lSolarPulseCounter = 0;
volatile unsigned long lWaterPulseCounter = 0;
volatile unsigned long lastSolar_millis;
volatile unsigned long lastWater_millis;

volatile long ResetWattCounter = 0;
volatile long ResetLiterCounter = 0;
volatile long AdminTimeOutCounter = 0;       // Counter for Disabling the AdminMode
volatile long RebootTimecCounter = 0;
volatile int PVOutputCounter = 0;            // Counter to keep track of time
volatile unsigned long timestamp = 0;

#define LED_PIN    2 //d4  onboard, blue LED
#define SOLAR_PIN 14 //d5
#define WATER_PIN 12 //d6
#define POWER_PIN 13 //d7

const int FLASH_PIN = 0;

static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; 
#define LEAP_YEAR(Y) ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )

#define VERSION_MAJOR  2
#define VERSION_MINOR  1

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
  unsigned long WaterPulseCount;
  unsigned long SolarPulseCount;
  String TZdbApiKey;
  unsigned long timestamp;
  long Pulsesperkwh;
  long Pulsesperm3;

}   config;

#endif

