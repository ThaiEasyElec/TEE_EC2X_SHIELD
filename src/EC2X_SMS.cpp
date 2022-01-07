#include "EC2X_SMS.h"


EC2X_SMS::EC2X_SMS()
{
	
}
bool EC2X_SMS::messageService(bool service)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CSMS="));
	lte.ser.print(service);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(F("+CSMS::"),5000);
	if(res==RES_MATCH)
	{
		lte.ser.flushSerial();
		return true;
	}
		lte.ser.flushSerial();
	return false;
}
bool EC2X_SMS::messageFormat(bool format)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CMGF="));
	lte.ser.print(format);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		lte.ser.flushSerial();
		return true;
	}
		lte.ser.flushSerial();
	return false;
	
}
String EC2X_SMS::serviceCenterAddress()
{
	//+CSCA: "+66923010900",145
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CSCA?\r"));
	uint8_t  res = lte.ser.waitData(F("+CSCA:"),5000);
	if(res==RES_MATCH)
	{
		String dat = lte.ser.getData_index(1);
		dat.replace("\"","");
		lte.ser.flushSerial();
		return dat;
	}
		lte.ser.flushSerial();
	return "Error";
	
}
bool EC2X_SMS::messageStorage(char *mem)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CPMS"));
	lte.ser.print(mem);
	lte.ser.print(F("\r"));
	
	uint8_t  res = lte.ser.waitData(F("+CPMS:"),5000);
	if(res==RES_MATCH)
	{
		lte.ser.flushSerial();
		return true;
	}
	lte.ser.flushSerial();
	return false;
}
bool EC2X_SMS::deleteMessage(int index,uint8_t delflag)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CMGD="));
	lte.ser.print(index);
	lte.ser.print(F(","));
	lte.ser.print(delflag);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		lte.ser.flushSerial();
		return true;
	}
		lte.ser.flushSerial();
	return false;
}
void EC2X_SMS::readMessage(int index)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CMGR="));
	lte.ser.print(index);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(F("+CMGR:"),10000);
	//Serial.println(res);
	if(res==RES_MATCH)
	{
		delay(5);
		//Serial.println(lte.ser.resData);
		//lte.ser.read();
		lte.ser.read();
		String str ="";
		while(lte.ser.available())
		{
			str += lte.ser.readStringUntil('\r');
			delay(10);
		}
		convetUnicodeToUTF8(str.c_str());

		lte.ser.flushSerial();
		return true;
	}
		lte.ser.flushSerial();
	return false;
	
}
int EC2X_SMS::convetUnicodeToUTF8(char *data)
{
  char buf[4];
  char * pEnd;
 for(size_t i=0;i<strlen(data);i+=2)
 {
    memset(buf,0,4);
    memcpy(buf,data+i,2);
    int x = strtol (buf,&pEnd,16); 
    if(x==0x0E)
    {
        i+=2;
        memset(buf,0,4);
        memcpy(buf,data+i,2);
        x = strtol (buf,&pEnd,16)&0xFF;
        Serial.write(0xE0);
        if(x<=0x3F)
        {
          Serial.write(0xB8);
          Serial.write(0x80+x);
        }
        else
        {
          Serial.write(0xB9);
          Serial.write(0x40+x);
        }
    }  
    else
      Serial.write(x);
  }
}




