#ifndef EC2X_GNSS_h
#define EC2X_GNSS_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X.h"

/*typedef struct gpsResp {
	bool fixed;
	uint8_t hh;
	uint8_t mm;
	uint8_t ss;
	uint8_t d;
	uint8_t m;
	uint8_t y;
	
	float  latitude; 
	float  longitude; 
	float hdop;
	float altitude;
	uint8_t fix;
	float  cog;
	float speed_Kmh;
	float speed_knots;
	
	uint8_t nsat;
	
}gpsResp;
*/
#define ALL 		0
#define	TIME 		1
#define	LATITUDE 	2 
#define	LONGTITUDE 	3
#define	HDOP 		4
#define	ALTITUDE 	5
#define	FIX 		6
#define	HEADING 	7
#define	SPEED_Kmh 	8
#define	SPEED_Knots 9
#define	DATE 		10
#define	NSAT 		11




class EC2X_GNSS 
{
	public:
		
		EC2X_GNSS();	
		bool turnOn();
		bool turnOn(uint8_t mode,int fixtime,int fixdist,int fixcnt,int fixrate);
		bool turnOff();
		String position();
		String getData_index(uint8_t num);
		bool fixed(void);	
		
		
	private:
		String lawData;
};

#endif

