#include <SoftwareSerial.h>
#include "EC2X.h"
#include "EC2X_TCP.h"

SoftwareSerial EC25_SER(8, 9); // RX, TX
#define POWER_PIN 4
#define CON_ID 1
#define SERVER "178.128.80.51"
#define PORT 8111
EC2X_TCP client(CON_ID);

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
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    if (!client.connected())
    {
      Serial.println(F("Open TCP:"));
      if (client.open(SERVER, PORT, 0))
      {
        Serial.println(F("Success"));
      }
      else
        Serial.println(F("Fail"));
    }
    else
    {
      Serial.print(F("Send TCP:"));
      String str = "Test send TCP from EC25";
      if (client.send(str.c_str(), str.length()))
        Serial.println(F("Success"));
      else
        Serial.println(F("Fail"));
    }
    previousMillis = millis();
  }

  if (client.connected())
  {
    if (client.getRxSize())
    {
      Serial.print(F("Receive TCP:"));
      size_t s = client.getRxSize();
      uint8_t data[s];
      client.readRxBuffer(data, s);
      Serial.write(data, s);
      Serial.println();
    }
  }
}
