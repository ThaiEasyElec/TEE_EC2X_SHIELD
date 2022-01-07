#ifndef EC2X_UDP_h
#define EC2X_UDP_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X.h"



class EC2X_UDP
{
	public:
		EC2X_UDP(uint8_t connectID);
		bool open(char *address,int port,uint8_t mode=0);
		bool close();
		
		size_t send(uint8_t *data,size_t len,unsigned long tout=2000);
		size_t print(String data){
			return send((uint8_t*)data.c_str(),data.length());
		};
		size_t println(String data){
			data+="\r\n";
			return send((uint8_t*)data.c_str(),data.length());
		};
		
		
		size_t getRxSize();
		size_t readRxBuffer(uint8_t *data,size_t size);
		void setconnectID(uint8_t id){connectID_ = id;};
		
	private:
		uint8_t connectID_;
		uint8_t mode_;
		const char* host;
		uint16_t port_;
};

#endif