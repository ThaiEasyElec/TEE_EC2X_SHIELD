#include "EC2X_File.h"

EC2X_FILE::EC2X_FILE()
{
	
}
unsigned long EC2X_FILE::QFLDS(char *path,bool m) //0 =free size,1=total size
{
	unsigned long s=0;
	lte.ser.print(F("AT+QFLDS=\""));
	lte.ser.print(path);
	lte.ser.print(F("\"\r"));
	uint8_t  res = lte.ser.waitData(F("+QFLDS:"),10000);
	if(res == RES_MATCH)
	{
		//+QFLDS: 2606080,2616320
		//Serial.println(res.data);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
			
		if(m==1)
		{
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
			s= atol(lte.ser.resLine);
		}
		else
		{
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			s= atol(lte.ser.resLine);
		}
		lte.ser.waitData(WAIT_OK,2000);
		lte.ser.flushSerial();
	}
	return s;
}
bool EC2X_FILE::uploadFile(char *path,uint8_t *data,size_t size)
{
	lte.ser.print(F("AT+QFUPL=\""));
	lte.ser.print(path);
	lte.ser.print(F("\","));
	lte.ser.print(size);
	lte.ser.print(F("\r"));
	
	
	uint8_t  res = lte.ser.waitData(F("CONNECT"),7000);
	
	if(res == RES_MATCH)
	{
		//Serial.println(res.data);
		lte.ser.write(data,size);
		res = lte.ser.waitData(F("+QFUPL:"),5000);
		if(res == RES_MATCH)
		{
			int len,sum;
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			len = atoi(lte.ser.resLine);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
			sum = atoi(lte.ser.resLine);
			
			lte.ser.waitData(WAIT_OK,500);
			if(len == size)
				return true;
		}
	}
	else if(res==RES_ERR)
	{
		Serial.println(lte.ser.resData);
		return false;
	}
	return false;
}
void EC2X_FILE:: list(char *path)
{
	lte.ser.print(F("AT+QFLST=\""));
	lte.ser.print(path);
	lte.ser.print(F("\"\r"));
	bool flag=true;
	while(flag)
	{
		uint8_t  res = lte.ser.waitData(F("+QFLST:"),1000);
		if(res == RES_MATCH)
		{
			char buf[50];
			//Serial.println(res.data);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
			Serial.println(lte.ser.resLine);
		}
		else
		{
			
			return;
		}
	}
}
int EC2X_FILE::size(char *path)
{
	lte.ser.print(F("AT+QFLST=\""));
	lte.ser.print(path);
	lte.ser.print(F("\"\r"));
	
	uint8_t  res = lte.ser.waitData(F("+QFLST:"),7000);
	if(res == RES_MATCH)
	{
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',',true);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
		size_t s = atoi(lte.ser.resLine);
		lte.ser.waitData(WAIT_OK,500);
		return s;
	}
	if(res == RES_ERR)
	{
		return -1;
	}
	return 0;
}
bool EC2X_FILE::exists(char* path)
{
	if(size(path)!=-1)
		return true;
	return false;	
}
uint16_t EC2X_FILE::open(char *path,uint8_t mode)
{
	//+QFOPEN: 3000
	//OK
	// 0 = Create,open,R/W
	// 1 = Create,overWrite,R/W
	// 2 = readOnly RO
	lte.ser.print(F("AT+QFOPEN=\""));
	lte.ser.print(path);
	lte.ser.print(F("\","));
	lte.ser.print(mode);
	lte.ser.print(F("\r"));
	
	/*Serial.print(F("AT+QFOPEN=\""));
	Serial.print(path);
	Serial.print(F("\","));
	Serial.print(mode);
	Serial.print(F("\r"));
	*/
	uint8_t  res = lte.ser.waitData(F("+QFOPEN:"),7000);
	if(res == RES_MATCH)
	{
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
		size_t s = atoi(lte.ser.resLine);
		lte.ser.waitData(WAIT_OK,500);
		return s;
	}	
	Serial.println(lte.ser.resData);
	return 0;
}
bool EC2X_FILE::close(uint32_t handle)
{
	lte.ser.print(F("AT+QFCLOSE="));
	lte.ser.print(handle);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,7000);
	if(res == RES_OK)
	{
		return true;
	}
	Serial.println(lte.ser.resData);
	return false;
}
size_t  EC2X_FILE::read(uint32_t handle,uint8_t *data,size_t len)
{
	lte.ser.print(F("AT+QFREAD="));
	lte.ser.print(handle);
	lte.ser.print(F(","));
	lte.ser.print(len);
	lte.ser.print(F("\r"));
	
	uint8_t  res = lte.ser.waitData(F("CONNECT"),7000);
	if(res == RES_MATCH)
	{
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
		size_t s1 = atoi(lte.ser.resLine);
		while(!lte.ser.available()){delay(2);}
		lte.ser.read();
		size_t s = lte.ser.readBytes(data,s1);
		lte.ser.waitData(WAIT_OK,500);
		return s;
	}
	else
	{
		//Serial.println(res.temp);
	}
	return 0;	
}
size_t  EC2X_FILE::write(uint32_t handle,uint8_t *data,size_t len,uint8_t tout)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QFWRITE="));
	lte.ser.print(handle);
	lte.ser.print(F(","));
	lte.ser.print(len);
	lte.ser.print(F(","));
	lte.ser.print(tout);
	lte.ser.print(F("\r"));
	
	//CONNECT
	//stream data
	//+QFWRITE: 3,34 // writeLen,total_len
	//OK
	uint8_t  res = lte.ser.waitData(F("CONNECT"),7000);
	if(res == RES_MATCH)
	{
		lte.ser.write(data,len);
		res = lte.ser.waitData(F("+QFWRITE:"),7000);
		if(res == RES_MATCH)
		{
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			size_t s = atoi(lte.ser.resLine);
			
			lte.ser.waitData(WAIT_OK,500);
			return s;
			
			/*int index = res.data.indexOf(" ");
			int index2 = res.data.indexOf(",");
			size_t s = res.data.substring(index+1,index2).toInt();
			lte.ser.waitData(WAIT_OK,500);
			return s;
			*/
		}
	}
	return 0;
}
bool EC2X_FILE::seek(uint32_t handle,size_t offset,uint8_t position)
{
	lte.ser.print(F("AT+QFSEEK="));
	lte.ser.print(handle);
	lte.ser.print(F(","));
	lte.ser.print(offset);
	lte.ser.print(F(","));
	lte.ser.print(position);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,7000);
	if(res==RES_OK)
	{
		return true;
	}

	return false;
}
size_t EC2X_FILE::position(uint32_t handle)
{
	lte.ser.print(F("AT+QFPOSITION="));
	lte.ser.print(handle);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(F("+QFPOSITION:"),7000);
	if(res ==RES_MATCH)
	{
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
		size_t s = atoi(lte.ser.resLine);
		lte.ser.waitData(WAIT_OK,500);
		return s;
	}
	
	return -1;
}	
bool EC2X_FILE::truncate(uint32_t handle)
{
	lte.ser.print(F("AT+QFTUCAT="));
	lte.ser.print(handle);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,7000);
	if(res==RES_OK)
	{
		return true;
	}
	
	return false;
}
bool EC2X_FILE::move(char *source,char * destination,bool copy,bool overwrite)
{
	lte.ser.print(F("AT+QFMOV=\""));
	lte.ser.print(source);
	lte.ser.print("\",\"");
	lte.ser.print(destination);
	lte.ser.print("\",");
	lte.ser.print(copy);
	lte.ser.print(",");
	lte.ser.print(overwrite);
	lte.ser.print(F("\r"));
	String resStr[2]={F("OK"),F("+CME ERROR:")};
	uint8_t  res = lte.ser.waitData(WAIT_OK,7000);
	if(res==RES_OK)
	{
		return true;
	}
	
	return false;
	
}
bool EC2X_FILE::del(char* path)
{
	lte.ser.print(F("AT+QFDEL=\""));
	lte.ser.print(path);
	lte.ser.print(F("\""));
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,7000);
	if(res==RES_OK)
	{
		return true;
	}
	
	return false;
}


















