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
	void init();
	bool messageService(bool service=false);
	bool messageFormat(bool format=TEXT);
	bool messageShowText(bool enable);
	bool messageTextMode(int fo=17,int vp=167,int pid=0,int dcs=8);
	String serviceCenterAddress();
	bool messageStorage(String &mem);
	int  lastMessageID();
	bool deleteMessage(int index,uint8_t delflag=0);
	bool deleteAllMessage(){return deleteMessage(1,4);};
	String getSenderNumber(int index);
	String getTime(int index);
	String getMessage(int index);
	String readMsg_(int msg_index,char val,size_t from);
	
	bool startSend(String number);
	bool endSend();
	int convetUnicodeToUTF8(char *data);
	void printUnicode(String str);
	
};



#endif