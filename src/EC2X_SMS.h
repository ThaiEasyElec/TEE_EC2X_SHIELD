#ifndef EC2X_SMS_h
#define EC2X_SMS_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X.h"

#define PDU 	false
#define TEXT 	true
class EC2X_SMS 
{
	public:
	EC2X_SMS();
	bool messageService(bool service=false);
	bool messageFormat(bool format=PDU);
	String serviceCenterAddress();
	bool messageStorage(char *mem);
	bool deleteMessage(int index,uint8_t delflag=0);
	void readMessage(int index);
	
	int convetUnicodeToUTF8(char *data);
	
};



#endif