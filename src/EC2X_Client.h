#ifndef EC2XClient_h
#define EC2XClient_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X_ser.h"

#include "EC2X_TCP.h"


#define Client_SSL_SIZE 100


class EC2XClient : public Stream
{
	public:
		uint8_t buffer[Client_SSL_SIZE];
		EC2XClient(uint8_t connectID){
			tcp = new EC2X_TCP(connectID);
		};
		
		bool connect() 
		{
			memset(buffer,0,Client_SSL_SIZE);
			return  tcp->open(host,port,0);
		}
		void begin(const char* h, int p) 
		{
			 host = h;
			 port = p;
		}
		void disconnect() 
		{ 
			tcp->close();
		}
		bool startSend()
		{
			return tcp->startSend();
		}
		bool endSend()
		{
			return tcp->stopSend();
		}
		
		//Stream Class 
		virtual size_t write(uint8_t c){
			Serial.println(F("Not support"));
			return 0;
		};
		virtual size_t write(uint8_t *c,size_t len)
		{
			return tcp->send(c,len);
		};
		virtual int read() 
		{
			
			if(head == datSize )
			{	
				availabelBuf = tcp->getRxSize();
				datSize = tcp->readRxBuffer(buffer,Client_SSL_SIZE);
				head=0;	
				if(datSize==0)
					return -1;
			}
		
			
			if(datSize==0)
			return -1;
			
			int r = buffer[head];
			head++;
			
			//if(availabelBuf!=0)
				availabelBuf--;
			
			
			return r;
			
		};	
		virtual int available()
		{
			/*if(tcp->getRxSize()>0&&datSize==0)
			{
				Serial.println("Read buff");
				datSize = tcp->readRxBuffer(buffer,Client_SSL_SIZE);
				head = 0;
				availabelBuf = datSize;
			}
			*/
			if(tcp->getRxSize()>0)
			{
				if(head == datSize)
				{	Serial.println("Read buff");
					datSize = tcp->readRxBuffer(buffer,Client_SSL_SIZE);
					head = 0;
					availabelBuf = tcp->getRxSize();
				}
				else if(availabelBuf==0)
				{
					Serial.println("Read buff 2");
					datSize = tcp->readRxBuffer(buffer,Client_SSL_SIZE);
					head = 0;
					availabelBuf = tcp->getRxSize();
				}
					
			}
			
			//Serial.println(availabelBuf);
			return availabelBuf;
		};
		virtual int peek(){
			return 0;
		}; 
		virtual void flush()
		{
			//ser_->flush();
		};
		using Stream::write; // make the other overloads of write visible
	
		EC2X_TCP *tcp;
	private:
		uint8_t connectID_;
		uint8_t mode_;
		const char* host;
		uint16_t port;
		
		
		size_t head=0;
		size_t availabelBuf=0;
		size_t tail=-1;
		size_t datSize=0;
		
		bool empty(){ return datSize?false:true;};
		bool full(){return datSize==Client_SSL_SIZE?true:false;};
		
		bool ringWrite(uint8_t dat){
			if (full())return false;
			tail =( tail +1)% Client_SSL_SIZE;
			buffer[tail] = dat;
			datSize++;return true;
			};
		size_t ringWrite(uint8_t *dat ,unsigned int size){
			unsigned int cnt=0;
			for(unsigned int i=0;i<size;i++){
				if(!ringWrite(dat[cnt++]))
				return cnt;
			}return cnt;};
		
		
		
   
};

























#endif