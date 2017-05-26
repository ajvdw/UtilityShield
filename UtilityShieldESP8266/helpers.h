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
    "<li><a href='/pvoutput'>PVOutput</a></li>\n"+
    "<li><a href='/network'>Network</a></li>\n</ul>\n</div>";
}

String html_footer()
{
  return (String) "<div id='footer'>Knowledge is Power</div>\n</body>\n</html>";
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
  WriteStringToEEPROM(140,config.PVoutputServerName);
  WriteStringToEEPROM(270,config.PVoutputApiKey); 
  WriteStringToEEPROM(372, config.TZdbServerName );  
  EEPROMWritelong(440, config.Latitude );
  EEPROMWritelong(444, config.Longitude );
  WriteStringToEEPROM( 448, config.TZdbApiKey );
  
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
    config.PVoutputServerName = ReadStringFromEEPROM(140);  
    config.PVoutputApiKey = ReadStringFromEEPROM(270);  
    config.TZdbServerName = ReadStringFromEEPROM(372);  
    config.Latitude = EEPROMReadlong(440);  
    config.Longitude= EEPROMReadlong(444);  
    config.TZdbApiKey = ReadStringFromEEPROM(448);  

    return true;
    
  }
  else
  {
    Serial.println("Configurarion NOT FOUND!!!!");
    return false;
  }
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
  return timestamp - config.timestamp;
}

String RunningString()
{
  return (String) Running();
}

unsigned long SecondsToday()
{
  return  timestamp % 86400;
}

unsigned int Weekday()
{
  return (((timestamp/86400) + 4) % 7);  // Sunday is day 0
}

String DateString()
{
  uint8_t _year=0, _month, _day, l;
  uint32_t days=0, t;

  t = (uint32_t)timestamp / 86400;
  while((unsigned)(days += (LEAP_YEAR(_year) ? 366 : 365)) <= t) _year++;
  days -= LEAP_YEAR(_year) ? 366 : 365;
  _day = t-days;
  
  _month=0;
  l=0;
  for (_month=0; _month<12; _month++) 
  {
    if (_month==1) 
    { // february
      if (LEAP_YEAR(_year))l=29; else l=28;
    } 
    else 
      l = monthDays[_month];
      
    if (_day >= l) _day -= l; else break;
  }
  return (String)(_year+1970)+(String)((_month<9)?"0":"")+(String)(_month+1)+(String)((_day<9)?"0":"")+ (String)(_day+1);
}

String TimeString()
{
  uint8_t _hour, _minute;
  uint32_t t;

  t = (uint32_t)timestamp / 60;
  _minute = t % 60;
  t /= 60; // now it is hours
  _hour = t % 24;
  return (String)((_hour<10)?"0":"")+(String)_hour + ":" + (String)((_minute<10)?"0":"") + (String)_minute;
}


String DateTimeString()
{
  uint8_t _year=0, _month, _day, _hour, _minute, _second, l;
  uint32_t days=0, t;

  t = (uint32_t)timestamp;
  _second = t % 60;
  t /= 60; // now it is minutes
  _minute = t % 60;
  t /= 60; // now it is hours
  _hour = t % 24;
  t /= 24; // now it is days
  
  while((unsigned)(days += (LEAP_YEAR(_year) ? 366 : 365)) <= t) _year++;

  days -= LEAP_YEAR(_year) ? 366 : 365;
  _day = t-days;
  
  _month=0;
  l=0;
  for (_month=0; _month<12; _month++) 
  {
    if (_month==1) 
    { // february
      if (LEAP_YEAR(_year))l=29; else l=28;
    } 
    else 
      l = monthDays[_month];
      
    if (_day >= l) _day -= l; else break;
  }
 
  return (String)(_year+1970) + "-" + (String)((_month<9)?"0":"")+(String)(_month+1) + "-" + (String)((_day<9)?"0":"")+ (String)(_day+1) + "  " + (String)_hour + ":" + (String)((_minute<10)?"0":"") + (String)_minute + ":" + (String)((_second<10)?"0":"") + (String)_second;
}

String CountDownString()
{
  return (String)PVOutputCounter;
}


void pinSolarChanged()
{
  long time = millis();
  long temp = 0;

  if( lastSolar_millis > 0  )
    {
      temp =  (time-lastSolar_millis);
    }
    if( temp > 3 ) // 3ms, antidender
    {
      lSolarPulseLength = temp;
      lSolarPulseCounter ++;  
      lastSolar_millis =time;
    }
    ResetWattCounter = 300;
}

void pinWaterChanged()
{
  long time = millis();
  long temp = 0;

  if( lastSolar_millis > 0  )
    {
      temp =  (time-lastWater_millis);
    }
    if( temp > 3 ) // 3ms, antidender
    {
      lWaterPulseLength = temp;
      lWaterPulseCounter ++;  
      lastWater_millis =time;
    }
    ResetLiterCounter = 300;
}

String WattString()
{
  if( lSolarPulseLength > 0 )
    return (String)(long)(3600000.0/(0.001*lSolarPulseLength*config.Pulsesperkwh));
  else
    return "0";
}

String LiterString()
{
  if( lWaterPulseLength > 0 )
    return (String)(long)(3600000.0/(0.001*lWaterPulseLength*config.Pulsesperm3));
  else
    return "0";
}


unsigned long WattHour()
{
  return (unsigned long)(1000.0 * lSolarPulseCounter /  config.Pulsesperkwh);
}

unsigned long LiterHour()
{
  return (unsigned long)(1000.0 * lWaterPulseCounter /  config.Pulsesperm3);
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

String m3hString()
{
  if(  config.Pulsesperm3 > 0 )
  {
    unsigned long lh = LiterHour();
    unsigned long m3h = lh / 1000; 
    lh = lh % 1000;
    
    return (String)(m3h) + "." + (String)((lh<100)?"0":"") + (String)((lh<10)?"0":"") + (String)lh;
  }
  else
    return "0.000";
}

String PostPVOutput()
{
  WiFiClient PVOclient;
  String GETString;
  String currentLine = ""; 

  Serial.println("Connecting to PVOutput");

  if (PVOclient.connect(config.PVoutputServerName.c_str(), 80)) 
  {
    // Build HTTP GET request
    GETString = "GET /service/r2/addstatus.jsp?c1=1";
    GETString += "&d=" + DateString(); 
    GETString += "&t=" + TimeString(); 
    GETString += "&v1="+ (String)WattHour();
    GETString += "&v2="+ WattString();
    GETString += "&sid="+ (String)config.SystemId;
    GETString += "&key="+ config.PVoutputApiKey; // 
    GETString += " HTTP/1.1";

    PVOclient.println(GETString);
    Serial.println(GETString);
    PVOclient.println((String)"Host: "+config.PVoutputServerName);
    Serial.println((String)"Host: "+config.PVoutputServerName);
    PVOclient.println("Connection: close\r\n");
    Serial.println("Connection: close\r\n");
    
    int i = 0;
    while ((!PVOclient.available()) && (i < 1000)) 
    {
      delay(10);
      i++;
    }
  }
  
  while (PVOclient.connected()) 
  {
    if (PVOclient.available()) 
    {
      // read incoming bytes:
      char inChar = PVOclient.read();
      // add incoming byte to end of line:
      currentLine += inChar;
      
      if( currentLine.startsWith( "OK 200:" ) )
      {
        PVOclient.stop();
        return "OK";      
      }
      
      // if you get a newline, clear the line:
      if (inChar == '\n') 
      {
        Serial.println(currentLine);
        currentLine = "";
       }
    } 
  } 

  Serial.println(currentLine);
  return "ERROR";
}

unsigned long GetTimestamp()
{
  WiFiClient DSTclient;
  String GETString;
  String currentLine = ""; 
  bool readingTimestamp = false;
  String strTimestamp = "";
  
  Serial.println("Connecting to DST server");

  if (DSTclient.connect(config.TZdbServerName.c_str(), 80)) 
  {
    // make HTTP GET request to timezonedb.com:
    GETString += "GET /?lat=";
    GETString += (String)(1E-6*config.Latitude); 
    GETString += "&lng=";
    GETString += (String)(1E-6*config.Longitude); // 
    GETString += "&key="+config.TZdbApiKey+" HTTP/1.1";

    DSTclient.println(GETString);
    Serial.println(GETString);
    DSTclient.println((String)"Host: "+config.TZdbServerName);
    Serial.println((String)"Host: "+config.TZdbServerName);
    DSTclient.println("Connection: close\r\n");
    Serial.println("Connection: close\r\n");
    
    int i = 0;
    while ((!DSTclient.available()) && (i < 1000)) 
    {
      delay(10);
      i++;
    }
  }
  while (DSTclient.connected()) 
  {
    if (DSTclient.available()) 
    {
      // read incoming bytes:
      char inChar = DSTclient.read();
      // add incoming byte to end of line:
      currentLine += inChar;

      // if you're currently reading the bytes of a Timestamp,
      // add them to the Timestamp String:
      if (readingTimestamp) {//the section below has flagged that we're getting the timestamp from server here
        if (inChar != '<') {
          strTimestamp += inChar;
        }
        else {
          // if you got a "<" character,
          // you've reached the end of the UTC offset:
          readingTimestamp = false;
          Serial.print("Timestamp in seconds: ");
          Serial.println(strTimestamp);
     
          // close the connection to the server:
          DSTclient.stop();
          //update the internal time-zone
          return strTimestamp.toInt();
        }
      }

      // if you get a newline, clear the line:
      if (inChar == '\n') 
      {
        Serial.println(currentLine);
        currentLine = "";
      }
      // if the current line ends with <text>, it will
      // be followed by the tweet:
      if ( currentLine.endsWith("<timestamp>")) {
        //Serial.println(currentLine);
        readingTimestamp = true;
        strTimestamp = "";
      }
    }
  }

  return 0;
}

void Second_Tick()
{
  if( PVOutputCounter ) PVOutputCounter--;
  AdminTimeOutCounter--; 
  RebootTimecCounter--;
  ResetWattCounter--;
  timestamp ++;

//  digitalWrite(LED_PIN, cSecondCounter%2);
}

