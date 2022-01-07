#ifndef EC2X_HTTP_h
#define EC2X_HTTP_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X.h"
#include "EC2X_File.h"


class EC2X_HTTP 
{
	public:
		
		EC2X_HTTP(uint8_t connectID);	
		bool config(String cmd,uint8_t mode);
		bool contextid(uint8_t id){return config(F("contextid"),id);};
		bool requestheader(bool en){return config(F("requestheader"),en);};
		bool sslctxid(uint8_t id){return config(F("sslctxid"),id);};
		bool contenttype(uint8_t id){return config(F("contenttype"),id);};
		bool rspoutAuto(bool en){return config(F("rspout/auto"),en);};
		bool closedInd(bool en){return config(F("closed/ind"),en);};
				
		bool setURL(char *url,size_t length,int timeout=60);
		bool setURL(char *url,int timeout=60){return setURL(url,strlen(url),timeout);};
		bool setURL(String& url,int timeout=60){return setURL(url.c_str(),url.length(),timeout);};
		int get_(int rspTime=80);
		bool saveHttpToRam(int rspTime=160);
		bool startPost(size_t size,int inputTime=60,int rsptime=160);
		int 	stopPost();
		int post_(uint8_t *data,size_t size,int inputTime=60,int rsptime=160);
		int post_(String& data,int inputTime=60,int rsptime=160){return post_(data.c_str(),data.length(),inputTime,rsptime);};
		int postFile(char *path,int rsptime=160);
		int request_file(String method,String url,uint8_t *data,size_t length);
		int request_ram(String method,String url,uint8_t *data,size_t length);
		
		int get(String url);
		
		int post(String url,uint8_t *data,size_t size);
		int post(String url,String& data);
		
		int put(String url,uint8_t *data,size_t size)
		{ 
			return request_file(F("PUT"),url,data,size);
		};
		int put(String url,String& data)
		{ 
			return request_file(F("PUT"),url,data.c_str(),data.length());
		};
		int del(String url,uint8_t *data,size_t size){ return request_file(F("DELETE"),url,data,size);};
		int del(String url,String& data){ return request_file(F("DELETE"),url,data.c_str(),data.length());};
		int request(uint8_t *data,size_t size){requestheader(true); return post_(data,size);};
		int request(String &data){
			requestheader(true); 
			return post_(data);
		};
		
		
		
		EC2X_FILE file;
		String getData_index(uint8_t num);
		char path[30];
		
	private:
		uint8_t connectID_;
		
};

#endif

