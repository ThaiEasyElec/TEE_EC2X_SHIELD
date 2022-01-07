#include "EC2X_GNSS.h"

const uint8_t daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
EC2X_GNSS::EC2X_GNSS()
{
	
}
bool EC2X_GNSS::turnOn()
{
	lte.ser.print(F("AT+QGPS="));
	lte.ser.print(1);
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
bool EC2X_GNSS::turnOn(uint8_t mode,int fixtime,int fixdist,int fixcnt,int fixrate)
{
	lte.ser.print(F("AT+QGPS="));
	lte.ser.print(mode);
	lte.ser.print(F(","));
	lte.ser.print(fixtime);
	lte.ser.print(F(","));
	lte.ser.print(fixdist);
	lte.ser.print(F(","));
	lte.ser.print(fixcnt);
	lte.ser.print(F(","));
	lte.ser.print(fixrate);
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
bool EC2X_GNSS::turnOff()
{
	lte.ser.print(F("AT+QGPSEND\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		lte.ser.flushSerial();
		return true;
	}
	lte.ser.flushSerial();
	return false;	
}
String EC2X_GNSS::position()
{
	String lawData;
	lte.ser.print(F("AT+QGPSLOC="));
	lte.ser.print(2);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(F("+QGPSLOC:"),5000);
	if(res==RES_MATCH)
	{
		lawData = String(lte.ser.resData);
		lte.ser.waitData(WAIT_OK,5000);
		lte.ser.flushSerial();
	}
	else
	{
		lawData = F("Not fixed now");
	}
	lte.ser.flushSerial();
	return lawData;	
}
bool EC2X_GNSS::fixed(void)
{
	String str = position();
	if(str.indexOf(F("Not fixed now"))!=-1)
	{
		lawData = str;
		return false;
	}
	lawData = str;
	//Serial.println(lawData);
	return true;
}

String EC2X_GNSS::getData_index(uint8_t num)
{
		//gpsResp resp;
		if(num==0)
			return lawData;
	    size_t i=0;
		while(lawData[i++]!=' '){}
		char buf[20];
		uint8_t cnt;
		for(uint8_t x=0;x<num;x++)
		{
			cnt=0;
			memset(buf,0,20);
			while(lawData[i]!=',' &&lawData[i]!=0){buf[cnt++]=lawData[i]; i++;}
			//Serial.println(buf);
			i++;
			
		}
		

	return String(buf);
}




































