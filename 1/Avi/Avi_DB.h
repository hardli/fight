#ifndef _AVI_DB_H_
#define _AVI_DB_H_

#include "Avi_Comm.h"


//#import  "C:\Program Files\Common Files\System\ado\msado15.dll"  rename("EOF","adoEOF")  

//主要是高版本windows与低版本windowsADO库版本不同导致的。
//#import "C:\ProgramFiles\Common Files\System\ado\msado15.dll" no_namespacerename("EOF","adoEOF")rename("BOF","adoBOF")
#if defined(ADO2_OLD)
  #pragma message("Using ADO2 TLB")
  #import "C:\Program Files\Common Files\System\ado\msado20.tlb" rename("EOF","adoEOF")
#else  
  #pragma message("Using Latest ADO")
  #import "C:\Program Files\Common Files\System\ado\msado15.dll" rename("EOF","adoEOF")
#endif



#import "C:\Program Files\Common Files\System\ado\msadox.dll" rename_namespace("ADOX") rename("EOF","adoxEOF") //no_namespace

BOOL CreateDB(LPCTSTR  lpszFile);
BOOL OpenMdb(void);
void Add2Db(VehSet TmpVeh);
void Add2Txt(VehSet TmpVeh);

#endif