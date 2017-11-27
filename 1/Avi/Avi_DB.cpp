#include "stdafx.h"
#include "Avi_DB.h"
#include "IniProc.h"
#include "Avi_Main.h"
#include <afxdisp.h>


BOOL OpenMdb(void)
{
    char a_cMdbPath[256]; //mdb�ļ�·��
    char a_cConnstr[512]; //�����ַ���
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
	//      WriteLogFile(_T("��ȡ���ݿ�·��ʧ��!"));
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
        WriteLogFile("û���ҵ����ݿ⣬��������Avi.mdb");
        BOOL result=CreateDB(a_cMdbPath);
		return result;
    }
    else
    {
		try
		{
			strcat(a_cConnstr,a_cMdbPath);
			CoInitialize(NULL);
			ADODB::_ConnectionPtr pConn(__uuidof(ADODB::Connection));//����connection����ָ��
			ADODB::_RecordsetPtr pRst(__uuidof(ADODB::Recordset)); //����recordset����ָ��
			pConn->Open(a_cConnstr, "","",ADODB::adModeUnknown); //�����ݿ�
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
        pCatalog->Create((LPCTSTR)strConnect);//�������ݿ�
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
        pCols->Append(_T("���")  ,ADOX::adInteger,0);//�Զ�����ֶ�
        pCols->Append(_T("�����")  ,ADOX::adInteger,0);//�����
        pCols->Append(_T("����")   ,ADOX::adWChar,0);//�ı��ֶ�
		pCols->Append(_T("����")   ,ADOX::adInteger,0);
        pCols->Append(_T("����")  ,ADOX::adInteger,0);
        pCols->Append(_T("ԭʼ����")  ,ADOX::adWChar,0);
		pCols->Append(_T("ͨ������")  ,ADOX::adInteger,0);
		pCols->Append(_T("���ƺ���")  ,ADOX::adWChar,0);
		pCols->Append(_T("��������")  ,ADOX::adWChar,0);
        pCols->Append(_T("�������")  ,ADOX::adDate,0);//�����ֶ�
        pCols->Append(_T("���ʱ��")  ,ADOX::adDate,0);//�����ֶ�
		pCols->Append(_T("�ж���")  ,ADOX::adInteger,0);
		pCols->Append(_T("������")  ,ADOX::adInteger,0);
		pCols->Append(_T("ר����")  ,ADOX::adInteger,0);
		pCols->Append(_T("���̸�")  ,ADOX::adInteger,0);
		pCols->Append(_T("��ͷ��")  ,ADOX::adInteger,0);
		pCols->Append(_T("����")  ,ADOX::adInteger,0);
        pCols->Append(_T("����")  ,ADOX::adInteger,0);
        pCols->Append(_T("ͷ����"), ADOX::adInteger,0);
        pCols->Append(_T("β����")  ,ADOX::adInteger,0);
        pCols->Append(_T("�ػ�")  ,ADOX::adInteger,0);
        pCols->Append(_T("����")  ,ADOX::adInteger,0);
		pCols->Append(_T("����β")  ,ADOX::adInteger,0);
        pCols->Append(_T("����")  ,ADOX::adInteger,0);
        pCols->Append(_T("��ճ���")  ,ADOX::adInteger,0);
        pCols->Append(_T("��������")  ,ADOX::adInteger,0);
		pCols->Append(_T("����һ��")  ,ADOX::adInteger,0);
		pCols->Append(_T("��ֵ����")  ,ADOX::adInteger,0);
		pCols->Append(_T("��ֵ����")  ,ADOX::adInteger,0);
		pCols->Append(_T("�뿪����")  ,ADOX::adInteger,0);
		pCols->Append(_T("��ע")  ,ADOX::adWChar,0);
        pCatalog->Tables->Refresh();
        long lCount=pCols->Count;
        for(long i=0; i<lCount; i++)
        {
            pCols->GetItem(i)->ParentCatalog =pCatalog;//��Ҫ������Catalog,�μ�Q201826 PRB: Error 3265 When You Access Properties Collection
            ADOX::PropertiesPtr pProperties=pCols->GetItem(i)->Properties;
			
        }
        pCols->GetItem(_T("���"))->Properties->GetItem(_T("Description"))->Value=_T("�������");//ע��
        pCols->GetItem(_T("���"))->Properties->GetItem(_T("AutoIncrement"))->Value=true;//�Զ����
		pCols->GetItem(_T("����"))->Properties->GetItem(_T("Jet OLEDB:Compressed UniCode Strings"))->Value=true;
        pCols->GetItem(_T("ԭʼ����"))->Properties->GetItem(_T("Jet OLEDB:Compressed UniCode Strings"))->Value=true;
		pCols->GetItem(_T("��������"))->Properties->GetItem(_T("Jet OLEDB:Compressed UniCode Strings"))->Value=true;
		pCols->GetItem(_T("���ƺ���"))->Properties->GetItem(_T("Jet OLEDB:Compressed UniCode Strings"))->Value=true;
		pCols->GetItem(_T("��ע"))->Properties->GetItem(_T("Jet OLEDB:Compressed UniCode Strings"))->Value=true;
        pCatalog->Tables->Append(_variant_t((IDispatch*)pTable));//��ӱ�
        //MessageBox("zheli");
        pCatalog->Tables->Refresh();//ˢ��
        pIndexNew.CreateInstance(_T("ADOX.Index"));
        pIndexNew->Name = _T("���");//��������
        pIndexNew->Columns->Append(_T("���"),ADOX::adInteger,0);//�����ֶ�
        pIndexNew->PutPrimaryKey(-1);//������
        pIndexNew->PutUnique(-1);//Ψһ����
        pTable->Indexes->Append(_variant_t ((IDispatch*)pIndexNew));//��������
        pIndexNew=NULL;
        pCatalog->Tables->Refresh();//ˢ��
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
		WriteLogFile("�������ݿ����");
        return FALSE;
    }
    catch(...)
    {
		WriteLogFile("�������ݿ��쳣");
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
	
	char a_cMdbPath[256]={0}; //mdb�ļ�·�� 
	char a_cConnstr[512]={0}; //�����ַ���
	CFileFind OneFile; 
	BOOL beWork;
	
	strcpy(a_cConnstr,"driver={Microsoft Access Driver (*.mdb)};dbq=");  
	//	if(!::GetCurrentDirectory(256,a_cMdbPath))  
	//	{  
	//		WriteLogFile("��ȡ���ݿ�·��ʧ��!");			
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
			strcat(a_cConnstr,a_cMdbPath);//�Ѻ�����ָ�ַ�����ӵ�ǰ�߽�β��(����ǰ�߽�β����'\0')�����'\0'  
			
			CoInitialize(NULL);  
			ADODB::_ConnectionPtr pConn(__uuidof(ADODB::Connection));//����connection����ָ��
			ADODB::_RecordsetPtr pRst(__uuidof(ADODB::Recordset)); //����recordset����ָ��  
			pConn->Open(a_cConnstr, "","",ADODB::adModeUnknown); //�����ݿ�  
			pRst->Open("select * from VelInfor",_variant_t((IDispatch*)pConn),ADODB::adOpenStatic,ADODB::adLockOptimistic,ADODB::adCmdText);  
			pRst->AddNew(); 
			
			
			StrTmp.Format("%d",TmpVeh.VehId);
			pRst->PutCollect("�����",_variant_t(StrTmp)); ;//�����
			
			//д�����ݿ�
			
			if(TmpVeh.VPattern>=1 && TmpVeh.VPattern<=4)
			{
				str="��";
				Cartype=TmpVeh.VPattern;
				
			}
			else if(TmpVeh.VPattern>=5 && TmpVeh.VPattern<=9)
			{
				str="��";
				Cartype=TmpVeh.VPattern-4;
				
			}
			else if(TmpVeh.VPattern==10 || TmpVeh.VPattern==11)
			{
				str="��װ��";
				Cartype=TmpVeh.VPattern-9;
			}
			else
				;
			if(TmpVeh.VSpeCode==201)
				StrTmp="����";
			else
			{
				StrTmp.Format("%d",Cartype);
				StrTmp=str+StrTmp;
			}
			pRst->PutCollect("����",_variant_t(StrTmp));//����
			
			
			StrTmp.Format("%d",TmpVeh.VDir);
			pRst->PutCollect("����",_variant_t(StrTmp));
			
			
			
			StrTmp.Format("%d",TmpVeh.AxleNum);
			pRst->PutCollect("����",_variant_t(StrTmp));//����
			
			StrTmp="";
			for (i=0;i<TmpVeh.AxleNum;i++)
			{
				sTmp.Format("%d",TmpVeh.AxleType[i]);
				StrTmp+=sTmp;
			}
			pRst->PutCollect("ԭʼ����",_variant_t(StrTmp));//����
			StrTmp="";
			
			StrTmp.Format("%d",TmpVeh.TranAxleType);
			pRst->PutCollect("ͨ������",_variant_t(StrTmp));//����
			
			if (TmpVeh.VoutFlag==0)
			{
				StrTmp="��������";
			}
			else if (TmpVeh.VoutFlag==1)
			{
				StrTmp="ǿ�Ƴɹ�";
			}
			else if (TmpVeh.VoutFlag==2)
			{
				StrTmp="ǿ��ʧ��";
			}
			pRst->PutCollect("��������",_variant_t(StrTmp));//��������
			
			
			StrTmp.Format("%s",TmpVeh.Plate);//���ƺ���
			pRst->PutCollect("���ƺ���",_variant_t(StrTmp));//��������
			
			
			SYSTEMTIME l_stime = {0};
			GetLocalTime(&l_stime);//��õ�ǰ����ʱ��
			
			StrTmp.Format("%4d/%02d/%02d",l_stime.wYear,l_stime.wMonth,l_stime.wDay);
			pRst->PutCollect("�������",_variant_t(StrTmp));//�������
			
			StrTmp.Format("%02d:%02d:%02d",l_stime.wHour,l_stime.wMinute,l_stime.wSecond);
			pRst->PutCollect("���ʱ��",_variant_t(StrTmp));//��ʾϵͳʱ��
			
			
			
			StrTmp.Format("%d",TmpVeh.VJudCode);
			pRst->PutCollect("�ж���",_variant_t(StrTmp));//�ж�׼��
			
			
			StrTmp.Format("%d",TmpVeh.VAdjCode);
			pRst->PutCollect("������",_variant_t(StrTmp));//����׼��
			
			
			StrTmp.Format("%d",TmpVeh.VSpeCode);
			pRst->PutCollect("ר����",_variant_t(StrTmp));//ר��
			
			StrTmp.Format("%d",TmpVeh.VDpHt);
			pRst->PutCollect("���̸�",_variant_t(StrTmp));//���̸�
			
			StrTmp.Format("%d",TmpVeh.VehHeadHt);
			pRst->PutCollect("��ͷ��",_variant_t(StrTmp));//��ͷ��
			
			StrTmp.Format("%d",TmpVeh.VehHeight);
			pRst->PutCollect("����",_variant_t(StrTmp));//����
			
			StrTmp.Format("%d",TmpVeh.Hopper);
			pRst->PutCollect("����",_variant_t(StrTmp));//����
			
			StrTmp.Format("%d",TmpVeh.HeadJump);
			pRst->PutCollect("ͷ����",_variant_t(StrTmp));//ͷ����
			
			
			StrTmp.Format("%d",TmpVeh.TailJump);
			pRst->PutCollect("β����",_variant_t(StrTmp));//β����
			
			StrTmp.Format("%d",TmpVeh.Cargo);
			pRst->PutCollect("�ػ�",_variant_t(StrTmp));//�ػ�
			
			StrTmp.Format("%d",TmpVeh.Container);
			pRst->PutCollect("����",_variant_t(StrTmp));//����
			
			StrTmp.Format("%d",TmpVeh.TruckTail);
			pRst->PutCollect("����β",_variant_t(StrTmp));//����β
			
			StrTmp.Format("%d",TmpVeh.CastGrd);
			pRst->PutCollect("����",_variant_t(StrTmp));//����
			
			StrTmp.Format("%d",TmpVeh.FillWin);
			pRst->PutCollect("��ճ���",_variant_t(StrTmp));//��ճ���
			
			StrTmp.Format("%d",TmpVeh.GlassWin);
			pRst->PutCollect("��������",_variant_t(StrTmp));//��������
			
			StrTmp.Format("%d",TmpVeh.SideUiform);
			pRst->PutCollect("����һ��",_variant_t(StrTmp));//����һ��
			
			StrTmp.Format("%d",0);
			pRst->PutCollect("��ֵ����",_variant_t(StrTmp));//����ֵ��������
			
			StrTmp.Format("%d",0);
			pRst->PutCollect("��ֵ����",_variant_t(StrTmp));//����ֵ��������
			
			StrTmp.Format("%d",0);
			pRst->PutCollect("�뿪����",_variant_t(StrTmp));//�뿪��������
			
			StrTmp="";
			
			for (i=0;i<24;i++)
			{
				strtmp=TmpVeh.FoldName[i];
				if(strtmp==NULL)
					sTmp=' ';
				StrTmp+=strtmp;
			}
			pRst->PutCollect("��ע",_variant_t(StrTmp));//��ע
			
			
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
			WriteLogFile("�����ݿ� ʧ��!");
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
	
	//��һ�� ��ȡ��ǰĿ¼ �õ���Ŀ¼
	GetMDBRoot(a_cTxtPath);
	if(a_cTxtPath[strlen(a_cTxtPath)-1]!='\\') 
	{
		a_cTxtPath[strlen(a_cTxtPath)]='\\';
	}
	strcat(a_cTxtPath,filename); 

	beWork= OneFile.FindFile( a_cTxtPath);

	if (!beWork)
	{
		StrTmp.Format("�����\t����\t����\t����\tԭʼ��\tͨ����\t��������\t");
		strTxt+=StrTmp;
		StrTmp.Format("����\t\t����\t\tʱ��\t\t�ж���\t������\tר����\t����\t��ͷ��\t");
		strTxt+=StrTmp;
		StrTmp.Format("����\t����\tͷ����\tβ����\t����\t�ػ�\t����β\t����\t��մ�\t������\t����һ��\t��ע\n");
		strTxt+=StrTmp;
	}
	//д��txt
	StrTmp.Format("%d",TmpVeh.VehId);
	strTxt+=StrTmp+"\t";//�����


	if(TmpVeh.VPattern>=1 && TmpVeh.VPattern<=4)
	{
		sTmp="��";
		Cartype=TmpVeh.VPattern;
		
	}
	else if(TmpVeh.VPattern>=5 && TmpVeh.VPattern<=9)
	{
		sTmp="��";
		Cartype=TmpVeh.VPattern-4;
		
	}
	else if(TmpVeh.VPattern==10 || TmpVeh.VPattern==11)
	{
		sTmp="��װ��";
		Cartype=TmpVeh.VPattern-9;
	}
	else
		;
	if(TmpVeh.VSpeCode==201)
		StrTmp="����";
	else
	{
		StrTmp.Format("%d",Cartype);
		StrTmp=sTmp+StrTmp;
	}

	strTxt+=StrTmp+"\t";//����
	
	
	StrTmp.Format("%d",TmpVeh.VDir);
	strTxt+=StrTmp+"\t";//����
	
	
	StrTmp.Format("%d",TmpVeh.AxleNum);
	strTxt+=StrTmp+"\t";//����
	
	StrTmp="";
	for (i=0;i<TmpVeh.AxleNum;i++)
	{
		sTmp.Format("%d",TmpVeh.AxleType[i]);
		StrTmp+=sTmp;
	}
	strTxt+=StrTmp+"\t";//ԭʼ����

	StrTmp="";
	StrTmp.Format("%d",TmpVeh.TranAxleType);
	strTxt+=StrTmp+"\t";//ͨ������
	
	if (TmpVeh.VoutFlag==0)
	{
		StrTmp="��������";
	}
	else if (TmpVeh.VoutFlag==1)
	{
		StrTmp="ǿ�Ƴɹ�";
	}
	else if (TmpVeh.VoutFlag==2)
	{
		StrTmp="ǿ��ʧ��";
	}
	strTxt+=StrTmp+"\t";//��������
	
	
	StrTmp.Format("%s",TmpVeh.Plate);//���ƺ���
	strTxt+=StrTmp+"\t";//���ƺ���
	if (StrTmp=="���Ƴ�")
	{
		strTxt+="\t";
	}
	
	
	SYSTEMTIME l_stime = {0};
	GetLocalTime(&l_stime);//��õ�ǰ����ʱ��
	
	StrTmp.Format("%4d/%02d/%02d",l_stime.wYear,l_stime.wMonth,l_stime.wDay);
	strTxt+=StrTmp+"\t";//�������
	
	StrTmp.Format("%02d:%02d:%02d",l_stime.wHour,l_stime.wMinute,l_stime.wSecond);
	strTxt+=StrTmp+"\t";//��ʾϵͳʱ��
	
	
	
	StrTmp.Format("%d",TmpVeh.VJudCode);
	strTxt+=StrTmp+"\t";//�ж�׼��
	
	
	StrTmp.Format("%d",TmpVeh.VAdjCode);
	strTxt+=StrTmp+"\t";//����׼��
	
	
	StrTmp.Format("%d",TmpVeh.VSpeCode);
	strTxt+=StrTmp+"\t";//ר��
	
	StrTmp.Format("%d",TmpVeh.VDpHt);
	strTxt+=StrTmp+"\t";//���̸�
	
	StrTmp.Format("%d",TmpVeh.VehHeadHt);
	strTxt+=StrTmp+"\t";//��ͷ��
	
	StrTmp.Format("%d",TmpVeh.VehHeight);
	strTxt+=StrTmp+"\t";//����
	
	StrTmp.Format("%d",TmpVeh.Hopper);
	strTxt+=StrTmp+"\t";//����
	
	StrTmp.Format("%d",TmpVeh.HeadJump);
	strTxt+=StrTmp+"\t";//ͷ����
	
	
	StrTmp.Format("%d",TmpVeh.TailJump);
	strTxt+=StrTmp+"\t";//β����
	
	StrTmp.Format("%d",TmpVeh.Cargo);
	strTxt+=StrTmp+"\t";//�ػ�
	
	StrTmp.Format("%d",TmpVeh.Container);
	strTxt+=StrTmp+"\t";//����
	
	StrTmp.Format("%d",TmpVeh.TruckTail);
	strTxt+=StrTmp+"\t";//����β
	
	StrTmp.Format("%d",TmpVeh.CastGrd);
	strTxt+=StrTmp+"\t";//����
	
	StrTmp.Format("%d",TmpVeh.FillWin);
	strTxt+=StrTmp+"\t";//��ճ���
	
	StrTmp.Format("%d",TmpVeh.GlassWin);
	strTxt+=StrTmp+"\t";//��������
	
	StrTmp.Format("%d",TmpVeh.SideUiform);
	strTxt+=StrTmp+"\t\t";//����һ��
	
//	StrTmp.Format("%d",0);
//	strTxt+=StrTmp+"\t";//����ֵ��������
//	
//	StrTmp.Format("%d",0);
//	strTxt+=StrTmp+"\t";//����ֵ��������
//	
//	StrTmp.Format("%d",0);
//	strTxt+=StrTmp+"\t";//�뿪��������
	
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
	strTxt+=StrTmp+"\n";//��ע	

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
		WriteLogFile("���������ʧ��");
	}

}