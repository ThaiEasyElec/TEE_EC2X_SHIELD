#include "EC2X.h"
#include "EC2X_UDP.h"
#include <SoftwareSerial.h>

SoftwareSerial EC25_SER(8, 9); // RX, TX
#define POWER_PIN 4

#define CONNECT_ID  1
#define SERVER "xxx.xxx.xxx"
#define PORT 8888

EC2X_UDP udp(CONNECT_ID);

unsigned long previousMillis = 0;   
const long interval = 1000;  

void setup() {
  EC25_SER.begin(9600);
  Serial.begin(9600);
  Serial.println(F("Hello EC25 LTE 4G Module"));
  lte.begin(&EC25_SER, POWER_PIN);
  Serial.println(F("Reset Module"));
  lte.reset();
  Serial.print(F("Wait Module Ready"));
  while (!lte.ready())
  {
    Serial.print(F("."));
    delay(500);
  }
  Serial.println();
  Serial.print(F("SIM Status:"));
  Serial.println(lte.simStatus() ? "OK" : "Fail");
  Serial.print(F("IMSI:"));
  Serial.println(lte.IMSI());
  Serial.print(F("IMEI:"));
  Serial.println(lte.IMEI());
  Serial.print(F("ICCID:"));
  Serial.println(lte.ICCID());
  Serial.print(F("Register Network:"));
  while (!lte.networkRegister())
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println(F("OK"));
  Serial.print(F("Operator:"));
  Serial.println(lte.getOperator());
  Serial.print(F("Signal:"));
  Serial.println(lte.signal());
  Serial.print(F("networkInfo:"));
  Serial.println(lte.networkInfo());
  Serial.print(F("Connect Internet:"));
  lte.NET_APN = F("Internet");
  lte.NET_USERNAME = F("");
  lte.NET_PASSWORD = F("");
  Serial.println(lte.connectInternet() ? F("Success") : F("Fail"));
  Serial.print(F("IP Address:"));
  Serial.println(lte.IPAddress());
  connectServer();
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    //udp.send("Hello",5);
    Serial.println(F("Send Data"));
    String data = "Hello World !!! test send UDP";
    udp.println(data);
    previousMillis = millis(); 
  }
  if(udp.getRxSize())
  {
    Serial.print(F("UDP Receive: "));
    uint8_t buf[100];
    size_t s = udp.readRxBuffer(buf,100);
    Serial.write(buf,s);
    Serial.println();
   
  }
}

void connectServer()
{
  Serial.print(F("Connect Server: "));
  Serial.println(udp.open(SERVER,PORT)?"OK":"Fail");
}
