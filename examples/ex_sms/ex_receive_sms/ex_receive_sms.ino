#include "EC2X.h"
#include "EC2X_SMS.h"
#include <SoftwareSerial.h>

SoftwareSerial EC25_SER(8, 9); // RX, TX
#define POWER_PIN 4
#define PHONE_NUMBER "09xxxxxxxx"
EC2X_SMS sms;
int lastSMS_ID = -1;

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
  sms.init();
  lastSMS_ID = sms.lastMessageID();
  Serial.print(F("Last SMS ID: "));
  Serial.println(lastSMS_ID);

  /// Ex.sendSMS
  /*
    Serial.println(F("Send SMS..."));
    if(sms.startSend(PHONE_NUMBER))
    {
      sms.printUnicode(F("Hello I am 4G Shield.\r\n"));
      sms.printUnicode(F("สวัสดีนะ เราส่งข้อความมาจาก โมดูล EC25\r\n"));
      if(!sms.endSend())
        Serial.println(F("Send SMS Fail"));
    }
  */
  /// Ex.Delete SMS
  /*
    Serial.print(F("Delete SMS by ID: "));
    Serial.println(sms.deleteMessage(index)?F("OK"):F("Fail"));
  */
  /// Ex.Delete All SMS
  Serial.print(F("Delete All SMS: "));
  Serial.println(sms.deleteAllMessage() ? F("OK") : F("Fail"));
  lastSMS_ID = sms.lastMessageID();

}

void loop()
{
  if (lastSMS_ID != sms.lastMessageID())
  {
    int index = sms.lastMessageID();
    readSMS(index);
    lastSMS_ID = index;
  }
}
void readSMS(int index)
{
  Serial.print(F("Time: "));
  Serial.println(sms.getTime(index));
  Serial.print(F("Sender: "));
  Serial.println(sms.getSenderNumber(index));
  Serial.print(F("Message: "));
  Serial.println(sms.getMessage(index));

}
