#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include<Wire.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


const char *ssid = "TANK MONITOR";
const char *pass = "password"; 


unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192,168,4,1);
IPAddress ClientIP(192,168,4,2);

int buzzer=12;
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

char packetBuffer[9];   //Where we get the UDP data
//=======================================================================
//                Setup
//=======================================================================
void setup()
{
    Serial.begin(9600);
    Serial.println();
    pinMode(12,OUTPUT);
                                                        
    lcd.begin();
    lcd.backlight();
    lcd.clear();
    //HEADING
    lcd.setCursor(3,0);
    lcd.print("TANK MONITOR");
    delay(1000);


    WiFi.softAP(ssid, pass);    //Create Access point
   
    //Start UDP
    Serial.println("Starting UDP");
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());
    
}
//======================================================================
//                MAIN LOOP
//======================================================================
void loop()
{  
  udp.parsePacket();
  udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

  
  Serial.println("Data received ");

    Serial.print("Level = "); 
    Serial.println(packetBuffer[0]);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Level: ");
    digitalWrite(buzzer,LOW);
  if (packetBuffer[0]=='0')
  {
    Serial.println("HIGH"); 
   lcd.setCursor(0,7);
   lcd.print("High");
  digitalWrite(buzzer,HIGH);
  }
   else if (packetBuffer[0]=='1'){
  Serial.println("MEDIUM");
  lcd.setCursor(0,7);
   lcd.print("Medium");
   }
  else if(packetBuffer[0]=='2'){
   Serial.println("LOW");   
  lcd.setCursor(0,7);
   lcd.print("Low");
  }
  else if(packetBuffer[0]=='3')
 {
  Serial.println("EXHAUSTED");   
  lcd.setCursor(0,7);
   lcd.print("Exhausted");
 } 
   
  delay(1000);
  
    packetBuffer[0]='x';

    udp.parsePacket();
  udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
  }
