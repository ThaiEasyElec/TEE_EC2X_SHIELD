#ifndef EC2X_LINE_NOTIFY_h
#define EC2X_LINE_NOTIFY_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X.h"
#include "EC2X_TCP_SSL.h"



class EC2X_LINE_NOTIFY 
{
	public:
	EC2X_LINE_NOTIFY(uint8_t socketID);
	void setSSL(uint8_t SSL_id);
	bool send(char *token,String msg,int stickerPacket=-1,long stickerID=-1);
	void (*callback)(uint8_t *serverResp,size_t length);
	
	private:
		uint8_t socketID_;
		uint8_t SSL_id_;
		EC2X_TCP_SSL *tcp_line;
};




































#endif