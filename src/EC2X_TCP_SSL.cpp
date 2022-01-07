#include "EC2X_TCP_SSL.h"

EC2X_TCP_SSL::EC2X_TCP_SSL(uint8_t connectID)
{
	if((connectID>0)&&(connectID<=5))
	{
		connectID_ = connectID;
	}
}
bool EC2X_TCP_SSL :: open(String address,int port,uint8_t mode,uint8_t SSL_id)
{
	//AT+QSSLOPEN=<pdpctxID>,<sslctxID>,<clientID>,<serveraddr>,<server_port>[,<access_mode>]
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QSSLOPEN="));
	lte.ser.print(TCPS_CONTEXT);
	lte.ser.print(F(","));
	lte.ser.print(SSL_id);
	lte.ser.print(F(","));
	lte.ser.print(connectID_);
	lte.ser.print(F(",\""));
	lte.ser.print(address);
	lte.ser.print(F("\","));
	lte.ser.print(port);
	lte.ser.print(F(","));
	lte.ser.print(mode);
	lte.ser.print(F("\r"));
	if(mode<2)
	{
	  uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
		if(res == RES_OK)
		{
			//+QSSLOPEN: 1,0
			res = lte.ser.waitData(F("+QSSLOPEN:"),10000);
			if(res == RES_MATCH)
			{
				lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
				lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
				int conid,err;
				conid = atoi(lte.ser.resLine);
				lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
				err = atoi(lte.ser.resLine);
				if((conid==connectID_)&&(err==0))
				{
					lte.ser.flushSerial();
					return true;
				}
				else if(err == 563)
				{
					close();
				}
				else
				{
					Serial.print(F("ERROR: "));
					Serial.println(lte.ser.resData);
				}
			}
			else
			{
				//Serial.println(res.temp);
			}
		}
		else
		{
			//Serial.println(res.temp);
		}
	}
	/*Transparent Mode 2*/
	if(mode==2)
	{
		uint8_t  res = lte.ser.waitData(F("CONNECT"),5000);
		if(res == RES_MATCH)
		{
			return true;
		}
	}
	return false;
}
bool EC2X_TCP_SSL::close()
{
	lte.ser.print(F("AT+QSSLCLOSE="));
	lte.ser.print(connectID_);
	lte.ser.print("\r");
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res == RES_OK)
	{
		return true;
	}
return false;
}
bool EC2X_TCP_SSL::connected()
{
	bool  connected_ =false;

	//AT+QISTATE=1,0
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QSSLSTATE="));
	lte.ser.print(connectID_);
	lte.ser.print("\r");
	
	uint8_t  res = lte.ser.waitData(F("+QSSLSTATE:"),2000);
	//Serial.println(res.data);
	if(res == RES_MATCH)
	{
		//Serial.println(res.data);
		//+QISTATE: 0,"TCP","178.128.80.51",1234,8412,2,1,0,0,"uart1"
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',',true);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
		if(atoi(lte.ser.resLine)==2)
			connected_ =true;	
		lte.ser.waitData(WAIT_OK,2000);
	}
	lte.ser.flushSerial();
	return connected_ ;
}
bool EC2X_TCP_SSL:: startSend(unsigned long tout)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QSSLSEND="));
	lte.ser.print(connectID_);
	lte.ser.print("\r");
	unsigned long pv = millis();
	do{
		if(lte.ser.available())
		{
			if(lte.ser.read()=='>')
			{
				return true;
			}
		}
	}
	while((millis()-pv)<tout);
	return false;
}
bool EC2X_TCP_SSL:: stopSend()
{
	lte.ser.write(0x1A);
	uint8_t res = lte.ser.waitData(F("SEND OK"),1000);
	if(res == RES_MATCH)
		return true;
	return false;
}
size_t EC2X_TCP_SSL:: send(uint8_t *data,size_t len,unsigned long tout)
{
	
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QSSLSEND="));
	lte.ser.print(connectID_);
	lte.ser.print(",");
	lte.ser.print(len);
	lte.ser.print("\r");
	unsigned long pv = millis();
	do
	{
		if(lte.ser.available())
		{
			int c = lte.ser.read();
			//Serial.write(c);
			if(c =='>')
			{
				size_t s = lte.ser.write(data,len);
				uint8_t  res = lte.ser.waitData(F("SEND OK"),500);
				if(res == RES_MATCH)
				{
					lte.ser.flushSerial();
					return s;
				}
			}
			else
			{
				//Serial.write(c);
			}
		}
	}
		while((millis()-pv)<tout);
	return 0;
}
size_t EC2X_TCP_SSL::getRxSize()
{
	size_t s;
	
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QSSLRECV="));
	lte.ser.print(connectID_);
	lte.ser.print(",0\r");
	uint8_t  res = lte.ser.waitData(F("+QSSLRECV:"),500);
	if(res == RES_MATCH)
	{
		//Serial.println(res.data);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',',true);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
		s = atoi(lte.ser.resLine);
		lte.ser.waitData(WAIT_OK,2000);
		lte.ser.flushSerial();
		return s;
	}
	lte.ser.flushSerial();
	return 0;
}
size_t EC2X_TCP_SSL::readRxBuffer(uint8_t *data,size_t size)
{
	for(uint8_t i=0;i<5;i++)
	{	
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QSSLRECV="));
	lte.ser.print(connectID_);
	lte.ser.print(",");
	lte.ser.print(size);
	lte.ser.print("\r");
	uint8_t res = lte.ser.waitData(F("+QSSLRECV:"),1000);
	if(res == RES_MATCH)
	{
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
		int s  = atoi(lte.ser.resLine);
		//Serial.println(res.data);
		lte.ser.read();
		int num = lte.ser.readBytes(data,s);
		lte.ser.waitData(WAIT_OK,2000);
		lte.ser.flushSerial();
		return (num);
	}
	else
	{
		//Serial.println(res.temp);
		delay(200);
	}
	}	
	return 0;
}






