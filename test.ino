#include <ESP8266WiFi.h>

#define SOLAR_PIN 14 //d5
#define WATER_PIN 12 //d6
#define P1_PIN 13    //d7

#define BUILTIN_LED 2 // d4

void setup()
{
  Serial.begin(115200);
  pinMode(SOLAR_PIN, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);  // initialize onboard LED as output
}
 
void loop() 
{                     
  digitalWrite( BUILTIN_LED, ( digitalRead(SOLAR_PIN) ) );
}

