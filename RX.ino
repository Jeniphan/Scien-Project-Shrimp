
//*******CODEช่วนของตัวรับข้อมูลแสดงผลผ่านหน้าจอLCD และส่งค่าอุณหภูมิไปยัง NodeMCU********

#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01p.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

SoftwareSerial chat(3, 4); // RX, TX
nRF24L01p receiver(7,8);//CSN,CE
LiquidCrystal_I2C lcd(0x27,16,2);

int i= 1;
int a;

void setup()  {

  Serial.begin(9600);

  chat.begin(4800);

  lcd.begin();
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  receiver.channel(90);  // ตั้งช่องความถี่ให้ตรงกัน
  receiver.RXaddress("SWT");  // ตั้งชื่อตำแห่นงให้ตรงกัน ชื่อตั้งได้สูงสุด 5 ตัวอักษร
  receiver.init();
}
String t;
String h;

void loop() 
{ 
if(receiver.available())
  {

    receiver.read(); // สั่งให้เริ่มอ่าน
    receiver.rxPL(t);
    receiver.rxPL(h);
    Serial.println(t);
    Serial.println(h);

    a = t.toInt();
    chat.print(a);
    Serial.println("Send Massseger");
    {
    lcd.setCursor(7,0);
    lcd.print(t);
     
    lcd.setCursor(8,1);
    lcd.print(h);
    lcd.setCursor(2,0);
    lcd.print("Tem:");
    lcd.setCursor(14,0);
    lcd.print("C");
    lcd.setCursor(1,1);
    lcd.print("Light:");
    lcd.setCursor(14,1);
    lcd.print("lx");
    }
    
  }
 t="";
 h="";
}



