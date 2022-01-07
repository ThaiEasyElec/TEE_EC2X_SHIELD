#include "EC2X_FTP.h"



LTE_FTP::LTE_FTP()
{
	
}
bool LTE_FTP::open(char *user ,char *password,char *server,int port)
{
	lte.ser.print(F("AT+QFTPCFG=\"account\",\""));
	lte.ser.print(user);
	lte.ser.print(F("\",\""));
	lte.ser.print(password);
	lte.ser.print(F("\"\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		lte.ser.flushSerial();
		lte.ser.print(F("AT+QFTPOPEN=\""));
		lte.ser.print(server);
		lte.ser.print(F("\","));
		lte.ser.print(port);
		lte.ser.print(F("\r"));
		res = lte.ser.waitData(WAIT_OK,5000);
		if(res==RES_OK)
		{
			res = lte.ser.waitData(F("+QFTPOPEN"),5000);
			if(res==RES_MATCH)
			{
				if((getData_index(1).toInt()==0)&&(getData_index(2).toInt()==0))
				{
					lte.ser.flushSerial();
					return true;
				}
			}
			
		}
	}
	lte.ser.flushSerial();

	return false;
	
}
bool LTE_FTP::config(char *cmd ,uint8_t data)
{
	lte.ser.print(F("AT+QFTPCFG=\""));
	lte.ser.print(cmd);
	lte.ser.print(F("\","));
	lte.ser.print(data);
	lte.ser.print(F("\r"));
	
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		lte.ser.flushSerial();
		return true;
	}
	return false;
}

String LTE_FTP::getData_index(uint8_t num)
{
	    size_t i=0;
		while(lte.ser.resData[i++]!=' '){}
		char buf[20];
		uint8_t cnt;
		for(uint8_t x=0;x<num;x++)
		{
			cnt=0;
			memset(buf,0,20);
			while(lte.ser.resData[i]!=',' &&lte.ser.resData[i]!=0){buf[cnt++]=lte.ser.resData[i]; i++;}
			//Serial.println(buf);
			i++;
			
		}
	return String(buf);
}
bool LTE_FTP::cd(char *path)
{
	lte.ser.print(F("AT+QFTPCWD=\""));
	lte.ser.print(path);
	lte.ser.print(F("\"\r"));
	
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QFTPCWD:"),5000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0)&&(getData_index(2).toInt()==0))
			{
				lte.ser.flushSerial();
				return true;
			}
				
		}	
	}
	lte.ser.flushSerial();
	return false;
}

String LTE_FTP::pwd()
{
	lte.ser.print(F("AT+QFTPPWD\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QFTPPWD:"),5000);
		if(res==RES_MATCH)
		{
			lte.ser.flushSerial();
			return getData_index(2);
		}	
	}
	lte.ser.flushSerial();
	return "";
}
bool LTE_FTP:: mkdir(char *path)
{
	lte.ser.print(F("AT+QFTPMKDIR=\""));
	lte.ser.print(path);
	lte.ser.print(F("\"\r"));
	
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QFTPMKDIR:"),5000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0)&&(getData_index(2).toInt()==0))
			{
				lte.ser.flushSerial();
				return true;
			}
				
		}	
	}
	lte.ser.flushSerial();
	return false;
}
void LTE_FTP:: ls(char *path)
{
	lte.ser.print(F("AT+QFTPLIST=\""));
	lte.ser.print(path);
	lte.ser.print(F("\"\r"));
	uint8_t  res = lte.ser.waitData(F("CONNECT"),5000);
	if(res==RES_MATCH)
	{
		Serial.println();
		unsigned long pv = millis(); 
		unsigned long tout = 2000;
		do
		{
			while(lte.ser.available())
			{
				Serial.write(lte.ser.read());
				pv = millis(); 
				tout = 250;
			}
		}
		while(millis()-pv<tout);
	}
	lte.ser.flushSerial();

}
bool LTE_FTP:: ls(char *path,char *save)
{
	file.del(save);
	
	lte.ser.print(F("AT+QFTPLIST=\""));
	lte.ser.print(path);
	lte.ser.print(F("\",\""));
	lte.ser.print(save);
	lte.ser.print(F("\"\r"));
	
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QFTPLIST:"),5000);
		if(res==RES_MATCH)
		{
			lte.ser.flushSerial();
			return true;
		}
	}
	lte.ser.flushSerial();
	return false;
}
bool LTE_FTP::putFromBuffer(char* filename,size_t address,uint8_t *buf,size_t length,bool lastData)
{
	if(startPut(filename,"COM:",address,length,lastData))
	{
		lte.ser.write(buf,length);
		
		uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
		if(res==RES_OK)
		{
			res = lte.ser.waitData(F("+QFTPPUT"),5000);
			if(res==RES_MATCH)
			{
				lte.ser.flushSerial();
				return true;
			}	
		}
	}
	Serial.println("Start fail");
	lte.ser.flushSerial();
    return false;
}
bool LTE_FTP::putFromFile(char* filename,char *path)
{
	if( startPut(filename,path,0))
	{
		uint8_t res = lte.ser.waitData(F("+QFTPPUT"),5000);
		if(res==RES_MATCH)
		{
			lte.ser.flushSerial();
			return true;
		}	
	}
	lte.ser.flushSerial();
    return false;
}

bool LTE_FTP::startPut(char* filename,char *sourse,size_t address,size_t length,bool lastData)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QFTPPUT=\""));
	lte.ser.print(filename);
	lte.ser.print(F("\",\""));
	lte.ser.print(sourse);
	lte.ser.print(F("\","));
	lte.ser.print(address);
		
	if(length!=0)
	{
		lte.ser.print(F(","));
		lte.ser.print(length);
	}
	if(lastData!=0)
	{
		lte.ser.print(F(","));
		lte.ser.print(lastData);
	}
	lte.ser.print(F("\r"));
	uint8_t  res;
	if(sourse[0]=='C')
	{
		res = lte.ser.waitData(F("CONNECT"),5000);
		if(res==RES_MATCH)
		{
			lte.ser.flushSerial();
			return true;
		}
	
	}
	else
	{
		res = lte.ser.waitData(WAIT_OK,5000);
		if(res==RES_OK)
		{
			lte.ser.flushSerial();
			return true;
		}
	}
		
	
	lte.ser.flushSerial();
	return false;
} 
bool LTE_FTP::endPut()
{
	delay(1000);
	lte.ser.print(F("+++"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QFTPPUT"),5000);
		if(res==RES_MATCH)
		{
			lte.ser.flushSerial();
			return true;
		}
	}
	lte.ser.flushSerial();
	return false;
}
size_t LTE_FTP::size(char *path)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QFTPSIZE=\""));
	lte.ser.print(path);
	lte.ser.print(F("\"\r"));
	
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QFTPSIZE:"),5000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0))
			{
				lte.ser.flushSerial();
				return getData_index(2).toInt();
			}
			
			lte.ser.flushSerial();
			return true;
		}
	}
	lte.ser.flushSerial();
	return 0;
	
}
size_t LTE_FTP::get(char *path,char *saveTo,size_t address)
{
	file.del(saveTo);
	
	if(ftpget(path,saveTo,address,0))
	{
		//+QFTPGET: 0,257
		uint8_t res = lte.ser.waitData(F("+QFTPGET:"),5000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0))
			{
				lte.ser.flushSerial();
				return getData_index(2).toInt();
			}
			lte.ser.flushSerial();
			return true;
		}
	}
	return 0;
}
bool LTE_FTP::ftpget(char* filename,char *sourse,size_t address,size_t length)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QFTPGET=\""));
	lte.ser.print(filename);
	lte.ser.print(F("\",\""));
	lte.ser.print(sourse);
	lte.ser.print(F("\","));
	lte.ser.print(address);
	if(length!=0)
	{
		lte.ser.print(F(","));
		lte.ser.print(length);
	}
	lte.ser.print(F("\r"));
	
	uint8_t res;
	if(sourse[0]=='C')
	{
		res = lte.ser.waitData(F("CONNECT"),5000);
		if(res==RES_MATCH)
		{
			lte.ser.flushSerial();
			return true;
		}
	
	}
	else
	{
		res = lte.ser.waitData(WAIT_OK,5000);
		if(res==RES_OK)
		{
			lte.ser.flushSerial();
			return true;
		}
	}
		
	lte.ser.flushSerial();
	return false;
}
bool LTE_FTP::logout()
{
	lte.ser.print(F("AT+QFTPCLOSE\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		//lte.ser.flushSerial();
		res = lte.ser.waitData(F("+QFTPCLOSE:"),5000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0)&&(getData_index(2).toInt()==0))
			{
				lte.ser.flushSerial();
				return true;
			}
		}
		
		return true;
	}
	lte.ser.flushSerial();
	return false;
}
bool LTE_FTP::rename(char *oldName,char *newName)
{
	lte.ser.print(F("AT+QFTPRENAME=\""));
	lte.ser.print(oldName);
	lte.ser.print(F("\",\""));
	lte.ser.print(newName);
	lte.ser.print(F("\"\r"));
	
	/*Serial.print(F("AT+QFTPRENAME=\""));
	Serial.print(oldName);
	Serial.print(F("\",\""));
	Serial.print(newName);
	Serial.print(F("\"\r"));
	*/
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QFTPRENAME:"),5000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0)&&(getData_index(2).toInt()==0))
			{
				lte.ser.flushSerial();
				return true;
			}
		}
	}
	lte.ser.flushSerial();
	return false;
}
String LTE_FTP::modify(char *file)
{
	lte.ser.print(F("AT+QFTPMDTM=\""));
	lte.ser.print(file);
	lte.ser.print(F("\"\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QFTPMDTM:"),5000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0))
			{
				lte.ser.flushSerial();
				String str = getData_index(2);
				str.replace("\"","");
				return str;
			}
		}
	}
	lte.ser.flushSerial();
	return "";
}
bool LTE_FTP::del(char *file)
{
	lte.ser.print(F("AT+QFTPDEL=\""));
	lte.ser.print(file);
	lte.ser.print(F("\"\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QFTPDEL:"),5000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0)&&(getData_index(2).toInt()==0))
			{
				lte.ser.flushSerial();
				return true;
			}
		}
	}
	lte.ser.flushSerial();
	return false;
}
bool LTE_FTP::deldir(char *file)
{
	lte.ser.print(F("AT+QFTPRMDIR=\""));
	lte.ser.print(file);
	lte.ser.print(F("\"\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QFTPRMDIR:"),5000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0)&&(getData_index(2).toInt()==0))
			{
				lte.ser.flushSerial();
				return true;
			}
		}
	}
	lte.ser.flushSerial();
	return false;
}







