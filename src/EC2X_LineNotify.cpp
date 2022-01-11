#include "EC2X_LineNotify.h"
#include "EC2X_TCP_SSL.h"


void func_null_line(uint8_t *serverResp,size_t length){}

EC2X_LINE_NOTIFY::EC2X_LINE_NOTIFY(uint8_t socketID)
{
	//socketID_ = socketID;
	callback  =func_null_line;
	tcp_line = new EC2X_TCP_SSL(socketID);
}
void  EC2X_LINE_NOTIFY::setSSL(uint8_t SSL_id)
{
	SSL_id_ = SSL_id;
}
bool EC2X_LINE_NOTIFY::send(char *token,String msg,int stickerPacket,long stickerID)
{
	if(tcp_line->open(F("notify-api.line.me"),443,0,SSL_id_))
	{
		if(tcp_line->startSend())
		{
			lte.ser.print(F("POST /api/notify HTTP/1.1\r\n"));
			lte.ser.print(F("Authorization: Bearer "));
			lte.ser.print(token);
			lte.ser.print(F("\r\n"));
			lte.ser.print(F("Content-Type: application/x-www-form-urlencoded\r\n"));
			lte.ser.print(F("Accept: */*\r\n"));
			lte.ser.print(F("Cache-Control: no-cache\r\n"));
			lte.ser.print(F("Host: notify-api.line.me\r\n"));
			lte.ser.print(F("Accept-Encoding: gzip, deflate, br\r\n"));
			lte.ser.print(F("Connection: keep-alive\r\n"));
			lte.ser.print(F("Content-Length: "));
			
			String 	msgLine = "message=";
					msgLine += msg;
			if(stickerPacket!=-1)
					msgLine += "&stickerPackageId="+String(stickerPacket);
			if(stickerID!=-1)
					msgLine += "&stickerId="+String(stickerID);
			
			//msgLine += "&notificationDisabled=1";
				
			lte.ser.print(msgLine.length());
			lte.ser.print(F("\r\n\r\n"));
			lte.ser.print(msgLine);
			if(tcp_line->stopSend())
			{
				//Serial.println("Send OK");
				unsigned long pv = millis();	
				unsigned long tout = 2000;
				do
				{
					while(tcp_line->getRxSize()>0)
					{
						uint8_t buf[100];
						size_t len = tcp_line->readRxBuffer(buf,100);
						(*callback)(buf,len);
						//Serial.write(buf,len);  
					}
				}
				while(millis()-pv<tout);
				tcp_line->close();
				return true;
			}
		}
		tcp_line->close();
	}	
	return false;
}































