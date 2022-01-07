#ifndef EC2X_MQTT_h
#define EC2X_MQTT_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X.h"

#define MQTT_V3_10 3
#define MQTT_V3_11 4

#define MAX_MQTT_BUF 256
#define ALL_MSG 5



class EC2X_MQTT
{
	public:
		EC2X_MQTT(uint8_t connectID);
		void init();
		bool configResp(void);
		bool version(uint8_t ver);
		bool will(bool will_fg,uint8_t will_qos,bool will_retain,String will_topic,String will_msg);
		bool timeOut(uint8_t pkt_timeout,uint8_t retry_times,bool timeout_notice);
		bool session(bool clean_session);
		bool keepalive(uint16_t time);
		bool ssl(bool sslenable,int sslctx_idx=-1);
		bool recvMode(bool msg_recv_mode,bool msg_len_enable);
		bool qmtping(uint8_t qmtping_interval);
		
		bool open(char *host,uint16_t port);
		bool close();
		bool connect(const char* clientID,const char* username,const char* password);
		bool disconnect();
		bool subResp(uint8_t mode=0);
		
		bool sub(char *topic,uint8_t qos,int msgID=1);
		bool sub(String topic,uint8_t qos,int msgID=1){
			return sub((char*)topic.c_str(),qos,msgID);
		};
		bool subs(char *topics,int msgID=1);
		
		bool subs(String topics,int msgID=1){
			return subs((char *)topics.c_str(),msgID);
		};
		
		bool unsub(char *topic,int msgID=1);
		bool unsub(String topic,int msgID=1){
			return unsub((char*)topic.c_str(),msgID);
		};
		
		
		bool pub(char *topic,uint8_t *payload,size_t msg_length,uint8_t qos=0,uint8_t retain=0,uint8_t msgID=1 ,unsigned long tout=5000);
		bool pub(String topic,String payload,uint8_t qos=0,uint8_t retain=0,uint8_t msgID=1 ,unsigned long tout=5000);
		bool pub(char* topic,char* payload,uint8_t qos=0,uint8_t retain=0,uint8_t msgID=1 ,unsigned long tout=5000);
		bool connected(void);
		
		uint8_t available();
		size_t readMessage(uint8_t id,String *topic,uint8_t *payloadBuff,size_t buffSize,unsigned long tout=200);
		uint8_t msgStatus[5];
		
		
		void mqttTask(void);
		void (*callback)(String topic ,uint8_t *payload,size_t length);
		
		
	private:
		uint8_t connectID_;
		
		
};















#endif