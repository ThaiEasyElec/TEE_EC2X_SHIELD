#include "EC2X.h"

#ifdef __arm__
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

EC2X lte;

EC2X::EC2X()
{
}
void EC2X::begin(Stream *ser_,int reset)
{
	rstPin = reset;
	ser.begin(ser_);
	if(rstPin!=-1)
	{
		pinMode(rstPin,OUTPUT);
		digitalWrite(rstPin,HIGH);
	}	
    NET_APN = F("INTERNET");
	NET_USERNAME = F("");
	NET_PASSWORD = F("");
	
}
void EC2X::reset()
{
	if(rstPin!=-1)
	{
		digitalWrite(rstPin,LOW);
		delay(100);
		digitalWrite(rstPin,HIGH);
	}
	else
	{	
		ser.print(F("AT+QPOWD=0\r"));
		ser.waitData(F("POWERED DOWN"),5000);
		//POWERED DOWN
	}
}
bool EC2X::ready(unsigned long tout)
{
	
	unsigned long pv = millis();
	do{
		ser.flushSerial();
		ser.print(F("AT\r"));
		uint8_t res = ser.waitData(F("RDY"),100);
		if(res>0&&res<2)
		{
			ser.print(F("ATE0\r"));
			ser.waitData(WAIT_OK,200);
			return true;
		}
		ser.flushSerial();
	}
	while((millis()-pv)<tout);
	return false;
}
bool EC2X::simStatus()
{
	for(uint8_t i=0;i<10;i++)
	{
		ser.print(F("AT+CPIN?\r"));
		uint8_t res = ser.waitData(F("+CPIN: READY"),2000);
		if(res==RES_MATCH)
		{
			ser.waitData(WAIT_OK,200);
			return true;
		}
		delay(500);
	}
	return false;
}
String EC2X::IMSI()
{
	ser.flushSerial();
	ser.flushSerial();
	
	
	ser.print(F("AT+CIMI\r"));
	unsigned long pv = millis();
	do
	{
		if(ser.readTo(ser.resLine,'O'))
		{
			String str = String(ser.resLine);
			ser.flushSerial();
			return str;
		}
		
	}
	while((millis() - pv)<1000);
	return "";
}
String EC2X::IMEI()
{
	ser.flushSerial();
	ser.flushSerial();
	lte.ser.print(F("AT+GSN\r"));
	unsigned long pv = millis();
	do
	{
		if(ser.readTo(ser.resLine,'O'))
		{
			String str = String(ser.resLine);
			ser.flushSerial();
			return str;
		}
		
	}
	while((millis() - pv)<1000);
	return "";
}
String EC2X::ICCID()
{
	lte.ser.print(F("AT+QCCID\r"));
	uint8_t  res = ser.waitData(F("+QCCID:"),2000);
	if(res==RES_MATCH)
	{
		ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,' ',true);
		ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,-1);
		String str = String(ser.resLine);
		ser.waitData(WAIT_OK,200);
		ser.flushSerial();
		//str[str.length()-1]=0;
		return str.substring(0,str.length()-1);
	}
	return "";
}
bool EC2X::networkRegister()
{
	for(uint8_t i=0;i<30;i++)
	{
		lte.ser.print(F("AT+CREG?\r"));
		uint8_t  res = ser.waitData(F("+CREG:"),2000);
		if(res==RES_MATCH)
		{
			ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,',',true);
			ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,-1);
			//int x = ser.getData_index(2).toInt();
			int x = atoi(ser.resLine);
			lte.ser.waitData(WAIT_OK,200);
			if(x==1)
				return true;	
		}
		delay(500);
	}
	ser.flushSerial();
	return false;
}
String EC2X::getOperator()
{
	for(uint8_t i=0;i<10;i++)
	{
		String str = getOperator_();
		int index1 = str.indexOf(F("\""));
		int index2 = str.indexOf(F("\""),index1+1);
		if(index1!=-1)
		{
			return str.substring(index1+1,index2);
		}
		delay(1000);
	}
	ser.flushSerial();
	return "";
	
}
String EC2X::getOperator_()
{
	lte.ser.print(F("AT+COPS?\r"));
	uint8_t  res = ser.waitData(F("+COPS:"),2000);
	if(res == RES_MATCH)
	{
		String str = String(ser.resData);
		ser.waitData(WAIT_OK,200);
		ser.flushSerial();
		return String(str);
	}
	ser.flushSerial();
	return "";
}
int EC2X::signal()
{
	int csq=99;
	for(uint8_t i=0;i<10;i++)
	{
		lte.ser.print(F("AT+CSQ\r"));
		uint8_t res = ser.waitData(F("+CSQ:"),10000);
		if(res==RES_MATCH)
		{
			
			ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,' ',true);
			ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,-1);
			csq = atoi(ser.resLine);
			ser.waitData(WAIT_OK);
			/*sscanf(res.data.c_str(),"%*s %d\n",&csq);
			lte.ser.waitData(WAIT_OK);
			*/
			ser.flushSerial();
			if(csq!=99)
				return csq;
		}
		delay(500);
	}
	ser.flushSerial();
	return csq;
}
String EC2X:: networkInfo()
{
	lte.ser.print(F("AT+QNWINFO\r"));
	uint8_t res = ser.waitData(F("+QNWINFO:"),2000);
	if(res==RES_MATCH)
	{
		//ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,' ',true);
		//ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,-1);
		String str = ser.getData_index(1);//String(ser.resLine);
		lte.ser.waitData(WAIT_OK,200);
		ser.flushSerial();
		return str;
	}
	ser.flushSerial();
	return "";
}
bool EC2X:: connectInternet()
{
	/*
	NET_APN = "INTERNET";
	NET_USERNAME = "";
	NET_PASSWORD = "";
	*/
	//lte.ser.print(F("AT+QICSGP=1,1,\"INTERNET\",\"\",\"\",1\r"));
	lte.ser.print(F("AT+QICSGP=1,1,\""));
	lte.ser.print(NET_APN);
	lte.ser.print(F("\",\""));
	lte.ser.print(NET_USERNAME);
	lte.ser.print(F("\",\""));
	lte.ser.print(NET_PASSWORD);
	lte.ser.print(F("\",1\r"));
	
	uint8_t  res = ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		lte.ser.print(F("AT+QIACT=1\r"));
		res = ser.waitData(WAIT_OK,5000); 
		if(res==RES_OK)
		{
			ser.flushSerial();
			return true;
		}
	}
	ser.flushSerial();
	return false;
}
bool EC2X:: disconnectInternet()
{
	lte.ser.print(F("AT+QIDEACT=1\r"));
	uint8_t  res = ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		ser.flushSerial();
		return true;
	}
	ser.flushSerial();
	return false;	
}
String EC2X::IPAddress()
{
	String ret = "";
	lte.ser.print(F("AT+QIACT?\r"));
	uint8_t  res = ser.waitData(F("+QIACT:"),2000);
	if(res==RES_MATCH)
	{
		ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,'\"',true);
		ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,'\"');
		ret  = String(ser.resLine);
	}
	lte.ser.waitData(WAIT_OK,500);
	ser.flushSerial();
	return ret;
	
}
bool EC2X::internetStatus()
{
	String str = IPAddress();
	if(str.length()>1)
		return true;
	return false;
}

String EC2X::getTime()
{
	String data="";
	lte.ser.print(F("AT+CCLK?\r"));
	uint8_t  res = ser.waitData(F("+CCLK:"),2000);
	if(res==RES_MATCH)
	{
		ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,'\"',true);
		ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,'\"');
		data  = String(ser.resLine);
	}
	ser.flushSerial();
	return data;
}
String EC2X::setNTP(char *server,int port)
{
	String data="";
	lte.ser.print(F("AT+QNTP=1,\""));
	lte.ser.print(server);
	lte.ser.print(F("\","));
	lte.ser.print(port);
	lte.ser.print(F("\r"));
	
	uint8_t  res = ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		 res = ser.waitData(F("+QNTP:"),5000);
		 if(res==RES_MATCH)
		{
			ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,'\"',true);
			ser.readBuffTo(ser.resLine,MAX_BUF,ser.resData,'\"');
			data  = String(ser.resLine);
		}
		 
	}
	ser.flushSerial();
	return data;
}

bool EC2X::call(char *number)
{
	lte.ser.print(F("ATD"));
	lte.ser.print(number);
	lte.ser.print(F(";"));
	lte.ser.print(F("\r"));
	uint8_t  res = ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		ser.flushSerial();
		return true;
	}
		ser.flushSerial();
	return false;
}
bool EC2X::disconnect()
{
	lte.ser.print(F("ATH"));
	lte.ser.print(F("\r"));
	uint8_t  res = ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		ser.flushSerial();
		return true;
	}
		ser.flushSerial();
	return false;
}

bool EC2X::answer()
{
	lte.ser.print(F("ATA"));
	lte.ser.print(F("\r"));
	uint8_t  res = ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		ser.flushSerial();
		return true;
	}
		ser.flushSerial();
	return false;
}
uint8_t EC2X::activityStatus()
{
	lte.ser.print(F("AT+CPAS"));
	lte.ser.print(F("\r"));
	uint8_t  res = ser.waitData(F("+CPAS:"),5000);
	if(res==RES_MATCH)
	{
		int id = lte.ser.getData_index(1).toInt();
		ser.flushSerial();
		return id;
	}
		ser.flushSerial();
	return 0;
}
String EC2X::currentCall()
{
	callStatus_ =-1;
	lte.ser.print(F("AT+CLCC"));
	lte.ser.print(F("\r"));
	uint8_t  res = ser.waitData(F("+CLCC:"),5000);
	if(res==RES_MATCH)
	{
		String number =  lte.ser.getData_index(6);
		number.replace("\"","");
		if(number.length()<=2)
		{
			res = ser.waitData(F("+CLCC:"),5000);
			if(res==RES_MATCH)
			{
				callStatus_ = lte.ser.getData_index(3).toInt();
				String number =  lte.ser.getData_index(6);
				number.replace("\"","");
				ser.flushSerial();
				return number;
			}
		}
	}
		ser.flushSerial();
	return "";
}

int EC2X::callStatus()
{
	currentCall();
	return callStatus_;
}
bool EC2X::characterSet(char *chset)
{
	lte.ser.print(F("AT+CSCS=\""));
	lte.ser.print(chset);
	lte.ser.print(F("\"\r"));
	uint8_t  res = ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		ser.flushSerial();
		return true;
	}
		ser.flushSerial();
	return false;
	
	
}
bool EC2X::ussd(char *str_ussd)
{
	//14:52:56.036 -> +CUSD: 0,"0E040E380E130E080E300E440E140E490E230E310E1A0E020E490E2D0E210E390E250E410E1E0E470E010E400E010E080E1B0E310E080E080E380E1A0E310E190E170E320E070053004D005300200E1F0E310E07002F0E0B0E370E490E2D0E410E1E0E470E010E400E01E00E20E70E8019042E17E2302A037030030EF0E3035"72
	lte.ser.print(F("AT+CUSD=1,"));
	lte.ser.print(str_ussd);
	lte.ser.print(F("\r"));
	unsigned long pv = millis();
	do
	{
		if(lte.ser.available())
		{
			int c = lte.ser.read();
			char * pEnd;
			if(c=='\"')
			{
				for(uint8_t x=0;x<10;x++)
				{
					while(lte.ser.available())
					{
						char buf[2];
						lte.ser.readBytes(buf,2);
						int x = strtol (buf,&pEnd,16); 
						 if(x==0x0E)
						{
							lte.ser.readBytes(buf,2);
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
				
			}
		}
		
	}
	while((millis() - pv)<10000);
	return false;
	
}
int EC2X::ramAvailable()
{
char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}



/*########################## SSL Config ##########################*/
bool SSL::sslConfig(const __FlashStringHelper *config,uint8_t SSL_id,char * mode)
{
	lte.ser.print(F("AT+QSSLCFG=\""));
	lte.ser.print(config);
	lte.ser.print(F("\","));
	lte.ser.print(SSL_id);
	lte.ser.print(F(","));
	lte.ser.print(mode);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,1000);
	if(res==RES_OK)
	{
		return true;
	}
	return false;
}
bool SSL::sslConfig(const __FlashStringHelper *config,uint8_t SSL_id,uint16_t mode)
{
	lte.ser.print(F("AT+QSSLCFG=\""));
	lte.ser.print(config);
	lte.ser.print(F("\","));
	lte.ser.print(SSL_id);
	lte.ser.print(F(","));
	lte.ser.print(mode);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,1000);
	if(res==RES_OK)
	{
		return true;
	}
	return false;
}








