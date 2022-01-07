#include "EC2X.h"
#include <SoftwareSerial.h>
#include "EC2X_HTTP.h"

SoftwareSerial EC25_SER(8, 9); // RX, TX
#define POWER_PIN 4
#define CONNECT_ID  1
#define SSL_ID      1
EC2X_HTTP http(CONNECT_ID);


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

  http.sslctxid(SSL_ID);
  httprequest();
}

void loop()
{
  if (Serial.available())
    EC25_SER.write(Serial.read());
  if (EC25_SER.available())
    Serial.write(EC25_SER.read());
}
void httprequest(void)
{
  int ret=0;
  Serial.println(F("Start http custom header"));
  
  //http get from --> "http://air4thai.pcd.go.th/services/getNewAQI_JSON.php?region=1"
  String url = F("http://air4thai.pcd.go.th");
  String urlpath = F("/services/getNewAQI_JSON.php?region=1");
  String host = F("air4thai.pcd.go.th");
  
  ret = http.setURL(url);
  if(ret)
   Serial.println(F("Set URL: OK"));
 else
    Serial.println(F("Set URL: Error"));
 
  String httpData = "GET ";
  httpData += urlpath;
  httpData += F(" HTTP/1.1\r\n");
  httpData += F("Host: ");
  httpData += host;
  httpData += F("\r\nAccept: */*\r\n");
  httpData += F("User-Agent: QUECTEL_MODULE\r\n");
  httpData += F("Connection: Keep-Alive\r\n");
  httpData += F("Content-Type: application/x-www-form-urlencoded\r\n");
  httpData += F("\r\n");
  ret = http.request(httpData);
  Serial.print(F("http status: "));
  Serial.println(ret);
  Serial.print(F("Save Http to Ram: "));
  if (http.saveHttpToRam())
  {
    Serial.println("OK");
    uint32_t handle = http.file.open(http.path);
    if (handle)
    {
      size_t s;
      uint8_t buffer[50];
      while ( s = http.file.read(handle, buffer, 50))
      {
        Serial.write(buffer, s);
      }
      http.file.close(handle);
    }
  }
  else
    Serial.println("Error");
}
