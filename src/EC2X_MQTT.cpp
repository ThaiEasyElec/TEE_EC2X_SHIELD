#include "EC2X_MQTT.h"


void func_null_(String topic ,uint8_t *payload,size_t length){}

EC2X_MQTT::EC2X_MQTT(uint8_t connectID)
{
	if((connectID>0)&&(connectID<=5))
	{
		connectID_ = connectID;
	}
	callback = func_null_;
}
void EC2X_MQTT::init()
{
	recvMode(true,true);
	
}
bool EC2X_MQTT::configResp(void)
{
	lte.ser.print("\r");
	uint8_t  res = lte.ser.waitData(WAIT_OK,1000);
	if(res == RES_OK)
	{
		return true;
	}
	return false;
}
bool EC2X_MQTT::version(uint8_t ver)
{
	lte.ser.print(F("AT+QMTCFG=\"version\","));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(ver);
	return configResp();
}
bool EC2X_MQTT::will(bool will_fg,uint8_t will_qos,bool will_retain,String will_topic,String will_msg)
{
	lte.ser.print(F("AT+QMTCFG=\"will\","));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(will_fg);
	lte.ser.print(F(","));
	lte.ser.print(will_qos);
	lte.ser.print(F(","));
	lte.ser.print(will_retain);
	lte.ser.print(F(",\""));
	lte.ser.print(will_topic);
	lte.ser.print(F("\","));
	lte.ser.print(F(",\""));
	lte.ser.print(will_msg);
	lte.ser.print(F("\""));
	return configResp();
}
bool EC2X_MQTT::timeOut(uint8_t pkt_timeout,uint8_t retry_times,bool timeout_notice)
{
	lte.ser.print(F("AT+QMTCFG=\"timeout\","));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(pkt_timeout);
	lte.ser.print(F(","));
	lte.ser.print(retry_times);
	lte.ser.print(F(","));
	lte.ser.print(timeout_notice);
	return configResp();
}
bool EC2X_MQTT::session(bool clean_session)
{
	//AT+QMTCFG=“session”,<client_idx>[,<clean_session>]
	lte.ser.print(F("AT+QMTCFG=\"session\","));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(clean_session);
	return configResp();	
}
bool EC2X_MQTT::keepalive(uint16_t time)
{
	//AT+QMTCFG=“keepalive”,<client_idx>[,<keep-alive time>]
	lte.ser.print(F("AT+QMTCFG=\"keepalive\","));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(time);
	return configResp();
}
bool EC2X_MQTT::ssl(bool sslenable,int sslctx_idx)
{
	//AT+QMTCFG=“ssl”,<client_idx>[,<sslenable>[,<sslctx_idx>]]

	lte.ser.print(F("AT+QMTCFG=\"ssl\","));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(sslenable);
	if(sslctx_idx!=-1)
	{
		lte.ser.print(F(","));
		lte.ser.print(sslctx_idx);
	}
	return configResp();
}
bool EC2X_MQTT::recvMode(bool msg_recv_mode,bool msg_len_enable)
{
//AT+QMTCFG=“recv/mode”,<client_idx>,<msg_recv_mode>[,<msg_len_enable>]
	lte.ser.print(F("AT+QMTCFG=\"recv/mode\","));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(msg_recv_mode);
	lte.ser.print(F(","));
	lte.ser.print(msg_len_enable);
	return configResp();
}
bool EC2X_MQTT::qmtping(uint8_t qmtping_interval)
{
	//AT+QMTCFG=“qmtping”[,<qmtping_interval>]
	lte.ser.print(F("AT+QMTCFG=\"qmtping\","));
	lte.ser.print(qmtping_interval);
	return configResp();
}
bool EC2X_MQTT::open(char *host,uint16_t port)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QMTOPEN="));
	lte.ser.print(connectID_);
	lte.ser.print(F(",\""));
	lte.ser.print(host);
	lte.ser.print(F("\","));
	lte.ser.print(port);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,10000);
	if(res == RES_OK)
	{
		res = lte.ser.waitData(F("+QMTOPEN:"),10000);
		if(res == RES_MATCH)
		{
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			int conn  = atoi(lte.ser.resLine);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
			int result  = atoi(lte.ser.resLine);
			
			//+QMTOPEN: 0,0
			if((conn==connectID_)&&(result == 0 ))
			{
				lte.ser.flushSerial();
				return true;
			}
			else
				Serial.println(lte.ser.resData);
		}
		
	}
	else
			Serial.println(lte.ser.resData);
	return false;
}
bool EC2X_MQTT::close()
{
	lte.ser.print(F("AT+QMTCLOSE="));
	lte.ser.print(connectID_);
	return configResp();
}
bool EC2X_MQTT::connect(const char* clientID,const char* username,const char* password)
{
	//AT+QMTCONN=<client_idx>,“<clientID>”[,“<username>”[,“<password>”]]
	lte.ser.print(F("AT+QMTCONN="));
	lte.ser.print(connectID_);
	lte.ser.print(F(",\""));
	lte.ser.print(clientID);
	lte.ser.print(F("\""));
	if(strlen(username)>0)
	{
		lte.ser.print(F(",\""));
		lte.ser.print(username);
		lte.ser.print(F("\""));
		
	}
	if(strlen(password)>0)
	{
		lte.ser.print(F(",\""));
		lte.ser.print(password);
		lte.ser.print(F("\""));
	}
	lte.ser.print("\r");
	
	uint8_t  res = lte.ser.waitData(WAIT_OK,1000);
	if(res == RES_OK)
	{
		res = lte.ser.waitData(F("+QMTCONN:"),10000);
		if(res ==RES_MATCH)
		{
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			int conn  = atoi(lte.ser.resLine);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			int result  = atoi(lte.ser.resLine);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
			int ret_code  = atoi(lte.ser.resLine);
			//+QMTCONN: 0,0,0
			if((conn == connectID_)&&(result == 0)&&(ret_code== 0))
			{
				lte.ser.flushSerial();
				return true;
			}
			else
			{
				Serial.println(lte.ser.resData);
			}
		}
	}
	
	return false;
}
bool EC2X_MQTT :: disconnect()
{
	lte.ser.print(F("AT+QMTDISC="));
	lte.ser.print(connectID_);
	lte.ser.print(F("\r"));
	uint8_t  res = lte.ser.waitData(WAIT_OK,1000);
	if(res == RES_OK)
	{
		res = lte.ser.waitData(F("+QMTDISC:"),10000);
		if(res ==RES_MATCH)
		{
			//+QMTDISC: 0,0
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			int conn  = atoi(lte.ser.resLine);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,'-1');
			int result  = atoi(lte.ser.resLine);
			if(( conn == connectID_)&&( result == 0))
			{
				lte.ser.flushSerial();
				return true;
			}
			else
			{
				Serial.println(lte.ser.resData);
			}
		}
	}
	return false;
}
bool EC2X_MQTT ::subResp(uint8_t mode)
{
	uint8_t  res = lte.ser.waitData(WAIT_OK,20000);
	if(res == RES_OK)
	{
		if(mode==0)
			res = lte.ser.waitData(F("+QMTSUB:"),20000);
		if(mode==1)
			res = lte.ser.waitData(F("+QMTUNS:"),20000);
		if(res == RES_MATCH)
		{
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			int conn  = atoi(lte.ser.resLine);
			
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			int msgid  = atoi(lte.ser.resLine);
			
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
			int result  = atoi(lte.ser.resLine);
		
			if(( conn == connectID_)&&( result == 0))
			{
				lte.ser.flushSerial();
				return true;
			}
			else
			{
				Serial.println(lte.ser.resData);
			}
		}
	}
	return false;
}
bool EC2X_MQTT :: sub(char *topic,uint8_t qos,int msgID)
{
	//AT+QMTSUB=<client_idx>,<msgID>,“<topic1>”,<qos1>[,“<topic2>”,<qos2>…]
	lte.ser.print(F("AT+QMTSUB="));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(msgID);
	lte.ser.print(F(",\""));
	lte.ser.print(topic);
	lte.ser.print(F("\","));
	lte.ser.print(qos);
	lte.ser.print(F("\r"));
	return subResp();
	
}
bool EC2X_MQTT :: subs(char* topics,int msgID)
{
	//AT+QMTSUB=<client_idx>,<msgID>,“<topic1>”,<qos1>[,“<topic2>”,<qos2>…]
	lte.ser.print(F("AT+QMTSUB="));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(msgID);
	lte.ser.print(F(","));
	lte.ser.print(topics);
	lte.ser.print(F("\r"));
	
	return subResp();
}
bool  EC2X_MQTT ::unsub(char *topic,int msgID)
{
	//AT+QMTUNS=<client_idx>,<msgID>,“<topic1>”[,“<topic2>”…]
	lte.ser.print(F("AT+QMTUNS="));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(msgID);
	lte.ser.print(F(",\""));
	lte.ser.print(topic);
	lte.ser.print(F("\"\r"));
	subResp(1);
}
bool EC2X_MQTT ::pub(char *topic,uint8_t *payload,size_t msg_length,uint8_t qos,uint8_t retain,uint8_t msgID,unsigned long tout)
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QMTPUBEX="));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	if(qos==0)
		msgID = 0;
	lte.ser.print(msgID);
	lte.ser.print(F(","));
	lte.ser.print(qos);
	lte.ser.print(F(","));
	lte.ser.print(retain);
	lte.ser.print(F(",\""));
	lte.ser.print(topic);
	lte.ser.print(F("\","));
	lte.ser.print(msg_length);
	lte.ser.print(F("\r"));
	
	unsigned long pv = millis();
		do{
			if(lte.ser.available())
			{
				int c = lte.ser.read();
				//Serial.write(c);
				if(c =='>')
				{
					
					
					size_t s = lte.ser.write(payload,msg_length);
					//+QMTPUBEX: <client_idx>,<msgID>,<result>[,<value>]
					lte.ser.waitData(WAIT_OK,100);
					
					uint8_t  res = lte.ser.waitData(F("+QMTPUBEX:"),1000);
					if(res == RES_MATCH)
					{
						//+QMTPUBEX: 0,0,0[,<value>]
						
						lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
						lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
						int conn  = atoi(lte.ser.resLine);
						lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
						int msgid  = atoi(lte.ser.resLine);
						lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
						int result  = atoi(lte.ser.resLine);
			
						if((conn== connectID_)&&(result == 0))
						{
							lte.ser.flushSerial();
							return true;
						}
						else
						{
							Serial.println(lte.ser.resData);
						}
					}
				}
				else
				{
					//Serial.write(c);
				}
			}
		}
		while((millis()-pv)<tout);
	
	return false;
	
}
bool EC2X_MQTT ::pub(String topic,String payload,uint8_t qos,uint8_t retain,uint8_t msgID,unsigned long tout)
{
	return pub(topic.c_str(),(uint8_t*)payload.c_str(),payload.length(),qos,retain,msgID,tout);
}
bool EC2X_MQTT ::pub(char *topic,char *payload,uint8_t qos,uint8_t retain,uint8_t msgID,unsigned long tout)
{
	return pub(topic,(uint8_t*)payload,strlen(payload),qos,retain,msgID,tout);
}
bool EC2X_MQTT::connected(void)
{
	//AT+QMTCONN?
	//+QMTCONN: 0,3
	//OK
	for(uint8_t i=0;i<3;i++)
	{
		lte.ser.flushSerial();
		lte.ser.print(F("AT+QMTCONN?\r"));
		uint8_t  res = lte.ser.waitData(F("+QMTCONN:"),1000);
		if(res == RES_MATCH)
		{
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			int conn  = atoi(lte.ser.resLine);
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,-1);
			int result  = atoi(lte.ser.resLine);
				
			lte.ser.waitData(WAIT_OK,100);
			if((conn== connectID_)&&(result !=4))
			{
				lte.ser.flushSerial();
				return true;
			}
			else
			{
				Serial.println(lte.ser.resData);
			}
		}
		delay(100);
	}
	return false;
}

uint8_t EC2X_MQTT::available()
{
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QMTRECV?\r"));
	uint8_t  res = lte.ser.waitData(F("+QMTRECV:"),1000);
	if(res == RES_MATCH)
	{
		
		//Serial.println(res.data);
		//+QMTRECV: 0,1,1,1,1,1
		
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,' ',true);
		lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
		uint8_t id  = atoi(lte.ser.resLine);
		for(uint8_t i=0;i<5;i++)
		{
			lte.ser.readBuffTo(lte.ser.resLine,MAX_BUF,lte.ser.resData,',');
			msgStatus[i]  = atoi(lte.ser.resLine);
		}
		
		lte.ser.waitData(WAIT_OK,100);

		if(id == connectID_)
		{
			return msgStatus[0]+msgStatus[1]+msgStatus[2]+msgStatus[3]+msgStatus[4];
		}
		
	}
	return 0;
}
size_t EC2X_MQTT::readMessage(uint8_t id,String *topic,uint8_t *payloadBuff,size_t buffSize,unsigned long tout)
{
	lte.ser.flushSerial();
	if(id>4)
		return 0;
	if(!msgStatus[id])
		return 0;
	lte.ser.flushSerial();
	lte.ser.print(F("AT+QMTRECV="));
	lte.ser.print(connectID_);
	lte.ser.print(F(","));
	lte.ser.print(id);
	lte.ser.print('\r');
	//+QMTRECV: 0,0,"abc1",9,"123456789"
	unsigned long pv = millis();
	do
	{
		if(lte.ser.available())
		{
			//char buf[50];
			lte.ser.readTo(lte.ser.resLine,':');
			if(lte.ser.getindex(lte.ser.resLine,"+QMTRECV")!=-1)
			{
				lte.ser.readTo(lte.ser.resLine,'\"');
				lte.ser.readTo(lte.ser.resLine,'\"');
				*topic = String(lte.ser.resLine);
				lte.ser.readTo(lte.ser.resLine,',');
				lte.ser.readTo(lte.ser.resLine,',');
				int len = atoi(lte.ser.resLine);
				if(len>buffSize)
				{
					len = buffSize;
					Serial.println(F("\r\n!!!!Warning  payload size > Free RAM size!!!!"));
				}
				while(!lte.ser.available()){delay(2);}
				lte.ser.read();
				lte.ser.readBytes(payloadBuff,len);
				lte.ser.waitData(WAIT_OK,100);
				return len;
			}
		}
	}
	while((millis()-pv)<tout);
	return 0;
}
void EC2X_MQTT::mqttTask(void)
{
	if(connected())
	{
		if(available())
		{
			for(uint8_t i=0;i<ALL_MSG;i++)
			{
				if(msgStatus[i])
				{
					int ram = lte.ramAvailable()/2;
					String  topic;
					uint8_t payload[ram];
					size_t len = readMessage(i,&topic,payload,ram);
					(*callback)(topic,payload,len);
				}
			}
		}
	}
}
