#ifndef EC2X_TCP_SSL_h
#define EC2X_TCP_SSL_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X.h"




class EC2X_TCP_SSL
{
	public:
		EC2X_TCP_SSL(uint8_t connectID);
		
		bool open(String address,int port,uint8_t mode,uint8_t SSL_id);
		bool close();
		bool connected();
		bool startSend(unsigned long tout=1000);
		bool stopSend();
		size_t send(uint8_t *data,size_t len,unsigned long tout=1000);
		size_t getRxSize();
		size_t readRxBuffer(uint8_t *data,size_t size);
		void setconnectID(uint8_t id){connectID_ = id;};
		
		
	private:
		uint8_t connectID_;
		uint8_t mode_;
};





















#endif