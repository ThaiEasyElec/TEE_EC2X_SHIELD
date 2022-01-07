#include "EC2X_SMS.h"


EC2X_SMS::EC2X_SMS()
{
	
}
void EC2X_SMS::init()
{
	messageFormat(TEXT);
	messageTextMode();
	lte.characterSet("UCS2");
	String mst = F("=\"ME\",\"ME\",\"ME\"");
	messageStorage(mst);
	messageShowText(false);
	delay(1000);
	
}
bool EC2X_SMS::messageShowText(bool enable)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CSDH="));
	lte.ser.print(enable);
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
bool EC2X_SMS::messageTextMode(int fo,int vp,int pid,int dcs )
{
	// AT+CSMP=17,167,0,8
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CSMP="));
	lte.ser.print(fo);
	lte.ser.print(F(","));
	lte.ser.print(vp);
	lte.ser.print(F(","));
	lte.ser.print(pid);
	lte.ser.print(F(","));
	lte.ser.print(dcs);
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
bool EC2X_SMS::messageStorage(String &mem)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CPMS"));
	lte.ser.print(mem);
	lte.ser.print(F("\r"));
	
	uint8_t  res = lte.ser.waitData(F("+CPMS:"),5000);
	if(res==RES_MATCH)
	{
		lte.ser.waitData(WAIT_OK,1000);
		/*lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
		Serial.println(lte.ser.resLine);	
		*/
		
		return true;
	}
	lte.ser.waitData(WAIT_OK,1000);
	return false;
}
int EC2X_SMS::lastMessageID()
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CPMS?\r"));
	uint8_t  res = lte.ser.waitData(F("+CPMS:"),5000);
	if(res==RES_MATCH)
	{
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
		//lte.ser.flushSerial();
		//Serial.println(lte.ser.resLine);
		int index = atoi(lte.ser.resLine)-1;
		lte.ser.waitData(WAIT_OK,1000);	
		return index;
	}
	lte.ser.waitData(WAIT_OK,1000);
	return -1;
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
String EC2X_SMS:: getSenderNumber(int index)
{
	String s = readMsg_(index,'\"',3);
	String out="";
	//Serial.println(s); //getsender
	uint8_t *msg = s.c_str();
	size_t len = s.length();
	for(size_t i=0;i<len;i++)
	{
		unsigned long ul;
		uint8_t buf[2]={msg[i++],msg[i]};
		ul = strtoul (buf, NULL, 16);
		if(char(ul)!=0x00)
			out += char(ul); 
		//Serial.write(ul);
	}
	//Serial.write("\r\n");
	return out;
}
String EC2X_SMS:: getTime(int index)
{
	return readMsg_(index,'\"',5);	
}
String EC2X_SMS:: getMessage(int index)
{
	String s = readMsg_(index,'\n',2);
	String out="";
	//Serial.println(s);
	
	uint8_t *msg = s.c_str();
	size_t len = s.length();
	
	//Serial.print("RAM:");
	//Serial.println(lte.ramAvailable());
	for(size_t i=0;i<len;i+=2)
	{
		unsigned long ul;
		uint8_t buf[2]={msg[i],msg[i+1]};
		ul = strtoul (buf, NULL, 16);
		if(char(ul)==0x0E)
		{
			out += char(0xE0);
			//Serial.write(0xE0);
			i+=2;
			buf[0] = msg[i];
			buf[1] = msg[i+1];
			ul = strtoul (buf, NULL, 16);
			if(char(ul)<0x40)
			{
				out += char(0xB8);
				out += char(ul+0x80);
				//Serial.write(0xB8);
				//Serial.write(ul+0x80);
			}
			else
			{
				out += char(0xB9);
				out += char(ul+0x40);
				//Serial.write(0xB9);
				//Serial.write(ul+0x40);
			}
			
			
		}
		else
		{	if(char(ul)!=0x00)
			{
				out += char(ul);
				//Serial.write(char(ul));
			}
				
		}
	}
	
	//Serial.print("RAM:");
	//Serial.println(lte.ramAvailable());
	return out;
}
String EC2X_SMS:: readMsg_(int msg_index,char val,size_t from)
{
	String ret="";
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CMGR="));
	lte.ser.print(msg_index);
	lte.ser.print(F("\r"));
	unsigned long pv = millis();	
	unsigned long tout = 2000;
	bool flag = false;
	uint8_t cntx = 0;
	
	do
	{
		while(lte.ser.available())
		{
			int c = lte.ser.read();
			
			if(flag)
			{
				//Serial.write(c);
				//if(c!='\r')
					ret +=  char(c);
			}
			if(char(c) == val )
			{
				cntx++;
				//Serial.print("cntx: ");
				//Serial.println(cntx);
				if(cntx==from)
					flag = true;
				else
				{
					if(flag==true)
					{
						if(ret.length()>0)
							ret.remove(ret.length()-1);	
					}
					flag = false;
				}
			}
			pv = millis(); 
		    tout = 100;
		}
	}
	while(millis()-pv<tout);
	return ret;
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
void EC2X_SMS::printUnicode(String str)
{
	uint8_t *str_ = str.c_str();
	size_t strlen = str.length();
	size_t i=0;
	#define smsser lte.ser
	while(str_[i])
	{
		if(str_[i]==0xE0)
		{
			if(str_[i+1]==0xB8)
			{
				i+=2;
				uint8_t ucode = str_[i]-0x80;
				lte.ser.print("0E");
				if(ucode<16)
					lte.ser.print('0');
				lte.ser.print(ucode,HEX);
			}
			if(str_[i+1]==0xB9)
			{
				i+=2;
				uint8_t ucode = str_[i]-0x40;
				lte.ser.print("0E");
				if(ucode<16)
					lte.ser.print('0');
				lte.ser.print(ucode,HEX);
			}
		}
		else
		{
			lte.ser.print("00");
			if(str_[i]<16)
				lte.ser.print('0');
			lte.ser.print(str_[i],HEX);
		}
		i++;
	}
}
bool EC2X_SMS::startSend(String number)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+CMGS=\""));
	printUnicode(number);
	lte.ser.print(F("\""));
	lte.ser.print(F("\r"));
	
	unsigned long pv = millis();
	do{
		if(lte.ser.available())
		{
			int c = lte.ser.read();
			//Serial.write(c);
			if(c =='>')
			{
				return true;
			}
			else
			{
				//Serial.write(c);
			}
		}
	}
	while((millis()-pv)<3000);
	return false;
}
bool EC2X_SMS::endSend()
{
	lte.ser.write(0x1A);
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		lte.ser.flushSerial();
		return true;
	}
		lte.ser.flushSerial();
	return false;
}




