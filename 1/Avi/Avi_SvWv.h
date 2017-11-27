#ifndef _AVI_SVWV_H_
#define _AVI_SVWV_H_

void ClearSvWv(void);
void IntoWaveCache(char *pWv,int Len);
DWORD WINAPI SvWaveProc(LPVOID lpParam);
BOOL WriteDatafile( LPCSTR lpFileName);
BOOL WriteDatafileData( LPVOID lpData, UINT dataSize);
void CloseDatafile();
BOOL StopSaveWave(void);
CString CheckWaveDir(char *TmpFloder);
/************************************************************************/
/*波形分车操作                                                          */
/************************************************************************/
void SwCarSpiltProc(int nCmd,int indx);
#endif

