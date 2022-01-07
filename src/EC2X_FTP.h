#ifndef EC2X_FTP_h
#define EC2X_FTP_h

#include "EC2X.h"
#include "EC2X_File.h"

#define BINARY 0
#define ASCII  1
#define ACTIVE 0
#define PASSIVE  1

#define FTP_TEMP "RAM:ftpTemp.txt"
class LTE_FTP 
{
	public:
		LTE_FTP();
		bool open(char *user ,char *password,char *server,int port);
		
		bool config(char *cmd ,uint8_t data);
		bool fileType(bool type){return config("filetype",type);};
		bool transMode(bool mode){return config("transmode",mode);};
		bool contextid(uint8_t id){return config("contextid",id);};
		bool rsptimeout(uint8_t time){return config("rsptimeout",time);};
		bool ssltype(uint8_t type){return config("ssltype",type);};
		bool data_address(bool add){return config("data_address",add);};
		bool cd(char *path);
		bool cd(String& path){return cd(path.c_str());};
		String pwd();
		bool mkdir(char *path);
		bool mkdir(String& path){return mkdir(path.c_str());};
		
		bool deldir(char *file);
		bool deldir(String& file){return deldir(file.c_str());};
		
		void ls(char *path);
		void ls(String& path){return ls(path.c_str());};
		
		bool ls(char *path,char *save);
		bool ls(String& path,char *save){return ls(path.c_str(),save);};
		
		bool startPut(char* filename,char *sourse,size_t address=0,size_t length=0,bool lastData=false);
		bool endPut();
		bool putFromBuffer(char* filename,size_t address,uint8_t *buf,size_t length=0,bool lastData=false);
		bool putFromBuffer(String& filename,size_t address,uint8_t *buf,size_t length=0,bool lastData=false){
			return putFromBuffer(filename.c_str(),address,buf,length,lastData);
		};
		bool putFromFile(char* filename,char *path);
		size_t size(char *path);
		size_t size(String &path){return size(path.c_str());};
		bool ftpget(char* filename,char *sourse,size_t address,size_t length);
		size_t get(char *path,char *saveTo,size_t address);
		size_t get(String& path,char *saveTo,size_t address=0){
			return get(path.c_str(),saveTo,address);
		};
		
		bool rename(char *oldName,char *newName);
		bool rename(String& oldName,String& newName){
			return rename(oldName.c_str(),newName.c_str());
		}
		String modify(char *file);
		String modify(String& file){return modify(file.c_str());};
		bool del(char *file);
		bool del(String& file){
			return del(file.c_str());
		}
		
		
		bool logout();
		EC2X_FILE file;
		
		String getData_index(uint8_t num);
		

};


















































#endif