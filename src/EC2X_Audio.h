#ifndef EC2X_AUDIO_h
#define EC2X_AUDIO_h

#include "Arduino.h"
#include "EC2X.h"

class  EC2X_AUDIO
{
	public:
	
	EC2X_AUDIO();
	bool volumn(uint8_t vol);
	bool mute(bool enable);
	bool loop(bool enable);
	bool dtmfGeneration(String str,int duration);
	bool toneDuration(int duration=3,int interval=0);
	bool audioMode(uint8_t mode=0);
	bool audioInterface(uint8_t io=5,bool mode=false,bool fsync=false,uint8_t clk=4,uint8_t format=0,uint8_t sample=0,uint8_t numslot=1,uint8_t slotmapping=1);
	bool echoCancellation(uint8_t index,int value);
	bool sideToneGain(uint8_t vol);
	bool uplinkGainsMic(uint8_t txgain,int txdgain);
	bool downlinkGainsRX(uint8_t rxgain);
	bool dtmfDetect(bool enable);
	bool playLocalDtmf(int n,String strdtmf);
	bool stopLocalDtmf();
	bool playDtmfFarEnd(bool ulmute,bool dlmute,String dtmf,int duration,int pause);
	bool playDtmfFarEndStatus();
	
	bool record(bool ctrl,String filename,uint8_t format=13,bool dlink=false);
	bool startRecord(String file,uint8_t format=13);
	bool record();
	bool stopRecord();
	
	bool play(bool ctrl,String file,bool repleat,bool ulmute,bool dlmute);
	bool playStatus();
	
	bool playText(uint8_t mode,String text);
	bool stopText();
	bool playTextStatus();
	
	bool playTextFarEnd(bool ulmute,bool dlmute,uint8_t mode,String text);
	bool playTextFarEndStatus();
	bool stopTextFarEnd();
	




	
	
	
	
	
	
	
	bool waitOK();
	
};

#endif