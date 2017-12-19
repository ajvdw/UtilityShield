#ifndef GLOBAL_H
#define GLOBAL_H

ESP8266WebServer server(80);				// The Webserver
Ticker tkSecond;

boolean AdminEnabled = true;		    // Enable Admin Mode for a given Time
bool TimeValid = false;
String PostResult="";
unsigned long taskTime=0;
  
volatile unsigned long lSolarPulseLength = 0;
volatile unsigned long lWaterPulseLength = 0;
volatile unsigned long lSolarPulseCounter = 0;
volatile unsigned long lWaterPulseCounter = 0;
volatile unsigned long WaterPulseCountStart = 0;
volatile unsigned long SolarPulseCountStart = 0;
  
volatile unsigned long lastSolar_millis;
volatile unsigned long lastWater_millis;

volatile long ResetWattCounter = 0;
volatile long ResetLiterCounter = 0;
volatile long AdminTimeOutCounter = 0;       // Counter for Disabling the AdminMode
volatile long RebootTimecCounter = 0;
volatile int PVOutputCounter = 0;            // Counter to keep track of time
volatile unsigned long timestamp = 0;
volatile unsigned long PVOutputPosted = 0;
volatile unsigned long prevDays = 0;

bool SchmittTrigger=false;

#define ST_UP_TRESHOLD  800
#define ST_DN_TRESHOLD  450

#define LED_PIN    2 //d4  onboard, blue LED
#define SOLAR_PIN 14 //d5
#define WATER_PIN 12 //d6
#define POWER_PIN 13 //d7

const int FLASH_PIN = 0;

#define VERSION_MAJOR  2
#define VERSION_MINOR  3

struct strConfig {
	String ssid;
	String password;
	byte  IP[4];
	byte  Netmask[4];
	byte  Gateway[4];
	boolean dhcp;

  long SystemId;
  String PVoutputApiKey;
  long PostEvery;
  unsigned long WaterPulseCount;
  unsigned long SolarPulseCount;
  unsigned long timestamp;
  long Pulsesperkwh;
  long Pulsesperm3;
  
  // Timezone, daylight saving
  byte startweek;
  byte startday;
  byte startmonth;
  byte starthour;
  byte startminute;
  byte startoffset;
  byte endweek;
  byte endday;
  byte endmonth;
  byte endhour;
  byte endminute;
  byte endoffset;
}   config;


#endif

