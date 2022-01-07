#include "EC2X_ser.h"

LTE_Serial::LTE_Serial()
{
	
}
void LTE_Serial::begin(Stream *ser)
{
	ser_ = ser;
	
}
/*note !!!
	 Send AT command
	 ATI<CR>
	 response
	 
	"<CR><LF><response><CR><LF>"
	<CR><LF> == /r/n == 0x0D 0x0A
*/
uint8_t LTE_Serial::waitData(__FlashStringHelper *str,unsigned long tout,char c)
{
	char input[50];
	readF(input,str);
	return waitData(input,tout,c);
}

uint8_t LTE_Serial::waitData(char *input,unsigned long tout,char c)
{

	resID=-1;
	unsigned long pv = millis();
	memset(resLine,0,MAX_BUF);
	do
	{
		while(ser_->available())
		{
			//Serial.println("zz");
			if(readTo(resLine,c))
			{
				//Serial.println(resLine);
				if(strlen(input)>1)
				{
					if(getindex(resLine,input)!=-1)
					{
						resID = RES_MATCH;
						goto OUT;
					}
				}
				if(getindex(resLine,"OK")!=-1)
				{
					resID = RES_OK;
					goto OUT;
				}
				if(getindex(resLine,"ERROR")!=-1)
				{
					resID = RES_ERR;
					goto OUT;
				}		
			}
		}
	}
	while((millis() - pv)<tout);
	
	OUT:
	strcpy(resData,resLine);
	return resID;
}
uint8_t LTE_Serial::readF(char* data,__FlashStringHelper *str)
{
	const char *d = (const char*)str;
	char c;
	uint8_t cnt=0;
	while(c = (char*)pgm_read_byte_near(d++))
	{
		data[cnt++] =c;
	}
	data[cnt]=0;
	return cnt;
	
}

uint8_t LTE_Serial::readTo(char *data,char c)
{
	uint8_t cnt=0;
	unsigned long pv = millis();
	do
	{
		while(ser_->available())
		{
			int x = ser_->read();
			//Serial.write(x);
			if(x == c)
			{
				data[cnt]=0;
				return cnt;
			}
			else
			{
				if((x!=0x0A) && (x!=0x0D))
				{
					//Serial.write(x);
					//Serial.println(cnt);
					//delay(1);
					data[cnt++] = x;
					pv = millis();
					if(cnt>= MAX_BUF)
					{
						data[cnt-1]=0;
						return cnt;
					}
				}
			}
		}
	}
	while(millis()-pv<5);
	
	data[cnt]=0;
	return cnt;
}
int LTE_Serial::getindex(char *x,char *y)
{
	char * p;
	 p = strstr (x,y);
	 if(p)
	 {
		return (int) (p - x);
	 }
	 return -1;
}

int LTE_Serial::readBuffTo(char *d,size_t dSize,char *s,char c,bool clear)
{
	static uint8_t p=0;
	int cnt=0;
	if(clear)
		p=0;
	
	while(s[p])
	{
		if(s[p]== c)
		{
			p++;
			break;
		}
		else
		{
			d[cnt++] = s[p];
			if(cnt>=dSize)
				break;
		}	
		p++;
		
	}
	d[cnt]=0;
	return cnt;
}

String LTE_Serial::getData_index(uint8_t num)
{
	    size_t i=0;
		while(resData[i++]!=' '){}
		char buf[20];
		uint8_t cnt;
		for(uint8_t x=0;x<num;x++)
		{
			cnt=0;
			memset(buf,0,20);
			while(resData[i]!=',' &&resData[i]!=0){buf[cnt++]=resData[i]; i++;}
			//Serial.println(buf);
			i++;
			
		}
	return String(buf);
}






























void LTE_Serial::showHEX(String data)
{
	Serial.println();
	for(uint8_t i=0;i<data.length();i++)
	{
		Serial.print(F("0x"));
		if(data[i]<=0x0F)
			Serial.print(F("0"));
		
		Serial.print(data[i],16);
		Serial.print(F(" "));
	}
}

void LTE_Serial::flushSerial()
{
	for(uint8_t i=0;i<10;i++)
	{
		while(ser_->available())
		{
			char c = ser_->read();
		}
	}
}
