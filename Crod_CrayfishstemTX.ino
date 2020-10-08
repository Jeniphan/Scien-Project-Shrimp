//*****CODEช่วนของตัวประมวลผลหลัก****

#include <DallasTemperature.h>
#include <OneWire.h>
#include "TSL2561.h"
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01p.h>

#define TEMPDigital 2
#define RELAY1 5
#define RELAY2 6

OneWire oneWire(TEMPDigital);
DallasTemperature sensors(&oneWire);
nRF24L01p receiver(7,8);//CSN,CE

TSL2561 tsl(TSL2561_ADDR_FLOAT); 
 
void setup()
{
  tsl.setGain(TSL2561_GAIN_16X);
  tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);
  pinMode(RELAY1,OUTPUT);
  pinMode(RELAY2,OUTPUT);
  Serial.begin(115200);
  Serial.println("Dallas Temperature");
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  receiver.channel(90);
  receiver.TXaddress("SWT");
  receiver.init();
  
  sensors.begin();
}

void loop()
{
  float TEMP = sensors.getTempCByIndex(0);
  sensors.requestTemperatures();
  Serial.print("Temperature: ");
  Serial.println(TEMP);
  
  
 if ( TEMP > 24.99)
 {
    Serial.println("HOT");
    digitalWrite(RELAY1,LOW);
 }
 if ( TEMP <= 22.00)
 {
    Serial.println("COLD");
    digitalWrite(RELAY1,HIGH);
 }


  uint16_t LIGHT = tsl.getFullLuminosity();
  Serial.print("Light: ");
  Serial.print(LIGHT);
  Serial.println(" lx");

  if (LIGHT >= 30)
    {
     Serial.println("CLOSD");
     digitalWrite(RELAY2,HIGH);
    }
    
  else 
   {
    Serial.println("OPEN");
    digitalWrite(RELAY2,LOW);
   }
 float ligh = LIGHT;
 String A = String(TEMP);
 String B = String(ligh);
 
  receiver.txPL(A);
  receiver.txPL(B);
  receiver.send(FAST); 
 
 delay(1000);
}
