
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char *ssid = "TANK MONITOR";
const char *pass = "password"; 

unsigned int localPort = 2000; // local port to listen for udp packets

IPAddress ServerIP(192,168,4,1);
IPAddress ClientIP(192,168,4,2);

// A udp instance to let us send and receive packets over udp
WiFiUDP udp;

int sensor_l=14;
int sensor_m=12;
int sensor_h=13;
char packetBuffer[9];   //Where we get the udp data
//======================================================================
//                Setup
//======================================================================
void setup()
{
    Serial.begin(9600);
    Serial.println();
    pinMode(sensor_l,INPUT);
    pinMode(sensor_m,INPUT);
    pinMode(sensor_h,INPUT);
    WiFi.begin(ssid, pass);   //Connect to access point
    
        Serial.println("");

  // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
    Serial.print(".");
    }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    
    //Start udp
    Serial.println("Starting udp");
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());
    
}
  
void loop() {

if(WiFi.status() == WL_CONNECTED){
int cb = udp.parsePacket();
    if (!cb) 
    {
      int val1=digitalRead(sensor_l);
      int val2=digitalRead(sensor_m);
      int val3=digitalRead(sensor_h);
      
      //If serial data is recived send it to UDP
      
        udp.beginPacket(ServerIP, 2000);  //Send Data to Master unit
        //Send UDP requests are to port 2000
        
        
        
        
        char a[2];
        int res=val1+val2+val3;
        a[0]=res+'0'; 
        
        udp.write(a,1); //Send one byte to ESP8266 
        udp.endPacket();
        Serial.println("Packet sent");
        delay(1000);
        
    }
    else {
      // We've received a UDP packet, send it to serial
      udp.read(packetBuffer, 1); // read the packet into the buffer, we are reading only one byte
      Serial.print(packetBuffer);
      delay(20);
    }
}
else
{
   WiFi.begin(ssid, pass);   //Connect to access point
  
    Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    
    //Start udp
    Serial.println("Starting udp");
    udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(udp.localPort());

}
  // Read button   
}
