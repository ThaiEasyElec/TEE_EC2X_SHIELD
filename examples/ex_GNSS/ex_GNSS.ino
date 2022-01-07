#include "EC2X.h"
#include "EC2X_GNSS.h"
#include <SoftwareSerial.h>
SoftwareSerial EC25_SER(8, 9); // RX, TX
#define POWER_PIN 4
EC2X_GNSS gps;
unsigned long previousMillis = 0; 
const long interval = 1000;

void setup() 
{
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

  Serial.print(F("turnOn GNSS: "));
  Serial.println(gps.turnOn()?"OK":"Fail");
 
 
}
void loop() 
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {   
    if(gps.fixed())
    {
       Serial.print(F("GNSS Data: "));
       Serial.println(gps.getData_index(ALL));
       Serial.print(F("UTC time: "));
       Serial.println(gps.getData_index(TIME));
       Serial.print(F("Latitude: "));
       Serial.println(gps.getData_index(LATITUDE));
       Serial.print(F("Longitude: "));
       Serial.println(gps.getData_index(LONGTITUDE)); 
       Serial.print(F("Horizontal precision: "));
       Serial.println(gps.getData_index(HDOP)); 
       Serial.print(F("altitude: "));
       Serial.println(gps.getData_index(ALTITUDE)+ " m"); 
       Serial.print(F("GNSS positioning mode: "));
       Serial.println(gps.getData_index(FIX)+ "D"); 
       Serial.print(F("HEADING: "));
       Serial.println(gps.getData_index(HEADING)+ " degree"); 
       Serial.print(F("SPEED: "));
       Serial.println(gps.getData_index(SPEED_Knots)+ " knots"); 
       Serial.print(F("SPEED: "));
       Serial.println(gps.getData_index(SPEED_Kmh)+ " Km/h"); 
       Serial.print(F("Date when fixing position: "));
       Serial.println(gps.getData_index(DATE));
       Serial.print(F("Number of satellites: "));
       Serial.println(gps.getData_index(NSAT));  
    } 
    else
    {
      Serial.println(F("GNSS not Fix"));
    }
    previousMillis = currentMillis;
  }
}
