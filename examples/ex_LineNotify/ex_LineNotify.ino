#include "EC2X.h"
#include "EC2X_LineNotify.h"
#include <SoftwareSerial.h>

SoftwareSerial EC25_SER(8, 9); // RX, TX
#define POWER_PIN 4

#define CONNECT_ID  1
#define SSL_ID      1

EC2X_LINE_NOTIFY line(CONNECT_ID);
#define LINE_TOKEN  "xxxxxxxxxxxxxxxxxxxxxxxx"

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

  lte.ssl.sslversion(SSL_ID, ALL);
  lte.ssl.ciphersuite(SSL_ID, "0XFFFF");
  lte.ssl.seclevel(SSL_ID, No_AUTH);
  line.setSSL(SSL_ID);
  line.callback = lineCallback;
  Serial.println("Send Message and Stricker");
  String strLine = F("ทดสอบส่งข้อความเข้า Line Notify ด้วยโมดูล 4G\r\n");
         strLine += F("Operator: ");
         strLine += lte.getOperator()+"\r\n";
         strLine += F("Signal: ");
         strLine += String(lte.signal())+"\r\n";
         strLine += F("IP Address: ");
         strLine += lte.IPAddress();    
  line.send(LINE_TOKEN,strLine,8522,16581276); // stickerPacket and stickerID --> https://developers.line.biz/en/docs/messaging-api/sticker-list/#specify-sticker-in-message-object
  
  Serial.println("Send Message");
  line.send(LINE_TOKEN,"ทดสอบส่งข้อความ");
}
void lineCallback(uint8_t *data,size_t len)
{
  Serial.write(data,len);
}
void loop()
{
  
}
