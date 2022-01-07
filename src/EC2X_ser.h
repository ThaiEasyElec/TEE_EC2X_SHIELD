#ifndef EC2X_SER_h
#define EC2X_SER_h

#include "Arduino.h"
#include <Stream.h>
//#include <MemoryFree.h>

#define RES_MATCH 	0
#define RES_OK		1
#define RES_ERR   	2

#define WAIT_OK F("")
#define MAX_BUF 50
class LTE_Serial : public Stream
{
	public:
	
	uint8_t resID;
	const char resData[MAX_BUF];
	const char resLine[MAX_BUF];
	
	
	LTE_Serial();
	
	void begin(Stream *ser);
	uint8_t waitData(char *input,unsigned long tout=1000,char c='\r');
	uint8_t waitData(__FlashStringHelper *str,unsigned long tout=1000,char c='\r');
	uint8_t readTo(char *data,char c);
	int getindex(char *x,char *y);
	uint8_t readF(char* data,__FlashStringHelper *str);
	int readBuffTo(char *d,size_t dSize,char *s,char c,bool clear=false);
	void showHEX(String data);
	void flushSerial();
	String getData_index(uint8_t num);
	//Stream Class 
	virtual int read() { return ser_->read();
	};	
	virtual size_t write(uint8_t c){return  ser_->write(c);
	};
	virtual int available(){return ser_->available();
	};
	virtual int peek(){return ser_->peek();
	}; 
	virtual void flush()
	{ser_->flush();
	};
	using Stream::write; // make the other overloads of write visible
private:
	Stream *ser_;
	
	private:
	
};














#endif