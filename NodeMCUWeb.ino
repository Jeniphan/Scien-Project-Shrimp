//*****CODE ช่วนของ NodeMCU ที่รับค่าและนำค่าส่งขึ้นเว็บ*****

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>

SoftwareSerial chat(D5, D6); // RX, TX
  // Config Firebase
  #define FIREBASE_HOST "test-f08f2.firebaseio.com"
  #define FIREBASE_AUTH "c5MLcTLDhUvMJWbaTDs2KDs48vEHlvUD8XxNYPo8"
   
  // Config connect WiFi
  #define WIFI_SSID "Android"
  #define WIFI_PASSWORD "0123456789"

String a;
int T = 1;
int I;

int timezone = 7;
    
  char ntp_server1[20] = "ntp.ku.ac.th";
  char ntp_server2[20] = "fw.eng.ku.ac.th";
  char ntp_server3[20] = "time.uni.net.th";

int dst = 0;

void setup() 
{
  Serial.print("Connecting to ");
     
  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
   
  while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay(500);
    }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
  Serial.println("Waiting for time");
  while (!time(nullptr)) 
    {
      Serial.print(".");
      delay(500);
    }
  Serial.println();
  Serial.println("Now: " + NowString());
   
   
  Serial.println("test!");
  
  Serial.begin(115200);

  chat.begin(4800);

}

void loop() 
{

Serial.println("Please wait Serial..");

  while (T == 1) 
  {   
    chat.print("1");
   if (chat.readString())
   {
    a = chat.readString();  // อ่าน Serial และนำไปเก็บในตัวแปร A
    Serial.println(a);
    I = a.toInt();

  if(I > 0)
    {
   StaticJsonBuffer<200> jsonBuffer;
   JsonObject& root = jsonBuffer.createObject();
   
  root["temperature"] = a;
  root["time"] = NowString();
    
  // append a new value to /logDHT
  String name = Firebase.push("logDHT", root);
  // handle error
  if (Firebase.failed()) 
    {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());
      return;
    }
  Serial.print("pushed: /logDHT/");
  Serial.println(name);
    }
    }
  }
}
  String NowString() 
  {
      time_t now = time(nullptr);
      struct tm* newtime = localtime(&now);
   
      String tmpNow = "";
     tmpNow += String(newtime->tm_year + 1900);
     tmpNow += "-";
     tmpNow += String(newtime->tm_mon + 1);
     tmpNow += "-";
     tmpNow += String(newtime->tm_mday);
     tmpNow += " ";
     tmpNow += String(newtime->tm_hour);
     tmpNow += ":";
     tmpNow += String(newtime->tm_min);
     tmpNow += ":";
     tmpNow += String(newtime->tm_sec);
     return tmpNow;
    }
 

