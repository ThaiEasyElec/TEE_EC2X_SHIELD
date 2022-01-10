#ifndef EC2X_h
#define EC2X_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X_ser.h"

#define TCP_CONTEXT 1

#define SSL3_0 	0
#define TLS1_0 	1
#define TLS1_1 	2
#define TLS1_2 	3
#define ALL 	4

#define TCPS_CONTEXT 1

#define No_AUTH 			0
#define SERVER_AUTH 		1
#define SERVER_CLIENT_AUTH 	2

#define RINGING 		3
#define CALL_PROGRESS 	4

#define IDLE 		-1
#define ACTIVE 		0
#define HELD		1
#define DIALING 	2
#define ALERTING	3 
#define INCOMMING	4
#define WAITING		5


class SSL
{
	public:
	SSL(){};
	bool sslConfig(const __FlashStringHelper *config,uint8_t SSL_id,char * mode);
	bool sslConfig(const __FlashStringHelper *config,uint8_t SSL_id,uint16_t mode);
	bool sslversion(uint8_t SSL_id,uint8_t ver){return sslConfig(F("sslversion"),SSL_id,ver);};
	bool ciphersuite(uint8_t SSL_id,char *ver){return sslConfig(F("ciphersuite"),SSL_id,ver);};
	bool seclevel(uint8_t SSL_id,uint8_t level){return sslConfig(F("seclevel"),SSL_id,level);};
	bool cacer(uint8_t SSL_id,char *path){return sslConfig(F("cacert"),SSL_id,path);};
	bool clientcer(uint8_t SSL_id,char *path){return sslConfig(F("clientcer"),SSL_id,path);};
	bool clientkey(uint8_t SSL_id,char *path){return sslConfig(F("clientkey"),SSL_id,path);};
	bool ignorelocaltime(uint8_t SSL_id,uint16_t time){return sslConfig(F("ignorelocaltime"),SSL_id,time);};
	bool negotiatetime(uint8_t SSL_id,uint16_t time){return sslConfig(F("negotiatetime"),SSL_id,time);};
	bool sni(uint8_t SSL_id,uint8_t level){return sslConfig(F("sni"),SSL_id,level);};	
};


class EC2X
{
	public:
	EC2X();
	LTE_Serial ser;
	void begin(Stream *ser_,int reset=-1);
	void reset();
	bool ready(unsigned long tout=0);
	bool simStatus();
	String IMSI();
	String IMEI();
	String ICCID();
	bool networkRegister();
	
	String getOperator_();
	String getOperator();
	int signal();
	String networkInfo();
	bool connectInternet();
	bool disconnectInternet();
	String IPAddress();
	bool internetStatus();
	String getTime();
	String setNTP(char *server,int port=123);
	bool ping(String server , Stream *ser_);
	String ping(String server);
	bool setDNS(String pridnsaddr);
	String getIPbyDomain(String domain);
	
	bool call(char *number);
	bool disconnect();
	bool answer();
	uint8_t activityStatus();
	String currentCall();
	int callStatus();
	String incommingNumber(){return currentCall();};
	bool characterSet(char *chset);
	bool ussd(char *str_ussd);
	SSL ssl; 
	
	String NET_APN;
	String NET_USERNAME;
	String NET_PASSWORD;
	
	int ramAvailable();
	private:
		int rstPin; 
		int callStatus_;
	
};

extern EC2X lte;











#endif