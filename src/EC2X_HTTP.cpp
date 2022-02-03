#include "EC2X_HTTP.h"

//#include <MemoryFree.h>;

EC2X_HTTP::EC2X_HTTP(uint8_t connectID)
{
	connectID_ = connectID;
	String p = "RAM:httpID"+String(connectID_)+".html";
	memset(path,0,30);
	strcpy(path,p.c_str());
	//path = p.c_str();
}

bool EC2X_HTTP::config(String cmd,uint8_t mode)
{
	lte.ser.print(F("AT+QHTTPCFG=\""));
	lte.ser.print(cmd);
	lte.ser.print("\",");
	lte.ser.print(mode);
	lte.ser.print("\r");
	uint8_t  res = lte.ser.waitData(WAIT_OK,1000);
	if(res==RES_OK)
	{
		return true;
	}
	return false;
}
bool EC2X_HTTP::setURL(char *url,size_t length,int timeout)
{
	lte.ser.print(F("AT+QHTTPURL="));
	lte.ser.print(length);
	lte.ser.print(",");
	lte.ser.print(timeout);
	lte.ser.print("\r");
	
	uint8_t  res = lte.ser.waitData(F("CONNECT"),1000);
	if(res==RES_MATCH)
	{
		
		lte.ser.write(url,length);
		res = lte.ser.waitData(WAIT_OK,1000);
		if(res==RES_OK)
		{
			lte.ser.flushSerial();
			return true;
		}
	}
	lte.ser.flushSerial();
	return false;
	
}
int EC2X_HTTP::get_(int rspTime)
{
	lte.ser.print(F("AT+QHTTPGET="));
	lte.ser.print(rspTime);
	lte.ser.print("\r");
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QHTTPGET:"),5000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0))
			{
				int ret = getData_index(2).toInt();
				lte.ser.flushSerial();
				return ret;
			}
		}
	}
	Serial.print(F("\r\nError: "));
	Serial.println(lte.ser.resData);
	return 0;
}

bool EC2X_HTTP::saveHttpToRam(int rspTime)
{
	lte.ser.print(F("AT+QHTTPREADFILE="));
	lte.ser.print("\"");
	lte.ser.print(path);
	lte.ser.print("\"");
	lte.ser.print(",");	
	lte.ser.print(rspTime);
	lte.ser.print("\r");
	uint8_t  res = lte.ser.waitData(WAIT_OK,1000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QHTTPREADFILE:"),1000);
		if(res==RES_MATCH)
		{
			if((getData_index(1).toInt()==0))
			{
				lte.ser.flushSerial();
					return true;
			}
		}
	}
	Serial.println(F("\r\nError: "));
	Serial.println(getData_index(2));
	return false;
}

String EC2X_HTTP::getData_index(uint8_t num)
{
	    size_t i=0;
		while(lte.ser.resData[i++]!=' '){}
		char buf[20];
		uint8_t cnt;
		for(uint8_t x=0;x<num;x++)
		{
			cnt=0;
			memset(buf,0,20);
			while(lte.ser.resData[i]!=',' &&lte.ser.resData[i]!=0){buf[cnt++]=lte.ser.resData[i]; i++;}
			//Serial.println(buf);
			i++;
			
		}
	return String(buf);
}
bool EC2X_HTTP::startPost(size_t size,int inputTime,int rsptime)
{
	
	lte.ser.print(F("AT+QHTTPPOST="));
	lte.ser.print(size);
	lte.ser.print(",");
	lte.ser.print(inputTime);
	lte.ser.print(",");
	lte.ser.print(rsptime);
	lte.ser.print("\r");

	uint8_t  res = lte.ser.waitData(F("CONNECT"),80000);
	if(res==RES_MATCH)
		return true;
	Serial.println(lte.ser.resData);
	
	return false;
}
int EC2X_HTTP::stopPost()
{
	uint8_t  res = lte.ser.waitData(WAIT_OK,1000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QHTTPPOST:"),5000);
		if(res==RES_MATCH)
		{
			int ret = getData_index(2).toInt();
			lte.ser.flushSerial();
			return ret;
		}
	}
	return 0;
}
int EC2X_HTTP::post_(uint8_t *data,size_t size,int inputTime,int rsptime)
{
	if(startPost(size,inputTime,rsptime))
	{
		//Serial.println("stream");
		lte.ser.write(data,size);
		return (stopPost());
	}
	return 0;
}
int EC2X_HTTP::postFile(char *path,int rsptime)
{
	lte.ser.print(F("AT+QHTTPPOSTFILE=\""));
	lte.ser.print(path);
	lte.ser.print(F("\","));
	lte.ser.print(rsptime);
	lte.ser.print("\r");
	//+QHTTPPOSTFILE: 0,200,4
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		res = lte.ser.waitData(F("+QHTTPPOSTFILE:"),10000);
		if(res==RES_MATCH)
		{
			int ret = getData_index(2).toInt();
			lte.ser.flushSerial();
			return ret;
		}
	}
	return 0;
		

}
int EC2X_HTTP::request_file(String method,String url,uint8_t *data,size_t length)
{
	requestheader(true);
	char reqPath[16] = "RAM:REQPATH.txt";
	int index = url.indexOf(':');
	int index2 = url.indexOf('/',index+3);
	
	String server = url.substring(0,index2);
	String urlpath(" ");
	urlpath += url.substring(index2);
	String host = url.substring(index+3,index2);
	if(urlpath.length()==0)
		urlpath = "/";	
	/*Serial.println("Method: " + method);
	Serial.println("Server: " + server);
	Serial.println("Path: " + urlpath);
	Serial.println("host " + host);
	*/
	if(!setURL(server))
	{
		Serial.println(F("Set URL: Error"));
		return 0;
	}
	else
		Serial.println(F("Set URL: OK"));
	
	/*if(file.exists(reqPath))
	{	
		file.del(reqPath);
	}
	*/
	
	uint32_t handle = file.open(reqPath);
	if(handle)
	{
		//method.toUpperCase();
		file.print(handle,method);
		file.print(handle,urlpath);
		file.print(handle,F(" HTTP/1.1\r\nHost: "));
		file.print(handle,host);
		file.print(handle, F("\r\nAccept: */*\r\n\
							User-Agent: QUECTEL_MODULE\r\n\
							Connection: Keep-Alive\r\n\
							Content-Type: application/x-www-form-urlencoded\r\n"));

		if((method!=F("GET"))||(method!=F("DELETE")))
		{
			file.print(handle,F("Content-Length: "));
			file.print(handle,String(length)+"\r\n\r\n");
			file.write(handle,data,length);
		}
		else
			file.print(handle, F("\r\n"));
		file.close(handle);
		return postFile(reqPath);
	}
	file.close(handle);
	return 0;		
}
int EC2X_HTTP::request_ram(String method,String url,uint8_t *data,size_t length)
{
	requestheader(true);
	Serial.print("!!Freemem: ");
	//Serial.println(freeMemory());
	int index = url.indexOf(':');
	int index2 = url.indexOf('/',index+3);
	String server = url.substring(0,index2);
	String urlpath = " "+ url.substring(index2);
	String host = url.substring(index+3,index2);
	if(urlpath.length()==0)
		urlpath = "/";
	
	/*Serial.println("Method: " + method);
	Serial.println("Server: " + server);
	Serial.println("Path: " + urlpath);
	Serial.println("host " + host);
	*/
	if(!setURL(server))
	{
		Serial.println(F("Set URL: Error"));
		return 0;
	}
	else
		Serial.println(F("Set URL: OK"));
	
	String httpData = method;
	httpData += urlpath;
	httpData += F(" HTTP/1.1\r\n");
	httpData += F("Host: ");
	httpData +=	host;
	httpData += F("\r\nAccept: */*\r\n");
	httpData +=	F("User-Agent: QUECTEL_MODULE\r\n");
	httpData +=	F("Connection: Keep-Alive\r\n");
	httpData += F("Content-Type: application/x-www-form-urlencoded\r\n");
	if((method!="GET")||(method!="DELETE"))
	{
		httpData += F("Content-Length: ");
		httpData += String(length)+"\r\n\r\n";
		
	}
	else
		httpData += F("\r\n");
	Serial.println(httpData.length());
	size_t size = length+httpData.length();
	/*
	Serial.print("Freemem: ");
	Serial.println(freeMemory());
        */

	if(startPost(size,80,80))
	{
		//Serial.println("stream");
		lte.ser.write(httpData.c_str(),httpData.length());
		lte.ser.write(data,length);
		return (stopPost());
	}
	return 0;	
}
int EC2X_HTTP::get(String url)
{
	requestheader(false);
	if(!setURL(url))
	{
		Serial.println(F("Set URL: Error"));
		return 0;
	}
	return get_();
}
int EC2X_HTTP::post(String url,uint8_t *data,size_t size)
{
	requestheader(false);
	if(!setURL(url))
	{
		Serial.println(F("Set URL: Error"));
		return 0;
	}
	return post_(data,size);
}
int EC2X_HTTP::post(String url,String& data)
{
	requestheader(false);
	if(!setURL(url))
	{
		Serial.println(F("Set URL: Error"));
		return 0;
	}
	return post_(data);
}


