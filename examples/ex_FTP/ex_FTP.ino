#include "EC2X.h"
#include "EC2X_FTP.h"
#include <SoftwareSerial.h>

SoftwareSerial EC25_SER(8, 9); // RX, TX
#define POWER_PIN 4


#define USER   "myuser"
#define PASS   "mypassword"
#define SERVER "http://myserver.com"
#define PORT    21 


LTE_FTP ftp;

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
  testFTP();
}

void loop()
{
  if (Serial.available())
    EC25_SER.write(Serial.read());
  if (EC25_SER.available())
    Serial.write(EC25_SER.read());
}
void testFTP(void)
{
  Serial.println(F("Test FTP"));
  Serial.print("Setting File Type: ");
  Serial.println(ftp.fileType(BINARY)?F("OK"):F("Fail"));
  Serial.print("Setting Transfer Mode: ");
  Serial.println(ftp.transMode(PASSIVE)?F("OK"):F("Fail"));
  Serial.print(F("OPEN FTP: "));
  bool ret = ftp.open(USER,PASS,SERVER,PORT);
  Serial.println(ret?"OK":"Fail");
  if(!ret)
    return;
  Serial.print(F("Get the Current Directory: "));
  String homepath = ftp.pwd();
  Serial.println(homepath);

  Serial.println(F("List File: "));
  listFile(homepath);
  
  Serial.print(F("Make Directory: "));
  String directorypath = homepath+"/testdir";
  Serial.println(ftp.mkdir(directorypath)?F("OK"):F("Fail"));

  Serial.println(F("List File: "));
  listFile(homepath);
  
  Serial.print(F("Change Directory to "));
  Serial.print(directorypath+": ");
  Serial.println(ftp.cd(directorypath)?F("OK"):F("Fail"));

  Serial.print(F("Put File : "));
  String filename = "testFile.txt";
  String str = "Test upload data from 4G to FTP Server";
  if(ftp.putFromBuffer(filename,0,str.c_str(),str.length(),true))
    Serial.println("Write OK");
  else
    Serial.println("Write Fail");

  Serial.println(F("Read File : "));
  readFile(filename);
  Serial.println();

  Serial.println(F("Append File : "));
  size_t fileSize = ftp.size(filename);
  str = "\r\n\r\n<<<< Test Append File >>>>\r\n";
  str += "IMSI: " +lte.IMSI() + "\r\n";
  str += "IMEI: " +lte.IMEI() + "\r\n";
  str += "ICCID: " +lte.ICCID() + "\r\n";
  if(ftp.putFromBuffer(filename,fileSize,str.c_str(),str.length(),true))
    Serial.println("Write OK");
  else
    Serial.println("Write Fail");

  Serial.println(F("Read File : "));
  readFile(filename);
  Serial.println();

  Serial.print(F("Rename File : "));
  String newfilename = "new_testFile.txt";
  Serial.println(ftp.rename(filename,newfilename)?F("OK"):F("Fail"));

  Serial.println(F("List File: "));
  listFile(directorypath);

  Serial.print(F("Get lasttime modify: "));
  Serial.println(ftp.modify(newfilename));

  Serial.print(F("Delete File: "));
  Serial.println(ftp.del(newfilename)?F("OK"):F("Fail"));
  
  Serial.print(F("Delete Folder: "));
  Serial.println(ftp.deldir(directorypath)?F("OK"):F("Fail"));

  Serial.print(F("logout: "));
  Serial.println(ftp.logout()?F("OK"):F("Fail"));
   
}

void listFile(String& path)
{
  if( ftp.ls(path,FTP_TEMP))
  {
    uint32_t handle = ftp.file.open(FTP_TEMP);
    if (handle)
    {
      size_t s;
      uint8_t buffer[100];
      while ( s = ftp.file.read(handle, buffer, 100))
      {
        Serial.write(buffer, s);
      }
      ftp.file.close(handle);
    }
  }
}
void readFile(String path)
{
  if(ftp.get(path,FTP_TEMP)>0)
  {
     uint32_t handle = ftp.file.open(FTP_TEMP);
    if (handle)
    {
      size_t s;
      uint8_t buffer[100];
      while ( s = ftp.file.read(handle, buffer, 100))
      {
        Serial.write(buffer, s);
      }
      ftp.file.close(handle);
    }
  }
}
