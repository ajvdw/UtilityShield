/* 
  Solar Meter (c)2015 by A.J. van de Werken  
  Inspired by ESP_WebConfig by John Lassen. 
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <time.h>
#include <Ticker.h>
#include <EEPROM.h>
#include "global.h"
#include "helpers.h"

// HTML
#include "css.h"
#include "error.html.h"
#include "home.html.h"
#include "wifi.html.h"
#include "info.html.h"
#include "meter.html.h"
#include "pvoutput.html.h"
#include "timezonedb.html.h"
 
#define AdminTimeOut 300  // Defines the Time in Seconds, when the Admin-Mode will be diabled

void setup ( void ) {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
//  pinMode(PULSE_PIN, INPUT_PULLUP);
  pinMode(FLASH_PIN, INPUT_PULLUP);
      
	EEPROM.begin(512);
	Serial.begin(115200);
	delay(500);
 
	Serial.println("Starting Solar Meter");
	if (!ReadConfig())
	{
		// DEFAULT CONFIG
		config.ssid = "MYSSID";
		config.password = "MYPASSWORD";
		config.dhcp = true;
		config.IP[0] = 192;config.IP[1] = 168;config.IP[2] = 1;config.IP[3] = 100;
		config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
		config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;config.Gateway[3] = 1;
		config.PVoutputServerName = "";
    config.PulseCount = 0;
		config.PostEvery =  0;
    config.Pulsesperkwh = 1000;
    config.SystemId = 0;
		config.DeviceName = "";
    config.TZdbServerName = "";  
    config.Latitude = 0;  
    config.Longitude= 0;  
    config.TZdbApiKey = "";  
 		WriteConfig();
		Serial.println("General config applied");
	}

  // Get current time at location
  unsigned long t = GetTimestamp();
  if( t > 0 ) 
    timestamp = t;
  else
    timestamp = config.timestamp;

	if (AdminEnabled)
	{
		WiFi.mode(WIFI_AP_STA);
    uint8_t mac[6];
    char apStr[18] = {0};
    WiFi.macAddress(mac);
    sprintf(apStr, "Solar %02X%02X%02X%02X",  mac[2], mac[3], mac[4], mac[5]);
		WiFi.softAP( apStr , "bestkeptsecret" );
	}
	else
	{
		WiFi.mode(WIFI_STA);
	}

	ConfigureWifi();
  server.on ( "/solar.css", send_css ); 
  server.on ( "/", send_home_html );
  server.on ( "/status", send_status_json );  
  server.on ( "/ssid", send_ssid_json );  
  server.on ( "/network", send_info_html );
  server.on ( "/meter", send_meter_html );
  server.on ( "/pvoutput", send_pvoutput_html );
  server.on ( "/wifi", send_wifi_html );
  server.on ( "/time", send_tzdb_html );
  
	server.onNotFound ( []() { Serial.println("Page Not Found"); server.send ( 401, "text/html", "Page not Found" );   }  );
	server.begin();
	Serial.println( "HTTP server started" );

  AdminTimeOutCounter = AdminTimeOut;
  lPulseCounter = config.PulseCount;
  RebootTimecCounter =  86400 * 6; // Run at least for six days before reboot
  
	tkSecond.attach(1, Second_Tick);
  
  attachInterrupt(PULSE_PIN , pinChanged, RISING );
  
}

void loop ( void ) 
{
	if (AdminEnabled)
	{
		if (AdminTimeOutCounter < 0)
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
	
	if (config.PostEvery  > 0 )
	{
		if( PVOutputCounter <= 0 )
    {
      // Post
      PostResult = PostPVOutput();
      PVOutputCounter = (config.PostEvery * 60 - timestamp %(config.PostEvery * 60));
    }
	}

  if( RebootTimecCounter < 0 && SecondsToday() > 3600*4 && Weekday() == 0 ) 
  {   // Reboot after running for at least 6 days on Sunday, After 4  
      config.PulseCount = lPulseCounter;
      config.timestamp = timestamp;
      // Save pulsecounter
      WriteConfig();
      ESP.restart();
  }

  if( ResetWattCounter < 0 ) 
    lPulseLength = 0;
    
	server.handleClient();
}

