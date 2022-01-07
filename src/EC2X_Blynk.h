#ifndef EC2x_BLYNK_h
#define EC2x_BLYNK_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "EC2x 4G LTE"
#endif
#ifndef BLYNK_HEARTBEAT
#define BLYNK_HEARTBEAT 1000
#endif

#ifndef BLYNK_TIMEOUT_MS
#define BLYNK_TIMEOUT_MS 2000
#endif

#define BLYNK_SEND_ATOMIC


#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include "EC2X.h"
#include "EC2X_TCP.h"


#define BLYNK_CONNECT_ID 11 

class BlynkTransport4G
{
public:
	BlynkTransport4G()
	{
		blynkClient = new EC2X_TCP(BLYNK_CONNECT_ID);
	}
	int connect() 
	{
		BLYNK_LOG1(BLYNK_F("Connect"));
		return blynkClient->open(host,port,0);
	}
	void begin(const char* h, int p) 
	{
		 host = h;
         port = p;
    }
	void disconnect() 
	{ 
		BLYNK_LOG1(BLYNK_F("Disconnect"));
		blynkClient->close();
	}
	size_t read(void* buf, size_t len) 
	{
		return blynkClient->readRxBuffer((uint8_t*)buf,len);
    }
	 size_t write(const void* buf, size_t len) 
	{
		/*for(uint8_t i=0;i<len;i++)
		{
			uint8_t x = buf++;
			Serial.print(x,16);
			Serial.print(" ");
		}
		*/
		Serial.println("000");
		Serial.write((uint8_t*)buf,len);
		Serial.println("111");
		return blynkClient->send((uint8_t*)buf,len);
    }
	bool connected() 
	{ 
		return blynkClient->connected(); 
	}
    int available() 
	{ 
		return blynkClient->getRxSize();
	}
	
	
private:
   EC2X_TCP *blynkClient;
   const char* host;
   uint16_t port;
};

class Blynk4G_EC2x : public BlynkProtocol<BlynkTransport4G>
{
	typedef BlynkProtocol<BlynkTransport4G> Base;
	
	public:
	
	Blynk4G_EC2x(BlynkTransport4G& transp): Base(transp)
	{
		
	}
	void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
    }
	
	void begin( const char* auth,
				const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
	{
		  //Serial.print("Begin");
		config(auth, domain,port);
	}
	
};

static BlynkTransport4G _blynkTransport;
Blynk4G_EC2x Blynk(_blynkTransport);
#include <BlynkWidgets.h>

















#endif