#include "EC2X_Audio.h"


EC2X_AUDIO::EC2X_AUDIO()
{
	
}
bool EC2X_AUDIO::volumn(uint8_t vol)
{
	lte.ser.print(F("AT+CLVL="));
	lte.ser.print(vol);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO::mute(bool enable)
{
	lte.ser.print(F("AT+CMUT="));
	lte.ser.print(enable);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO::loop(bool enable)
{
	lte.ser.print(F("AT+QAUDLOOP="));
	lte.ser.print(enable);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO::dtmfGeneration(String str,int duration)
{
	lte.ser.print(F("AT+VTS=\""));
	lte.ser.print(str);
	lte.ser.print(F("\","));
	lte.ser.print(duration);
	lte.ser.print(F("\r"));
	return waitOK();
	
}
bool EC2X_AUDIO::toneDuration(int duration,int interval)
{
	lte.ser.print(F("AT+VTD="));
	lte.ser.print(duration);
	lte.ser.print(F(","));
	lte.ser.print(interval);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO::audioMode(uint8_t mode)
{
	lte.ser.print(F("AT+QAUDMOD="));
	lte.ser.print(mode);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO::audioInterface(uint8_t io,bool mode,bool fsync,uint8_t clk,uint8_t format,uint8_t sample,uint8_t numslot,uint8_t slotmapping)
{
	lte.ser.print(F("AT+QDAI="));
	lte.ser.print(io);
	lte.ser.print(F(","));
	lte.ser.print(mode);
	lte.ser.print(F(","));
	lte.ser.print(fsync);
	lte.ser.print(F(","));
	lte.ser.print(clk);
	lte.ser.print(F(","));
	lte.ser.print(format);
	lte.ser.print(F(","));
	lte.ser.print(sample);
	lte.ser.print(F(","));
	lte.ser.print(numslot);
	lte.ser.print(F(","));
	lte.ser.print(slotmapping);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO:: echoCancellation(uint8_t index,int value)
{
	lte.ser.print(F("AT+QEEC="));
	lte.ser.print(index);
	lte.ser.print(F(","));
	lte.ser.print(value);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO::sideToneGain(uint8_t vol)
{
	lte.ser.print(F("AT+QSIDET="));
	lte.ser.print(vol);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO:: uplinkGainsMic(uint8_t txgain,int txdgain)
{
	lte.ser.print(F("AT+QMIC="));
	lte.ser.print(txgain);
	lte.ser.print(F(","));
	lte.ser.print(txdgain);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO:: downlinkGainsRX(uint8_t rxgain)
{
	lte.ser.print(F("AT+QRXGAIN="));
	lte.ser.print(rxgain);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO::dtmfDetect(bool enable)
{
	lte.ser.print(F("AT+QTONEDET="));
	lte.ser.print(enable);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO:: playLocalDtmf(int n,String strdtmf)
{
	lte.ser.print(F("AT+QLDTMF="));
	lte.ser.print(n);
	lte.ser.print(F(",\""));
	lte.ser.print(strdtmf);
	lte.ser.print(F("\"\r"));
	return waitOK();
}
bool EC2X_AUDIO:: stopLocalDtmf()
{
	lte.ser.print(F("AT+QLDTMF\r"));
	return waitOK();
}
bool EC2X_AUDIO:: playDtmfFarEnd(bool ulmute,bool dlmute,String dtmf,int duration,int pause)
{
	lte.ser.print(F("AT+QWDTMF="));
	lte.ser.print(ulmute);
	lte.ser.print(F(","));
	lte.ser.print(dlmute);
	lte.ser.print(F(",\""));
	lte.ser.print(dtmf);
	lte.ser.print(F("\","));
	lte.ser.print(duration);
	lte.ser.print(F(","));
	lte.ser.print(pause);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO:: playDtmfFarEndStatus()
{
	lte.ser.print(F("AT+QWDTMF?\r"));
	uint8_t  res = lte.ser.waitData(F("+QWDTMF:"),5000);
	if(res==RES_MATCH)
	{
		int ret = lte.ser.getData_index(1).toInt();
		lte.ser.flushSerial();
		return ret;
	}
		lte.ser.flushSerial();
	return false;
	
}
bool EC2X_AUDIO::record(bool ctrl,String filename,uint8_t format,bool dlink)
{
	lte.ser.print(F("AT+QAUDRD="));
	lte.ser.print(ctrl);
	lte.ser.print(F(",\""));
	lte.ser.print(filename);
	lte.ser.print(F("\","));
	lte.ser.print(format);
	lte.ser.print(F(","));
	lte.ser.print(dlink);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO::startRecord(String file,uint8_t format)
{
	return record(true,file,format);
}
bool EC2X_AUDIO::stopRecord()
{
	lte.ser.print(F("AT+QAUDRD=0\r"));
	return waitOK();
}
bool EC2X_AUDIO::play(bool ctrl,String file,bool repleat,bool ulmute,bool dlmute)
{
	lte.ser.print(F("AT+QPSND="));
	lte.ser.print(ctrl);
	lte.ser.print(F(",\""));
	lte.ser.print(file);
	lte.ser.print(F("\","));
	lte.ser.print(repleat);
	lte.ser.print(F(","));
	lte.ser.print(ulmute);
	lte.ser.print(F(","));
	lte.ser.print(dlmute);
	lte.ser.print(F("\r"));
	return waitOK();
}
bool EC2X_AUDIO::playStatus()
{
	lte.ser.print(F("AT+QPSND?\r"));
	uint8_t  res = lte.ser.waitData(F("+QPSND:"),5000);
	if(res==RES_MATCH)
	{
		int ret = lte.ser.getData_index(1).toInt();
		lte.ser.flushSerial();
		return ret;
	}
		lte.ser.flushSerial();
	return false;
}
bool EC2X_AUDIO::playText(uint8_t mode,String text)
{
	lte.ser.print(F("AT+QTTS="));
	lte.ser.print(mode);
	lte.ser.print(F(",\""));
	lte.ser.print(text);
	lte.ser.print(F("\"\r"));
	return waitOK();
}
bool EC2X_AUDIO::stopText()
{
	lte.ser.print(F("AT+QTTS=0\r"));
	return waitOK();
}
bool EC2X_AUDIO::playTextStatus()
{
	lte.ser.print(F("AT+QTTS?\r"));
	uint8_t  res = lte.ser.waitData(F("+QTTS:"),5000);
	if(res==RES_MATCH)
	{
		int ret = lte.ser.getData_index(1).toInt();
		lte.ser.flushSerial();
		return ret;
	}
		lte.ser.flushSerial();
	return false;
	
}

bool EC2X_AUDIO::playTextFarEnd(bool ulmute,bool dlmute,uint8_t mode,String text)
{
	lte.ser.print(F("AT+QWTTS="));
	lte.ser.print(ulmute);
	lte.ser.print(F(","));
	lte.ser.print(dlmute);
	lte.ser.print(F(","));
	lte.ser.print(mode);
	lte.ser.print(F(",\""));
	lte.ser.print(text);
	lte.ser.print(F("\"\r"));
	return waitOK();
}
bool EC2X_AUDIO::playTextFarEndStatus()
{
	lte.ser.print(F("AT+QWTTS?\r"));
	uint8_t  res = lte.ser.waitData(F("+QWTTS:"),5000);
	if(res==RES_MATCH)
	{
		int ret = lte.ser.getData_index(1).toInt();
		lte.ser.flushSerial();
		return ret;
	}
		lte.ser.flushSerial();
	return false;
	
}
bool EC2X_AUDIO::stopTextFarEnd()
{
	lte.ser.print(F("AT+QWTTS=1,0,0\r"));
	return waitOK();
}

bool EC2X_AUDIO::waitOK()
{
	uint8_t  res = lte.ser.waitData(WAIT_OK,5000);
	if(res==RES_OK)
	{
		lte.ser.flushSerial();
		return true;
	}
		lte.ser.flushSerial();
	return false;
}




