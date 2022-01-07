#ifndef EC2X_FILE_h
#define EC2X_FILE_h

#include "Arduino.h"
#include <Stream.h>
#include "EC2X.h"

#define open_RW 		0
#define overWrite_RW 	1
#define readOnly_RO 	2

#define BEGIN 		0
#define CURRENT 	1
#define END 		2
class EC2X_FILE
{
	public:
		EC2X_FILE();
		unsigned long  QFLDS(char *path,bool m); //0 =free size,1=total size
		unsigned long  totalSize(char *path){return QFLDS(path,true);};
		unsigned long freeSize(char *path){return QFLDS(path,false);};
		bool uploadFile(char *path,uint8_t *data,size_t size);
		void list(char* path);
		int size(char *path);
		bool exists(char *path);
		uint16_t open(char* path,uint8_t mode=0);// 0 = Create,open,R/W , 1 = Create,overWrite,R/W , 2 = readOnly RO
		bool close(uint32_t handle);
		size_t read(uint32_t handle,uint8_t *data,size_t len);
		size_t write(uint32_t handle,uint8_t *data,size_t len,uint8_t tout=10);
		size_t print(uint32_t handle,String data){return write(handle,data.c_str(),data.length());};
		bool seek(uint32_t handle,size_t offset,uint8_t position=0); //position-> 0= begin , 1= current pointer , 2 = end of file 
		size_t position(uint32_t handle);
		bool truncate(uint32_t handle);
		bool move(char *source,char * destination,bool copy,bool overwrite);
		bool cut(char *source,char * destination){return move(source,destination,false,true);};
		bool copy(char *source,char * destination){return move(source,destination,true,true);};
		bool del(char* path);
		
		
	private:
		
};




































#endif