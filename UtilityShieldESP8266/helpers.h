#include "global.h"


String html_header()
{
  return (String) "<!DOCTYPE html>\n<html>\n<head>\n<link href='solar.css' rel=stylesheet>\n</head>\n<body>\n<div id='header'><h1>Solar, Water and Energy Monitor</h1></div>";
}

String html_menu()
{
  return (String) "<div id='menu'>\n<ul>\n"+
    "<li><a href='/'>Home</a></li>\n"+
    "<li><a href='/wifi'>Wifi</a></li>\n"+
    "<li><a href='/power'>Power</a></li>\n"+
    "<li><a href='/solar'>Solar</a></li>\n"+   
    "<li><a href='/water'>Water</a></li>\n"+
    "<li><a href='/time'>Time</a></li>\n"+
    "<li><a href='/weather'>Weather</a></li>\n"+
    "<li><a href='/pvoutput'>PVOutput</a></li>\n"+
    "<li><a href='/network'>Network</a></li>\n</ul>\n</div>";
}

String html_footer()
{
  return (String) "<div id='footer'>www.github.com/ajvdw</div>\n</body>\n</html>";
}

void WriteStringToEEPROM(int beginaddress, String string)
{
	char  charBuf[string.length()+1];
	string.toCharArray(charBuf, string.length()+1);
	for (int t=  0; t<sizeof(charBuf);t++)
	{
			EEPROM.write(beginaddress + t,charBuf[t]);
	}
}

String ReadStringFromEEPROM(int beginaddress)
{
  byte counter=0;
  char rChar;
  String retString = "";
  while (1)
  {
  	rChar = EEPROM.read(beginaddress + counter);
  	if (rChar == 0) break;
  	if (counter > 63) break;
  	counter++;
  	retString.concat(rChar);
  
  }
  return retString;
}

void EEPROMWritelong(int address, long value)
{
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  
  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  
  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void WriteConfig()
{
  Serial.println("Writing Config");
  EEPROM.write(0,'C');
  EEPROM.write(1,'F');
  EEPROM.write(2,'G');
  EEPROM.write(3,VERSION_MAJOR); 
  EEPROM.write(4,VERSION_MINOR); 
    
  EEPROM.write(16,config.dhcp);
  EEPROMWritelong(18,config.PostEvery); // 4 Byte

  EEPROM.write(32,config.IP[0]);
  EEPROM.write(33,config.IP[1]);
  EEPROM.write(34,config.IP[2]);
  EEPROM.write(35,config.IP[3]);

  EEPROM.write(36,config.Netmask[0]);
  EEPROM.write(37,config.Netmask[1]);
  EEPROM.write(38,config.Netmask[2]);
  EEPROM.write(39,config.Netmask[3]);

  EEPROM.write(40,config.Gateway[0]);
  EEPROM.write(41,config.Gateway[1]);
  EEPROM.write(42,config.Gateway[2]);
  EEPROM.write(43,config.Gateway[3]);
  EEPROMWritelong(44,config.timestamp); // 4 Byte
  EEPROMWritelong(48,config.Pulsesperkwh); // 4 Byte
  EEPROMWritelong(52,config.Pulsesperm3); // 4 Byte
  EEPROMWritelong(56,config.SolarPulseCount); // 4 Byte
  EEPROMWritelong(60,config.WaterPulseCount); // 4 Byte
  EEPROMWritelong(64,config.SystemId); // 4 Byte
  WriteStringToEEPROM(68,config.ssid);
  WriteStringToEEPROM(102,config.password);

  WriteStringToEEPROM(270,config.PVoutputApiKey);   
  EEPROM.write(320, config.startweek);
  EEPROM.write(321, config.startday);
  EEPROM.write(322, config.startmonth);
  EEPROM.write(323, config.starthour);
  EEPROM.write(324, config.startminute);
  EEPROM.write(325, config.startoffset);
  EEPROM.write(326, config.endweek);
  EEPROM.write(327, config.endday);
  EEPROM.write(328, config.endmonth);
  EEPROM.write(329, config.endhour);
  EEPROM.write(330, config.endminute);
  EEPROM.write(331, config.endoffset); 

  EEPROMWritelong(332, config.GetEvery);
  WriteStringToEEPROM( 336, config.WUApiKey);
  WriteStringToEEPROM( 370, config.Location);
  EEPROM.commit();
}

boolean ReadConfig()
{
  
  Serial.println("Reading Configuration");
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' &&  EEPROM.read(3) == VERSION_MAJOR )
  {
    Serial.println("Configurarion Found!");
    config.dhcp =   EEPROM.read(16);
    config.PostEvery = EEPROMReadlong(18); // 4 Byte
    config.IP[0] = EEPROM.read(32);
    config.IP[1] = EEPROM.read(33);
    config.IP[2] = EEPROM.read(34);
    config.IP[3] = EEPROM.read(35);
    config.Netmask[0] = EEPROM.read(36);
    config.Netmask[1] = EEPROM.read(37);
    config.Netmask[2] = EEPROM.read(38);
    config.Netmask[3] = EEPROM.read(39);
    config.Gateway[0] = EEPROM.read(40);
    config.Gateway[1] = EEPROM.read(41);
    config.Gateway[2] = EEPROM.read(42);
    config.Gateway[3] = EEPROM.read(43);

    config.timestamp = EEPROM.read(44);
    config.Pulsesperkwh = EEPROMReadlong(48); // 4 Byte
    config.Pulsesperm3 = EEPROMReadlong(52); // 4 Byte


    config.SolarPulseCount = EEPROMReadlong(56); // 4 Byte
    config.WaterPulseCount = EEPROMReadlong(60); // 4 Byte
    config.SystemId = EEPROMReadlong(64); // 4 Byte
    config.ssid = ReadStringFromEEPROM(68);
    config.password = ReadStringFromEEPROM(102);
 
    config.PVoutputApiKey = ReadStringFromEEPROM(270);    
    config.startweek=EEPROM.read(320);
    config.startday=EEPROM.read(321);
    config.startmonth=EEPROM.read(322);
    config.starthour=EEPROM.read(323);
    config.startminute=EEPROM.read(324);
    config.startoffset=EEPROM.read(325);
    config.endweek=EEPROM.read(326);
    config.endday=EEPROM.read(327);
    config.endmonth=EEPROM.read(328);
    config.endhour=EEPROM.read(329);
    config.endminute=EEPROM.read(330);
    config.endoffset=EEPROM.read(331); 
    
    config.GetEvery =EEPROMReadlong(332);
    config.WUApiKey = ReadStringFromEEPROM(336);
    config.Location = ReadStringFromEEPROM(370); 
    
    return true;
    
  }
  else
  {
    Serial.println("Configurarion NOT FOUND!!!!");
    return false;
  }
}

void reboot()
{
    config.SolarPulseCount = lSolarPulseCounter;
    config.WaterPulseCount = lWaterPulseCounter;
    config.timestamp = now();
    // Save pulsecounter
    WriteConfig();
    ESP.restart();
}

// Check the Values is between 0-255
boolean checkRange(String Value)
{
   if (Value.toInt() < 0 || Value.toInt() > 255)
   {
     return false;
   }
   else
   {
     return true;
   }
}

// convert a single hex digit character to its integer value (from https://code.google.com/p/avr-netino/)
unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}

String urldecode(String input) // (based on https://code.google.com/p/avr-netino/)
{
   char c;
   String ret = "";
   
   for(byte t=0;t<input.length();t++)
   {
     c = input[t];
     if (c == '+') c = ' ';
         if (c == '%') {


         t++;
         c = input[t];
         t++;
         c = (h2int(c) << 4) | h2int(input[t]);
     }
    
     ret.concat(c);
   }
   return ret;
  
}

String GetMacAddress()
{
    uint8_t mac[6];
    char macStr[18] = {0};
    
    WiFi.macAddress(mac);
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],  mac[1], mac[2], mac[3], mac[4], mac[5]);
    return  String(macStr);
}

void ConfigureWifi()
{
  Serial.println("Configuring Wifi");
  
  WiFi.begin (config.ssid.c_str(), config.password.c_str());
  if (!config.dhcp)
  {
    WiFi.config(IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3] ),  IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ) , IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
  }
}

unsigned long Running()
{
  return millis()/1000;
}

String RunningString()
{
  return (String) Running();
}

unsigned long SecondsToday()
{
  return  now() % 86400;
}

unsigned int Weekday()
{
  return (((now()/86400) + 4) % 7);  // Sunday is day 0
}

String DateString()
{
  uint16_t _year=year(), _month=month(), _day=day();

  return (String)(_year)+(String)((_month<10)?"0":"")+(String)(_month)+(String)((_day<10)?"0":"")+ (String)(_day);
}

String TimeString()
{
  uint16_t _hour=hour(), _minute=minute();
  return (String)((_hour<10)?"0":"")+(String)_hour + ":" + (String)((_minute<10)?"0":"") + (String)_minute;
}


String DateTimeString()
{
  uint16_t _year=year(), _month=month(), _day=day(), _hour=hour(), _minute=minute(), _second=second();
  return (String)(_year) + "-" + (String)((_month<10)?"0":"")+(String)(_month) + "-" + (String)((_day<10)?"0":"")+ (String)(_day) + "  " + (String)_hour + ":" + (String)((_minute<10)?"0":"") + (String)_minute + ":" + (String)((_second<10)?"0":"") + (String)_second;
}

String CountDownString()
{
  if( config.PostEvery  > 0 )
    return (String)PVOutputCounter;
  else
    return "-";
}


void pinSolarChanged()
{
  long time = millis();
  long temp = 0;

  if( lastSolar_millis >= 0  ) temp =  (time-lastSolar_millis);
 
  if( temp > 20 ) // 20ms, anti bounce
  {
    if( temp < 300000 )
      lSolarPulseLength = temp;
    else 
      lSolarPulseLength = 300000;
    lSolarPulseCounter ++;  
    lastSolar_millis = time;
  }
  ResetWattCounter = 300; // Fall back to 0 after 300 seconds
}


void pinWaterChanged()
{
  long time = millis();
  long temp = 0;

  if( lastWater_millis >= 0  ) temp =  (time-lastWater_millis);
  if( temp > 150 ) // 150ms, anti bounce
  {
    if( temp < 60000 )
      lWaterPulseLength = temp;
    else
      lWaterPulseLength = 60000; 
    lWaterPulseCounter ++;  
    lastWater_millis = time;
  }
  ResetLiterCounter = 60; // Fall back to 0 after 60 seconds
}

String Watt()
{
  long w;
  if( lSolarPulseLength > 0 && config.Pulsesperkwh > 0 )
    w=(3600000.0/(0.001*lSolarPulseLength*config.Pulsesperkwh));
  else
    w=0;
  return (String)w;
}


float LiterPerMinute()
{
  if( lWaterPulseLength > 0 && config.Pulsesperm3 > 0 )
    return 60000.0/(0.001*lWaterPulseLength*config.Pulsesperm3);
  else
    return 0.0;
}

unsigned long WattHour()
{
  return config.Pulsesperkwh > 0?(unsigned long)(0.5+1000.0*lSolarPulseCounter/config.Pulsesperkwh) : 0;
}

unsigned long DailyWattHourSolar()
{
  return config.Pulsesperkwh > 0?(unsigned long)(0.5+1000.0*(lSolarPulseCounter-SolarPulseCountStart)/config.Pulsesperkwh) : 0;
}

unsigned long DailyLiterWater()
{
  return config.Pulsesperm3 > 0?(unsigned long)(0.5+1000.0*(lWaterPulseCounter-WaterPulseCountStart)/config.Pulsesperm3) : 0;  
}

unsigned long Liter()
{
  return config.Pulsesperm3 > 0?(unsigned long)(0.5+1000.0*lWaterPulseCounter/config.Pulsesperm3) : 0;
}

String kWhString()
{
  if(  config.Pulsesperkwh > 0 )
  {
    unsigned long wh = WattHour();
    unsigned long kwh = wh / 1000; 
    wh = wh % 1000;
    
    return (String)(kwh) + "." + (String)((wh<100)?"0":"") + (String)((wh<10)?"0":"") + (String)wh;
  }
  else
    return "0.000";
}

String m3String()
{
  if(  config.Pulsesperm3 > 0 )
  {
    unsigned long lt = Liter();
    unsigned long m3 = lt / 1000; 
    lt = lt % 1000;
    
    return (String)(m3) + "." + (String)((lt<100)?"0":"") + (String)((lt<10)?"0":"") + (String)lt;
  }
  else
    return "0.000";
}

/*
void readDSMR() {
  long tl = 0;
  long tld =0;

  if (Serial.available()) {
    input = Serial.read();
    char inChar = (char)input;
    // Fill buffer up to and including a new line (\n)
    buffer[bufpos] = input&127;
    bufpos++;

    if (input == '\n') { // We received a new line (data up to \n)
      if (sscanf(buffer,"1-0:1.8.1(%ld.%ld" ,&tl, &tld)==2){
        tl *= 1000;
        tl += tld;
        //mEVLT = tl;
      }

      // 1-0:1.8.2 = Elektra verbruik hoog tarief (DSMR v4.0)
      if (sscanf(buffer,"1-0:1.8.2(%ld.%ld" ,&tl, &tld)==2){
        tl *= 1000;
        tl += tld;
        //mEVHT = tl;
      }

      // 1-0:1.7.0 = Electricity consumption actual usage (DSMR v4.0)
      if (sscanf(buffer,"1-0:1.7.0(%ld.%ld" ,&tl , &tld) == 2)
      { 
        //mEAV = (tl*1000)+tld;
      }

      // 0-1:24.2.1 = Gas (DSMR v4.0) on Kaifa MA105 meter
      if (strncmp(buffer, "0-1:24.2.1", strlen("0-1:24.2.1")) == 0) {
        if (sscanf(strrchr(buffer, '(') + 1, "%d.%d", &tl, &tld) == 2) {
          //mG = (tl*1000)+tld; 
        }
      }

      // Empty buffer again (whole array)
      for (int i=0; i<75; i++)
      { 
        buffer[i] = 0;
      }
      bufpos = 0;
    }
  } //Einde 'if AltSerial.available'
} //Einde 'decodeTelegram()' functie

*/

bool SyncTime()
{
  String GETString;

  TimeValid = false;
  
  // Build HTTP GET request
  GETString = "http://pvoutput.org";
  GETString += "/service/r2/getsystem.jsp";
  GETString += "?sid="+ (String)config.SystemId;
  GETString += "&key="+ config.PVoutputApiKey; 

  HTTPClient http;

  const char * headerkeys[] = {"Date"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  http.collectHeaders(headerkeys, headerkeyssize );
  http.begin(GETString);
  int httpCode = http.GET();

  if( http.hasHeader( "Date" ) )
  {  
    String GMTString = http.header("Date");
    char tmp[81], *pt;
    int i=0;
    char *months = "JanFebMarAprMayJunJulAugSepOctNovDec", *m;
    tmElements_t tm;
    time_t t, l;
    
    strncpy( tmp, GMTString.c_str(), 80 );tmp[80]=0;

    // parse date time 
    pt=strtok( tmp, " :" ); // day of week
    while( pt )
    {
      switch(i)
      {
        case 1: tm.Day=atoi(pt);  break;
        case 2: m=strstr(months,pt); m[3]=0;if(m)tm.Month=((m-months)/3+1); break;
        case 3: tm.Year=atoi(pt)-1970; break;
        case 4: tm.Hour=atoi(pt); break;
        case 5: tm.Minute=atoi(pt); break;
        case 6: tm.Second=atoi(pt);break;
      }
      i++;
      pt = strtok( NULL, " :" );
    }    

    // GMT
    t = makeTime(tm); 

    long startshift = config.startoffset - 12;
    if( startshift < 0 ) startshift = startshift * 60 - config.startminute; else startshift = startshift * 60 + config.startminute;
    
    long endshift = config.endoffset - 12;
    if( endshift < 0 ) endshift = endshift * 60 - config.endminute; else endshift = endshift * 60 + config.endminute;
       
    TimeChangeRule tcrDST = { "DST", config.startweek, config.startday+1, config.startmonth+1, config.starthour, startshift };
    TimeChangeRule tcrSTD = { "STD", config.endweek,   config.endday+1,   config.endmonth+1,   config.endhour,   endshift   };  
    Timezone TZ(tcrDST, tcrSTD);
    TimeChangeRule *tcr;        

    // convert to local timezone
    l = TZ.toLocal(t, &tcr);

    // adjust clock to timestamp in header
    setTime( l ) ; 
    TimeValid = true;   
  }
  http.end();
  return TimeValid;
}

String GetWeather()
{
  String GETString;
  GETString = "http://api.wunderground.com/api/";
  GETString += config.WUApiKey;
  GETString += "/geolookup/conditions/q/IA/";
  GETString += config.Location + ".json";
  
  HTTPClient http;
  
  http.begin(GETString);
  int httpCode = http.GET();

Serial.println( httpCode );
  if( httpCode == 200 )
  {
    int start;
    int semicolon;
    int comma;    

    String response = http.getString(); 
    String pair;
    
    start = response.indexOf( "\"city\":" );
    if( start > 0 )
    {
      pair = response.substring(start,start+135);
      semicolon = pair.indexOf( ":" );
      comma = pair.indexOf(",");
      if( semicolon > 0 && comma > 0 )
      WULocation = pair.substring(semicolon+2,comma-1);
      //Serial.println( WULocation );
    }    

    start = response.indexOf( "\"temp_c\":" );
    if( start > 0 )
    {
      pair = response.substring(start,start+32);
      semicolon = pair.indexOf( ":" );
      comma = pair.indexOf(",");
      if( semicolon > 0 && comma > 0 )
      WUTemp = pair.substring(semicolon+1,comma);
    }   

    start = response.indexOf( "\"precip_today_metric\":" );
    if( start > 0 )
    {
      pair = response.substring(start,start+32);
      semicolon = pair.indexOf( ":" );
      comma = pair.indexOf(",");
      if( semicolon > 0 && comma > 0 )
      WURaintoday = pair.substring(semicolon+2,comma-1);
    }    

    start = response.indexOf( "\"wind_dir\":" );
    if( start > 0 )
    {
      pair = response.substring(start,start+32);
      semicolon = pair.indexOf( ":" );
      comma = pair.indexOf(",");
      if( semicolon > 0 && comma > 0 )
      WUWindDir = pair.substring(semicolon+2,comma-1);
    }    


    start = response.indexOf( "\"wind_kph\":" );
    if( start > 0 )
    {
      pair = response.substring(start,start+32);
      semicolon = pair.indexOf( ":" );
      comma = pair.indexOf(",");
      if( semicolon > 0 && comma > 0 )
      WUWindSpeed = pair.substring(semicolon+1,comma);

    } 
     
  }

  
  http.end();

  return (httpCode == HTTP_CODE_OK)? "OK": "ERROR";
}

String weatherString()
{
  String Result = WUTemp + "C " + WURaintoday + "mm " + WUWindSpeed + "km/h " + WUWindDir;
  return Result;
}

String PostPVOutput()
{
  String GETString;
  static unsigned long prevWattHour = WattHour();
  static unsigned long prevLiter = Liter();
  unsigned long dt =   now() - PVOutputPosted;
  long power;
  float waterflow;

  power = 0;
  waterflow = 0.0;
  PVOutputPosted = now();

  if( dt > 0 )
  {
    power = 3600L * (WattHour() - prevWattHour) / dt;   // Watt  
    waterflow = 60.0 * (Liter() - prevLiter) / dt;      // Liter per minute
  }
    
  // Build HTTP GET request
  GETString = "http://pvoutput.org";
  GETString += "/service/r2/addstatus.jsp?c1=1";
  GETString += "&d=" + DateString(); 
  GETString += "&t=" + TimeString(); 
  GETString += "&v1="+ (String)WattHour();
  GETString += "&v2="+ (String)power;
  GETString += "&v5="+ (String)WUTemp;
  GETString += "&v7="+ (String)DailyLiterWater();
  GETString += "&v8="+ String(waterflow,1);
  GETString += "&v11="+ String(WUWindSpeed);
  GETString += "&sid="+ (String)config.SystemId;
  GETString += "&key="+ config.PVoutputApiKey; // 
  
  prevWattHour = WattHour();
  prevLiter = Liter();
  
  HTTPClient http;
  
  http.begin(GETString);
  int httpCode = http.GET();
  
  //Serial.println( (String)"PVOutput response: " + http.getString() );

  http.end();

  return (httpCode == HTTP_CODE_OK)? "OK": "ERROR";
}

void Second_Tick()
{
  PVOutputCounter--;
  WUndergroundCounter--;
  AdminTimeOutCounter--; 
  RebootTimecCounter--;
  ResetWattCounter--;
  ResetLiterCounter--;
}


