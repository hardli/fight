#include "stdafx.h"
#include "Avi_DB.h"
#include "IniProc.h"
#include "Avi_Main.h"
#include <afxdisp.h>


BOOL OpenMdb(void)
{
    char a_cMdbPath[256]; //mdb文件路径
    char a_cConnstr[512]; //连接字符串
    char a_cCheckPath[256];
	
    CFileFind OneFile;
    BOOL beWork;
	
    memset(a_cMdbPath,0,256);
    memset(a_cConnstr,0,512);
    memset(a_cCheckPath,0,256);
	
    //strcpy(a_cConnstr,"driver={Microsoft Access Driver (*.mdb)};dbq=");
	
	GetMDBRoot(a_cMdbPath);
	//    if(!::GetCurrentDirectory(256,a_cMdbPath))
	//   {
	//      WriteLogFile(_T("获取数据库路径失败!"));
	// }
    //strcpy(a_cCheckPath,a_cMdbPath);
    if(a_cMdbPath[strlen(a_cMdbPath)-1]!='\\')
    {
        a_cMdbPath[strlen(a_cMdbPath)]='\\';
    }
	
    strcat(a_cMdbPath,"Avi.mdb");
    beWork= OneFile.FindFile( a_cMdbPath);
    if(beWork==0)
    {
        WriteLogFile("没有找到数据库，即将创建Avi.mdb");
        BOOL result=CreateDB(a_cMdbPath);
		return result;
    }
    else
    {
		try
		{
			strcat(a_cConnstr,a_cMdbPath);
			CoInitialize(NULL);
			ADODB::_ConnectionPtr pConn(__uuidof(ADODB::Connection));//产生connection智能指针
			ADODB::_RecordsetPtr pRst(__uuidof(ADODB::Recordset)); //产生recordset智能指针
			pConn->Open(a_cConnstr, "","",ADODB::adModeUnknown); //打开数据库
			pRst->Open("select * from VelInfor",_variant_t((IDispatch*)pConn),ADODB::adOpenStatic,ADODB::adLockOptimistic,ADODB::adCmdText);
			
			//nTemp1 = pRst->GetRecordCount();
			
			pRst->Close();
			pConn->Close();
			pRst.Release();
			pConn.Release();
			CoUninitialize();
		}
		catch (_com_error &e) {
			WriteLogFile(e.Description());
			return FALSE;
		}
    }
	return true;
}


BOOL CreateDB(LPCTSTR  lpszFile)
{
	try
    {
		CoInitialize(NULL);// 

		//ADODB::_ConnectionPtr tempConnn;
		
		ADOX::_CatalogPtr pCatalog(__uuidof(ADOX::Catalog));
		ADOX::_TablePtr  pTable = NULL;
		ADOX::_IndexPtr pIndexNew  = NULL;
		ADOX::_IndexPtr pIndex  = NULL;
		CString strConnect;
		CString strDBPath=lpszFile;
		strConnect.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s"),strDBPath);
		COleVariant Connect(strConnect);

	
        pCatalog.CreateInstance(_T("ADOX.Catalog"));
        pCatalog->Create((LPCTSTR)strConnect);//创建数据库
        //tempConnn.CreateInstance(_T("ADODB.Connection"));
//        tempConnn->PutCommandTimeout(15);
//        tempConnn->PutConnectionTimeout(15);
        //tempConnn->put_CursorLocation(ADODB::adUseClient);
        //tempConnn->Open(_bstr_t(strConnect),_bstr_t(),_bstr_t(),ADODB::adConnectUnspecified);
        //pCatalog->PutActiveConnection(_variant_t((IDispatch *) tempConnn));
        pTable.CreateInstance(_T("ADOX.Table"));
        pTable->ParentCatalog =pCatalog;
        pTable->Name="VelInfor";
        ADOX::ColumnsPtr pCols =pTable->Columns;
        pCols->Append(_T("编号")  ,ADOX::adInteger,0);//自动编号字段
        pCols->Append(_T("车序号")  ,ADOX::adInteger,0);//车序号
        pCols->Append(_T("车型")   ,ADOX::adWChar,0);//文本字段
		pCols->Append(_T("方向")   ,ADOX::adInteger,0);
        pCols->Append(_T("轴数")  ,ADOX::adInteger,0);
        pCols->Append(_T("原始轴型")  ,ADOX::adWChar,0);
		pCols->Append(_T("通用轴型")  ,ADOX::adInteger,0);
		pCols->Append(_T("车牌号码")  ,ADOX::adWChar,0);
		pCols->Append(_T("出车类型")  ,ADOX::adWChar,0);
        pCols->Append(_T("检测日期")  ,ADOX::adDate,0);//日期字段
        pCols->Append(_T("检测时间")  ,ADOX::adDate,0);//日期字段
		pCols->Append(_T("判定码")  ,ADOX::adInteger,0);
		pCols->Append(_T("调整码")  ,ADOX::adInteger,0);
		pCols->Append(_T("专车码")  ,ADOX::adInteger,0);
		pCols->Append(_T("底盘高")  ,ADOX::adInteger,0);
		pCols->Append(_T("车头高")  ,ADOX::adInteger,0);
		pCols->Append(_T("车高")  ,ADOX::adInteger,0);
        pCols->Append(_T("车斗")  ,ADOX::adInteger,0);
        pCols->Append(_T("头高跳"), ADOX::adInteger,0);
        pCols->Append(_T("尾高跳")  ,ADOX::adInteger,0);
        pCols->Append(_T("载货")  ,ADOX::adInteger,0);
        pCols->Append(_T("货箱")  ,ADOX::adInteger,0);
		pCols->Append(_T("货车尾")  ,ADOX::adInteger,0);
        pCols->Append(_T("掷地")  ,ADOX::adInteger,0);
        pCols->Append(_T("封闭车窗")  ,ADOX::adInteger,0);
        pCols->Append(_T("玻璃车窗")  ,ADOX::adInteger,0);
		pCols->Append(_T("侧面一致")  ,ADOX::adInteger,0);
		pCols->Append(_T("低值计数")  ,ADOX::adInteger,0);
		pCols->Append(_T("高值计数")  ,ADOX::adInteger,0);
		pCols->Append(_T("离开计数")  ,ADOX::adInteger,0);
		pCols->Append(_T("备注")  ,ADOX::adWChar,0);
        pCatalog->Tables->Refresh();
        long lCount=pCols->Count;
        for(long i=0; i<lCount; i++)
        {
            pCols->GetItem(i)->ParentCatalog =pCatalog;//重要！设置Catalog,参见Q201826 PRB: Error 3265 When You Access Properties Collection
            ADOX::PropertiesPtr pProperties=pCols->GetItem(i)->Properties;
			
        }
        pCols->GetItem(_T("编号"))->Properties->GetItem(_T("Description"))->Value=_T("车辆编号");//注释
        pCols->GetItem(_T("编号"))->Properties->GetItem(_T("AutoIncrement"))->Value=true;//自动编号
		pCols->GetItem(_T("车型"))->Properties->GetItem(_T("Jet OLEDB:Compressed UniCode Strings"))->Value=true;
        pCols->GetItem(_T("原始轴型"))->Properties->GetItem(_T("Jet OLEDB:Compressed UniCode Strings"))->Value=true;
		pCols->GetItem(_T("出车类型"))->Properties->GetItem(_T("Jet OLEDB:Compressed UniCode Strings"))->Value=true;
		pCols->GetItem(_T("车牌号码"))->Properties->GetItem(_T("Jet OLEDB:Compressed UniCode Strings"))->Value=true;
		pCols->GetItem(_T("备注"))->Properties->GetItem(_T("Jet OLEDB:Compressed UniCode Strings"))->Value=true;
        pCatalog->Tables->Append(_variant_t((IDispatch*)pTable));//添加表
        //MessageBox("zheli");
        pCatalog->Tables->Refresh();//刷新
        pIndexNew.CreateInstance(_T("ADOX.Index"));
        pIndexNew->Name = _T("编号");//索引名称
        pIndexNew->Columns->Append(_T("编号"),ADOX::adInteger,0);//索引字段
        pIndexNew->PutPrimaryKey(-1);//主索引
        pIndexNew->PutUnique(-1);//唯一索引
        pTable->Indexes->Append(_variant_t ((IDispatch*)pIndexNew));//创建索引
        pIndexNew=NULL;
        pCatalog->Tables->Refresh();//刷新
		CoUninitialize();//
		return TRUE;
	}
	catch(_com_error &e)
	{
		// ESErrPrintProviderError(tempConnn);
		//ESErrPrintComError(e);
		//WriteLogFile(e.Description());
		_bstr_t bstrSource(e.Source()); 
		_bstr_t bs = _bstr_t( " Error: ") + _bstr_t(e.Error()) + _bstr_t( " Msg: ") 
		+ _bstr_t(e.ErrorMessage()) + _bstr_t( " Description: ") 
		+ _bstr_t(e.Description())+bstrSource; 
		WriteLogFile(bs);
		WriteLogFile("创建数据库出错");
        return FALSE;
    }
    catch(...)
    {
		WriteLogFile("创建数据库异常");
    }
    return FALSE;
	
}

void Add2Db(VehSet TmpVeh)
{
	int i=0;
	int RowIndx=0;
	int CowIndx=1;
	CString StrTmp="";
	CString sTmp="";
	CString   str="";
	BYTE   Cartype;
	char  strtmp;
	
	char a_cMdbPath[256]={0}; //mdb文件路径 
	char a_cConnstr[512]={0}; //连接字符串
	CFileFind OneFile; 
	BOOL beWork;
	
	strcpy(a_cConnstr,"driver={Microsoft Access Driver (*.mdb)};dbq=");  
	//	if(!::GetCurrentDirectory(256,a_cMdbPath))  
	//	{  
	//		WriteLogFile("获取数据库路径失败!");			
	//	}  
	GetMDBRoot(a_cMdbPath);
	if(a_cMdbPath[strlen(a_cMdbPath)-1]!='\\') 
	{
		a_cMdbPath[strlen(a_cMdbPath)]='\\';
	}
	strcat(a_cMdbPath,"Avi.mdb"); 
	beWork= OneFile.FindFile( a_cMdbPath);
	if(beWork==0)
	{
		
	}
	else
	{
		
		try
		{
			strcat(a_cConnstr,a_cMdbPath);//把后者所指字符串添加到前者结尾处(覆盖前者结尾处的'\0')并添加'\0'  
			
			CoInitialize(NULL);  
			ADODB::_ConnectionPtr pConn(__uuidof(ADODB::Connection));//产生connection智能指针
			ADODB::_RecordsetPtr pRst(__uuidof(ADODB::Recordset)); //产生recordset智能指针  
			pConn->Open(a_cConnstr, "","",ADODB::adModeUnknown); //打开数据库  
			pRst->Open("select * from VelInfor",_variant_t((IDispatch*)pConn),ADODB::adOpenStatic,ADODB::adLockOptimistic,ADODB::adCmdText);  
			pRst->AddNew(); 
			
			
			StrTmp.Format("%d",TmpVeh.VehId);
			pRst->PutCollect("车序号",_variant_t(StrTmp)); ;//车序号
			
			//写入数据库
			
			if(TmpVeh.VPattern>=1 && TmpVeh.VPattern<=4)
			{
				str="客";
				Cartype=TmpVeh.VPattern;
				
			}
			else if(TmpVeh.VPattern>=5 && TmpVeh.VPattern<=9)
			{
				str="货";
				Cartype=TmpVeh.VPattern-4;
				
			}
			else if(TmpVeh.VPattern==10 || TmpVeh.VPattern==11)
			{
				str="集装箱";
				Cartype=TmpVeh.VPattern-9;
			}
			else
				;
			if(TmpVeh.VSpeCode==201)
				StrTmp="行人";
			else
			{
				StrTmp.Format("%d",Cartype);
				StrTmp=str+StrTmp;
			}
			pRst->PutCollect("车型",_variant_t(StrTmp));//车型
			
			
			StrTmp.Format("%d",TmpVeh.VDir);
			pRst->PutCollect("方向",_variant_t(StrTmp));
			
			
			
			StrTmp.Format("%d",TmpVeh.AxleNum);
			pRst->PutCollect("轴数",_variant_t(StrTmp));//轴数
			
			StrTmp="";
			for (i=0;i<TmpVeh.AxleNum;i++)
			{
				sTmp.Format("%d",TmpVeh.AxleType[i]);
				StrTmp+=sTmp;
			}
			pRst->PutCollect("原始轴型",_variant_t(StrTmp));//轴型
			StrTmp="";
			
			StrTmp.Format("%d",TmpVeh.TranAxleType);
			pRst->PutCollect("通用轴型",_variant_t(StrTmp));//轴数
			
			if (TmpVeh.VoutFlag==0)
			{
				StrTmp="正常出车";
			}
			else if (TmpVeh.VoutFlag==1)
			{
				StrTmp="强制成功";
			}
			else if (TmpVeh.VoutFlag==2)
			{
				StrTmp="强制失败";
			}
			pRst->PutCollect("出车类型",_variant_t(StrTmp));//出车类型
			
			
			StrTmp.Format("%s",TmpVeh.Plate);//车牌号码
			pRst->PutCollect("车牌号码",_variant_t(StrTmp));//出车类型
			
			
			SYSTEMTIME l_stime = {0};
			GetLocalTime(&l_stime);//获得当前本地时间
			
			StrTmp.Format("%4d/%02d/%02d",l_stime.wYear,l_stime.wMonth,l_stime.wDay);
			pRst->PutCollect("检测日期",_variant_t(StrTmp));//检测日期
			
			StrTmp.Format("%02d:%02d:%02d",l_stime.wHour,l_stime.wMinute,l_stime.wSecond);
			pRst->PutCollect("检测时间",_variant_t(StrTmp));//显示系统时间
			
			
			
			StrTmp.Format("%d",TmpVeh.VJudCode);
			pRst->PutCollect("判定码",_variant_t(StrTmp));//判定准则
			
			
			StrTmp.Format("%d",TmpVeh.VAdjCode);
			pRst->PutCollect("调整码",_variant_t(StrTmp));//调整准则
			
			
			StrTmp.Format("%d",TmpVeh.VSpeCode);
			pRst->PutCollect("专车码",_variant_t(StrTmp));//专车
			
			StrTmp.Format("%d",TmpVeh.VDpHt);
			pRst->PutCollect("底盘高",_variant_t(StrTmp));//底盘高
			
			StrTmp.Format("%d",TmpVeh.VehHeadHt);
			pRst->PutCollect("车头高",_variant_t(StrTmp));//车头高
			
			StrTmp.Format("%d",TmpVeh.VehHeight);
			pRst->PutCollect("车高",_variant_t(StrTmp));//车高
			
			StrTmp.Format("%d",TmpVeh.Hopper);
			pRst->PutCollect("车斗",_variant_t(StrTmp));//车斗
			
			StrTmp.Format("%d",TmpVeh.HeadJump);
			pRst->PutCollect("头高跳",_variant_t(StrTmp));//头跳高
			
			
			StrTmp.Format("%d",TmpVeh.TailJump);
			pRst->PutCollect("尾高跳",_variant_t(StrTmp));//尾跳高
			
			StrTmp.Format("%d",TmpVeh.Cargo);
			pRst->PutCollect("载货",_variant_t(StrTmp));//载货
			
			StrTmp.Format("%d",TmpVeh.Container);
			pRst->PutCollect("货箱",_variant_t(StrTmp));//货箱
			
			StrTmp.Format("%d",TmpVeh.TruckTail);
			pRst->PutCollect("货车尾",_variant_t(StrTmp));//货车尾
			
			StrTmp.Format("%d",TmpVeh.CastGrd);
			pRst->PutCollect("掷地",_variant_t(StrTmp));//掷地
			
			StrTmp.Format("%d",TmpVeh.FillWin);
			pRst->PutCollect("封闭车窗",_variant_t(StrTmp));//封闭车窗
			
			StrTmp.Format("%d",TmpVeh.GlassWin);
			pRst->PutCollect("玻璃车窗",_variant_t(StrTmp));//玻璃车窗
			
			StrTmp.Format("%d",TmpVeh.SideUiform);
			pRst->PutCollect("侧面一致",_variant_t(StrTmp));//侧面一致
			
			StrTmp.Format("%d",0);
			pRst->PutCollect("低值计数",_variant_t(StrTmp));//低阈值触发计数
			
			StrTmp.Format("%d",0);
			pRst->PutCollect("高值计数",_variant_t(StrTmp));//高阈值触发计数
			
			StrTmp.Format("%d",0);
			pRst->PutCollect("离开计数",_variant_t(StrTmp));//离开触发计数
			
			StrTmp="";
			
			for (i=0;i<24;i++)
			{
				strtmp=TmpVeh.FoldName[i];
				if(strtmp==NULL)
					sTmp=' ';
				StrTmp+=strtmp;
			}
			pRst->PutCollect("备注",_variant_t(StrTmp));//备注
			
			
			pRst->Update();
			pRst->Close();  
			pConn->Close();  
			pRst.Release();  
			pConn.Release(); 
			CoUninitialize();
			
		}
		catch (_com_error& e)
		{
			_bstr_t bstrSource(e.Source()); 
			_bstr_t bs = _bstr_t( " Error: ") + _bstr_t(e.Error()) + _bstr_t( " Msg: ") 
			+ _bstr_t(e.ErrorMessage()) + _bstr_t( " Description: ") 
			+ _bstr_t(e.Description())+bstrSource; 
			WriteLogFile(bs);		
			WriteLogFile("存数据库 失败!");
		}		  	
	}
}




void Add2Txt(VehSet TmpVeh)
{
	int i=0;
	CFileFind OneFile; 
	BOOL beWork=false;
	CString filename="";
	char a_cTxtPath[512]={0};
	BYTE   Cartype;
	CString  StrTmp="";
	CString  strTxt="";
	CString sTmp="";
	CString strTime= CTime::GetCurrentTime().Format("%Y-%m-%d");
	filename=strTime+".txt";
	
	//第一步 获取当前目录 得到根目录
	GetMDBRoot(a_cTxtPath);
	if(a_cTxtPath[strlen(a_cTxtPath)-1]!='\\') 
	{
		a_cTxtPath[strlen(a_cTxtPath)]='\\';
	}
	strcat(a_cTxtPath,filename); 

	beWork= OneFile.FindFile( a_cTxtPath);

	if (!beWork)
	{
		StrTmp.Format("车序号\t车型\t方向\t轴数\t原始轴\t通用轴\t出车类型\t");
		strTxt+=StrTmp;
		StrTmp.Format("车牌\t\t日期\t\t时间\t\t判定码\t调整码\t专车码\t底盘\t车头高\t");
		strTxt+=StrTmp;
		StrTmp.Format("车高\t车斗\t头跳高\t尾跳高\t货箱\t载货\t货车尾\t掷地\t封闭窗\t玻璃窗\t侧面一致\t备注\n");
		strTxt+=StrTmp;
	}
	//写入txt
	StrTmp.Format("%d",TmpVeh.VehId);
	strTxt+=StrTmp+"\t";//车序号


	if(TmpVeh.VPattern>=1 && TmpVeh.VPattern<=4)
	{
		sTmp="客";
		Cartype=TmpVeh.VPattern;
		
	}
	else if(TmpVeh.VPattern>=5 && TmpVeh.VPattern<=9)
	{
		sTmp="货";
		Cartype=TmpVeh.VPattern-4;
		
	}
	else if(TmpVeh.VPattern==10 || TmpVeh.VPattern==11)
	{
		sTmp="集装箱";
		Cartype=TmpVeh.VPattern-9;
	}
	else
		;
	if(TmpVeh.VSpeCode==201)
		StrTmp="行人";
	else
	{
		StrTmp.Format("%d",Cartype);
		StrTmp=sTmp+StrTmp;
	}

	strTxt+=StrTmp+"\t";//车型
	
	
	StrTmp.Format("%d",TmpVeh.VDir);
	strTxt+=StrTmp+"\t";//方向
	
	
	StrTmp.Format("%d",TmpVeh.AxleNum);
	strTxt+=StrTmp+"\t";//轴数
	
	StrTmp="";
	for (i=0;i<TmpVeh.AxleNum;i++)
	{
		sTmp.Format("%d",TmpVeh.AxleType[i]);
		StrTmp+=sTmp;
	}
	strTxt+=StrTmp+"\t";//原始轴型

	StrTmp="";
	StrTmp.Format("%d",TmpVeh.TranAxleType);
	strTxt+=StrTmp+"\t";//通用轴型
	
	if (TmpVeh.VoutFlag==0)
	{
		StrTmp="正常出车";
	}
	else if (TmpVeh.VoutFlag==1)
	{
		StrTmp="强制成功";
	}
	else if (TmpVeh.VoutFlag==2)
	{
		StrTmp="强制失败";
	}
	strTxt+=StrTmp+"\t";//出车类型
	
	
	StrTmp.Format("%s",TmpVeh.Plate);//车牌号码
	strTxt+=StrTmp+"\t";//车牌号码
	if (StrTmp=="无牌车")
	{
		strTxt+="\t";
	}
	
	
	SYSTEMTIME l_stime = {0};
	GetLocalTime(&l_stime);//获得当前本地时间
	
	StrTmp.Format("%4d/%02d/%02d",l_stime.wYear,l_stime.wMonth,l_stime.wDay);
	strTxt+=StrTmp+"\t";//检测日期
	
	StrTmp.Format("%02d:%02d:%02d",l_stime.wHour,l_stime.wMinute,l_stime.wSecond);
	strTxt+=StrTmp+"\t";//显示系统时间
	
	
	
	StrTmp.Format("%d",TmpVeh.VJudCode);
	strTxt+=StrTmp+"\t";//判定准则
	
	
	StrTmp.Format("%d",TmpVeh.VAdjCode);
	strTxt+=StrTmp+"\t";//调整准则
	
	
	StrTmp.Format("%d",TmpVeh.VSpeCode);
	strTxt+=StrTmp+"\t";//专车
	
	StrTmp.Format("%d",TmpVeh.VDpHt);
	strTxt+=StrTmp+"\t";//底盘高
	
	StrTmp.Format("%d",TmpVeh.VehHeadHt);
	strTxt+=StrTmp+"\t";//车头高
	
	StrTmp.Format("%d",TmpVeh.VehHeight);
	strTxt+=StrTmp+"\t";//车高
	
	StrTmp.Format("%d",TmpVeh.Hopper);
	strTxt+=StrTmp+"\t";//车斗
	
	StrTmp.Format("%d",TmpVeh.HeadJump);
	strTxt+=StrTmp+"\t";//头跳高
	
	
	StrTmp.Format("%d",TmpVeh.TailJump);
	strTxt+=StrTmp+"\t";//尾跳高
	
	StrTmp.Format("%d",TmpVeh.Cargo);
	strTxt+=StrTmp+"\t";//载货
	
	StrTmp.Format("%d",TmpVeh.Container);
	strTxt+=StrTmp+"\t";//货箱
	
	StrTmp.Format("%d",TmpVeh.TruckTail);
	strTxt+=StrTmp+"\t";//货车尾
	
	StrTmp.Format("%d",TmpVeh.CastGrd);
	strTxt+=StrTmp+"\t";//掷地
	
	StrTmp.Format("%d",TmpVeh.FillWin);
	strTxt+=StrTmp+"\t";//封闭车窗
	
	StrTmp.Format("%d",TmpVeh.GlassWin);
	strTxt+=StrTmp+"\t";//玻璃车窗
	
	StrTmp.Format("%d",TmpVeh.SideUiform);
	strTxt+=StrTmp+"\t\t";//侧面一致
	
//	StrTmp.Format("%d",0);
//	strTxt+=StrTmp+"\t";//低阈值触发计数
//	
//	StrTmp.Format("%d",0);
//	strTxt+=StrTmp+"\t";//高阈值触发计数
//	
//	StrTmp.Format("%d",0);
//	strTxt+=StrTmp+"\t";//离开触发计数
	
	StrTmp="";
	
//	for (i=0;i<24;i++)
//	{
//		sTmp.Format("%s",TmpVeh.FoldName);
//		strtmp=TmpVeh.FoldName[i];
//		if(strtmp==NULL)
//			sTmp=' ';
//		StrTmp+=strtmp;
//	}
	StrTmp.Format("%s",TmpVeh.FoldName);
	strTxt+=StrTmp+"\n";//备注	

	try
	{
		FILE* fp;
		if ((fp=fopen(a_cTxtPath,"a+"))==NULL)
		{
			return ;
		}
		fwrite(strTxt.GetBuffer(strTxt.GetLength()),sizeof(char),strTxt.GetLength(),fp);
		fclose(fp);
		strTxt.ReleaseBuffer();
	}
	catch (...) 
	{
		WriteLogFile("存出车数据失败");
	}

}