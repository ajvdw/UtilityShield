#include <ArduinoJson.h>

/* 
  Uitility Shield (c)2018 by A.J. van de Werken  
  Inspired by ESP_WebConfig by John Lassen. 
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


#include <Ticker.h>
#include <EEPROM.h>
#include <TimeLib.h>  //by Paul Stoffregen, not included in the Arduino IDE !!!
#include <Timezone.h> //by Jack Christensen, not included in the Arduino IDE !!!
 
#include "global.h"
#include "helpers.h"

// HTML
#include "css.h"
#include "favicon.ico.h"
#include "error.html.h"
#include "home.html.h"
#include "wifi.html.h"
#include "info.html.h"
#include "energy.html.h"
#include "solar.html.h"
#include "water.html.h"
#include "pvoutput.html.h"
#include "timezone.html.h"
#include "weather.html.h"
 
#define AdminTimeOut 300  // Defines the Time in Seconds, when the Admin-Mode will be diabled

void setup() 
{
  // put your setup code here, to run once:

  //Serial.begin(115200);
  //Serial.swap(); //GPIO15 (TX) and GPIO13 (RX)
  //Serial.set_tx(1); // GIIO1 (TX) 

  Serial.begin(115200);

  
  //if you get here you have connected to the WiFi
  Serial.println("connected...)");

  EEPROM.begin(512);

  Serial.println("Starting Utility Shield");

  if (!ReadConfig())
  {
    // DEFAULT CONFIG
    config.ssid = "MYSSID";
    config.password = "MYPASSWORD";
    config.dhcp = true;
    config.IP[0] = 192;config.IP[1] = 168;config.IP[2] = 1;config.IP[3] = 100;
    config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
    config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;config.Gateway[3] = 1;

    config.SolarPulseCount = 0;
    config.WaterPulseCount = 0;
    config.PostEvery =  0;
    config.Pulsesperkwh = 1000;
    config.Pulsesperm3 = 1000;
    config.SystemId = 0;
    
    config.GetEvery = 0;
    config.Location = "";
    config.WUApiKey = "";

    // Timezone / DST setting for Central Europe
    config.startweek=0;
    config.startday=0;
    config.startmonth=2;
    config.starthour=2;
    config.startminute=0;
    config.startoffset=2;
    config.endweek=0;
    config.endday=0;
    config.endmonth=9;
    config.endhour=3;
    config.endminute=0;
    config.endoffset=1; 
    WriteConfig();
    
    Serial.println("General config applied");
  }

  timestamp = config.timestamp;

  if (AdminEnabled)
  {
    WiFi.mode(WIFI_AP_STA);
    uint8_t mac[6];
    char apStr[18] = {0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0};
    WiFi.macAddress(mac);
    sprintf(apStr, "Shield-%02X%02X%02X%02X",  mac[2], mac[3], mac[4], mac[5]);
    WiFi.softAP( apStr , "nosecret" );
  }
  else
  {
    WiFi.mode(WIFI_STA);
  }
  
  ConfigureWifi();

  server.on ( "/", send_home_html );
  server.on ( "/favicon.ico", send_favicon_ico );
  server.on ( "/solar.css", send_css ); 
  server.on ( "/status", send_status_json );  
  server.on ( "/ssid", send_ssid_json );  
  server.on ( "/network", send_info_html );
  server.on ( "/power", send_power_html );
  server.on ( "/solar", send_solar_html );
  server.on ( "/water", send_water_html );
  server.on ( "/pvoutput", send_pvoutput_html );
  server.on ( "/weather", send_weather_html );
  server.on ( "/wifi", send_wifi_html );
  server.on ( "/time", send_tzdb_html );
  server.on ( "/reboot", []() { Serial.println("Rebooting"); server.sendHeader("Location", String("/"), true); server.send ( 302, "text/plain", "Rebooting..." );  reboot(); }  );
  
  server.onNotFound ( []() { Serial.println("Page Not Found"); server.send ( 401, "text/html", "Page not Found" );   }  );
  server.begin();
  Serial.println( "HTTP server started" );

  AdminTimeOutCounter = AdminTimeOut;
  
  lSolarPulseCounter = config.SolarPulseCount;
  lWaterPulseCounter = config.WaterPulseCount;   

  // Set day counters       
  SolarPulseCountStart = lSolarPulseCounter;
  WaterPulseCountStart = lWaterPulseCounter;
      
  RebootTimecCounter =  86400 * 6; // Run at least for six days before reboot

	tkSecond.attach(1, Second_Tick);

  int retry = 5;
  while( !TimeValid && retry )
  {
    delay( 1000 );
    SyncTime(); // Initial Call to set the time
    retry --;
  }
  
  prevDays = now() / 86400;
  
  attachInterrupt(SOLAR_PIN , pinSolarChanged, RISING );  

  pinMode( LED_PIN, OUTPUT );
}

void loop ( void ) 
{
  long Days;
  
  if (millis()>taskTime) 
  {
    int sample = analogRead(A0);
    if( sample > ST_UP_TRESHOLD && SchmittTrigger == false ) { pinWaterChanged(); SchmittTrigger = true; }
    if( sample < ST_DN_TRESHOLD && SchmittTrigger == true ) SchmittTrigger = false;
    taskTime = millis() + 50;
  }      

  Days = now() / 86400;
  
  if( prevDays != Days )
  {
    // It a new day! Reset daycounters;
    prevDays = Days;
    WaterPulseCountStart = lWaterPulseCounter;
    SolarPulseCountStart = lSolarPulseCounter;
    SyncTime();
  }
  
  prevDays = Days;
  
	if( AdminEnabled )
	{
		if( AdminTimeOutCounter < 0 )
		{
			 AdminEnabled = false;
			 Serial.println("Admin Mode disabled!");
			 WiFi.mode(WIFI_STA);
		}
	}
  else if( digitalRead( FLASH_PIN ) == LOW )
  {
    AdminTimeOutCounter = AdminTimeOut;
    AdminEnabled = true;         
    Serial.println("Partial admin Mode enabled!");
  }


  if( config.GetEvery  > 0 )
  {
    if( WUndergroundCounter < 0 )
    {
      // Get
      Serial.println("Getting the weather ...");
      GetResult = GetWeather();
      WUndergroundCounter = (config.GetEvery*60);
    }
  }

  
	if( config.PostEvery  > 0 )
	{
		if( PVOutputCounter < 0 )
    {
      // Post
      Serial.println("Posting to pvoutput ...");
      PostResult = PostPVOutput();
      PVOutputCounter = (config.PostEvery*60);
    }
	}

  if( RebootTimecCounter < 0 && SecondsToday() > 3600*4 && Weekday() == 0 ) 
  {   // Reboot after running for at least 6 days on Sunday, After 4AM
    reboot();
  }
  
  if( ResetWattCounter < 0 ) lSolarPulseLength = 0;  
  if( ResetLiterCounter < 0 ) lWaterPulseLength = 0;
  
  handleDSMR();

	server.handleClient();

  
}


