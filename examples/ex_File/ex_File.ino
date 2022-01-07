#include "EC2X.h"
#include "EC2X_File.h"
#include <SoftwareSerial.h>

SoftwareSerial EC25_SER(8, 9); // RX, TX
#define POWER_PIN 4

EC2X_FILE file;

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
  /*Serial.print(F("SIM Status:"));
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
  */
  testFile();
}

void loop()
{
  if (Serial.available())
    EC25_SER.write(Serial.read());
  if (EC25_SER.available())
    Serial.write(EC25_SER.read());
}

void testFile(void)
{
  getSpaceInfo();

  Serial.println(F("##Upload Data to Mem"));
  String path = "RAM:test1.txt";
  String data = "\r\n\r\n<<<< Test Upload File >>>>\r\n";
  data += "IMSI: " + lte.IMSI() + "\r\n";
  data += "IMEI: " + lte.IMEI() + "\r\n";
  data += "ICCID: " + lte.ICCID() + "\r\n";
  //uploaDataToMem(path,data);
  writeFile(path, data);

  Serial.println(F("##List All File in RAM: "));
  file.list("RAM:*");

  Serial.print(F("##Get File Size: "));
  Serial.println(file.size(path.c_str()));

  Serial.print(F("##File exists :"));
  Serial.println(file.exists(path.c_str()) ? F("Found") : F("NotFound"));

  Serial.println(F("##Read File"));
  readFile(path);

  Serial.println(F("##Append File"));
  String appendData = "Operator: " + lte.getOperator() + "\r\n";
  appendData += "Signal: " + String(lte.signal()) + "\r\n";
  appendFile(path, appendData);

  Serial.println(F("##Read File"));
  readFile(path);

  Serial.print(F("##Copy File: "));
  String copy_to = "RAM:test2.txt";
  Serial.println(file.copy(path.c_str(), copy_to.c_str()) ? F("OK") : F("Fail"));

  Serial.println(F("##List All File in RAM: "));
  file.list("RAM:*");

  Serial.println(F("##Delete File"));
  file.del(copy_to.c_str());

  Serial.println(F("##List All File in RAM: "));
  file.list("RAM:*");

  Serial.println(F("##Delete All File"));
  file.del("RAM:*");

  Serial.println(F("##List All File in RAM: "));
  file.list("RAM:*");


}
void getSpaceInfo(void)
{
  Serial.println(F("##Get the Space Information of RAM (Bytes)"));
  Serial.print(F("Total Size: "));
  Serial.println(file.totalSize("RAM"));
  Serial.print(F("Free Size: "));
  Serial.println(file.freeSize("RAM"));

  Serial.println(F("##Get the Space Information of UFS (Bytes)"));
  Serial.print(F("Total Size: "));
  Serial.println(file.totalSize("UFS"));
  Serial.print(F("Free Size: "));
  Serial.println(file.freeSize("UFS"));
}
void uploaDataToMem(String& path, String& data)
{
  Serial.print(F("Upload Data: "));
  Serial.println(file.uploadFile(path.c_str(), data.c_str(), data.length()) ? "OK" : "Fail");
}
void writeFile(String path, String data)
{
  uint32_t handle = file.open(path.c_str(), open_RW);
  if (handle)
  {
    file.seek(handle, 0, BEGIN); // move pointer to end of file
    file.write(handle, data.c_str(), data.length());
    file.close(handle);
  }
}
void readFile(String path)
{
  uint32_t handle = file.open(path.c_str(), readOnly_RO);
  if (handle)
  {
    size_t s;
    uint8_t buffer[100];
    while ( s = file.read(handle, buffer, 100))
    {
      Serial.write(buffer, s);
    }
    file.close(handle);
  }
}
void appendFile(String path, String data)
{
  uint32_t handle = file.open(path.c_str(), open_RW);
  if (handle)
  {
    file.seek(handle, 0, END); // move pointer to end of file
    file.write(handle, data.c_str(), data.length());
    file.close(handle);
  }
}
