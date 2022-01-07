#include "EC2X.h"
#include <SoftwareSerial.h>
#include "EC2X_MQTT.h"

SoftwareSerial EC25_SER(8, 9); // RX, TX
#define POWER_PIN 4
#define CONNECT_ID  1
#define SSL_ID      1
EC2X_MQTT mqtt(CONNECT_ID);

#define MQTT_SERVER   "xxxxxxx.com"
#define MQTT_PORT     1883
#define MQTT_DEVICE   "EC25"
#define MQTT_USER     "your user"
#define MQTT_PASSWORD "your pass"

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

  lte.ssl.sslversion(SSL_ID, ALL);
  lte.ssl.ciphersuite(SSL_ID, "0XFFFF");
  lte.ssl.seclevel(SSL_ID, No_AUTH);

  mqtt.init();
  mqtt.ssl(true,SSL_ID);
  mqtt.callback = mqttCallBack;
  mqttConnect();

}
void mqttConnect(void)
{
  if(lte.internetStatus()) // check internet connected?
  {
    Serial.print(F("MQTT Connect Server: "));
    uint8_t ret = mqtt.open(MQTT_SERVER,MQTT_PORT);
    if(ret)
    {
      Serial.println(F("OK"));
      Serial.print(F("MQTT login: "));
      ret = mqtt.connect(MQTT_DEVICE,MQTT_USER,MQTT_PASSWORD);
      if(ret)
      {
         Serial.println(F("OK"));
          mqttSubScribe();
      }
      else
      {
         Serial.println(F("Fail"));
      }
    }
    else
      Serial.println(F("Fail"));
  }
  else
  {
    Serial.println(F("Internet Fail!!!"));
    Serial.print(F("ConnectInternet: "));
    Serial.println(lte.connectInternet() ? "OK" : "Fail");
    Serial.print(F("IP Address: "));
    Serial.println(lte.IPAddress());
  }
}
void mqttSubScribe(void)
{
    String subTopic_1 = "LED0";
    Serial.print(F("MQTT Topic: "));
    Serial.println(subTopic_1);
    Serial.print(F("MQTT single sub: "));
    Serial.println(mqtt.sub(subTopic_1,0) ? "OK" : "Fail");

    String subTopic_2 = "\"LED1\",0,\"LED2\",0,\"LED3\",0,\"LED4\",0";
    Serial.print(F("MQTT Topic:"));
    Serial.println(subTopic_2);
    Serial.print(F("MQTT multi sub: "));
    Serial.println(mqtt.subs(subTopic_2.c_str()) ? "OK" : "Fail");
}
void loop()
{

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) 
  {
    if(mqtt.connected())
     {
        if(mqtt.pub("Hello","1234567890"))
          Serial.println("pub: OK"); 
        else
          Serial.println("pub: Fail");
     }
    else
      mqttConnect();
    previousMillis = millis();
  }
  mqtt.mqttTask();
}
void mqttCallBack(String topic ,uint8_t *payload,size_t length)
{
   Serial.println(F("<########################"));
   Serial.println("Topic: "+topic);
   Serial.print("Payload: ");
   Serial.write(payload,length);
   Serial.println();
   Serial.println("Length: "+String(length) + "bytes");
   Serial.println(F("########################>"));
}
