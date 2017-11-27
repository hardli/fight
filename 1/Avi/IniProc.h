#ifndef _INIPROC_H_
#define _INIPROC_H_


bool WriteLogFile(char* msg);
char * GetIniRoot(void);
void ReadAllPara(void);
void ReadModelPara(char *pPath);
void ReadLaserPara(char *pPath);
void ReadSysParam(char *pPath);
void ReadVoutPara(char *pPath);
void ReadWvFilePara(char *pPath);
void ReadCamPara(char *pPath);
void ReadQuePara(char *pPath);


#endif

