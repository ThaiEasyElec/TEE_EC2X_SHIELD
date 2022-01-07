#ifndef EC2X_TCP_h
#define EC2X_TCP_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X.h"



class EC2X_TCP 
{
	public:
		EC2X_TCP(uint8_t connectID);
		bool open(char *address,int port,uint8_t mode);
		bool close();
		bool startSend(unsigned long tout=1000);
		bool stopSend();
		size_t send(uint8_t *data,size_t len,unsigned long tout=2000);
		bool connected();
		size_t getRxSize();
		size_t readRxBuffer(uint8_t *data,size_t size);
		void setconnectID(uint8_t id){connectID_ = id;};
		
	private:
		uint8_t connectID_;
		uint8_t mode_;
		
};


































#endif