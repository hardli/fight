#ifndef _AVICOM_H_
#define _AVICOM_H_

#include "Config.h"




typedef struct tagVehSet
{
	uint8 VoutFlag;//车辆出车类型 0--正常出车；1--强制出车；2--手动出车
	uint32 VehId;
	uint8 VPattern;
	uint8 VDir;
	uint8 VSpeed;
	uint8 AxleNum;
	uint8 AxleType[6];
	uint8 TyreCnt;
	uint8 VJudCode;
	uint8 VAdjCode;
	uint8 VSpeCode;
	uint32 TranAxleType;//转换后的轴型
	uint8 AxlegroupNum; //轴组数
	uint16 VDpHt;//底盘高
	uint16 VehHeight;//车高
	uint16 VehHeadHt;//车头高
	uint8 SideUiform;//是否具备侧面一致性
	uint8 CastGrd;//是否从中间投掷到地上
	uint8 Hopper;//是否为车斗；1-为原始斗方法；2-纵向聚类斗；4-轮轴定位斗
	uint8 Container;//是否具备货箱 0--不存在货箱；1--侧面平整；2--驾驶室低箱高；
	uint8 HeadJump;//是否存在车头跳变 0--不存在；1--直接找到存在；2--过渡段存在；
	uint8 TailJump;//是否存在尾部跳变 0--不存在；1--存在；
	uint8 Cargo;//是否载货 1--X基线载货；2--外形载货；
	uint8 FillWin;//是否封闭车窗；
	uint8 GlassWin;//是否存在玻璃车窗
	uint8 TruckTail;//是否存在车尾;

	uint8 FoldName[24];
	uint8 Plate[20];//

}VehSet;//车辆信息记录


typedef struct tagModelSet
{
	uint16 ZeroPos;//中心点
	uint16 JGHeight;//激光器高度
	uint16 StartX;//即车道开始的X(搜寻轮轴的起始X)
	uint16 LaneWidth;//道路宽度
	uint16 MaxHt;//最大净空高度
	uint16 IsLanHt;//安全岛高度
	uint16 IsLanWd;//安全岛宽度
	uint16 u16CRC;//CRC校验	
}ModelSet;


typedef struct tagLaserSet
{
	char IP0[15];
	uint16 Port0;
	char IP1[15];
	uint16 Port1;
	uint8 AqcMode;
}LaserSet;

#define FILESIN 2
#define SOFTSIM 1
#define LASERIN 0

typedef struct tagSysTem
{
	uint8 RunMode;//运行模式
	uint8 ProjectId;//方案编号
	uint8 DirEnable;//方向设备使能
	uint8 DirType;//方向设备类型
	uint8 SvWv;//存波形标志位
	uint8 PlotImg;//绘图标志位
	uint8 SvDB;//存数据库标志位
	uint8 WvCheck;//波形丢帧检测标志位
}SystemSet;

typedef struct tagVOutSet
{
	uint8 A3ForOut;
	uint8 AxReport;
	uint8 EmFramCnt;
	uint8 HiFramCnt;
	uint8 LwFramCnt;
	uint8 LvFramCnt;
	uint8 GratPos;
	uint16 GratHt;
}VOutSet;

typedef struct tagWvFiles
{
	uint8 AddEmFram;
	uint8 ForceOut;
	uint8 PathExist;
	uint8 bSearChed;
	uint8 WvPath[MAX_PATH];
}WvFiles;


#define Max_CACHE 10000
#define	Max_Size  831
typedef struct tagSvWave
{
	uint8  bSvFlag;
	uint16 SvIndx;
	HANDLE SvThdHd;
	uint16 RecIndx;
	uint16 SvCnt;
	uint8 WvCache[Max_CACHE][Max_Size+5];//结构组成 831波形+新车标志1位
}SvWave;

typedef struct tagAqcState
{
	uint32 RecTotal;
	uint32 ProWvCnt;
	uint32 RecWvErr;
	uint32 RecWvCnt;
	uint32 RecErrCnt;
	uint32 RecCmdCnt;
}AqcState;

typedef struct tagCameSet
{
	uint8 CamEnable;
	char CamIp[20];
	char CamPwd[30];
	uint16 CamPort;
	uint8 CamPicCar;
	uint8 CamPicPlate;

	uint8 CamEnable1;
	char CamIp1[20];
	char CamPwd1[30];
	uint16 CamPort1;
	uint8 CamPicCar1;
	uint8 CamPicPlate1;
}CamSet;

typedef struct tagQueSet
{
	 int DelVehTime ;		//删除单车时间，默认6秒
	 int EndVehWaitTime;	//强制收尾时等待出车时间
	 int ClearQueTime;		//出车后间隔一段时间清除队列
	 int MaxVehCnt;			//队列允许的最大车辆数
	 int Quecheck;			//队列维护使能
	 int TypecheckID;		//各省份车型轴型转换代码
	 int IsETC;				//是否是ETC车道
}QueSet;

/************************************************************************/
/* 宏定义错误码                                                         */
/************************************************************************/
#define ERR_FAIL 0  //失败或不满足
#define ERR_SUCC 1  //成功或满足



//2-200 为主控DLL预留


//200-300 为算法DLL预留
#define ERR_MdSET 201
#define ERR_LsSet 202
#define ERR_SySet 203
#define ERR_VOSet 204
#define ERR_PTNUL 205
#define ERR_THNUL 206
#define ERR_PFVEH 207
#define ERR_PFAXL 208
#define ERR_PFPLT 209

//301-400 为激光器连接用
#define ERR_SOCKVERERR 301
#define ERR_CREATESOCK 302
#define ERR_CONNFAILED 303
#define ERR_RECVTHREAD 304
#define ERR_RECFUNNULL 305
#define ERR_SVRCLOSED  306
#define ERR_RECCONNERR 307
#define ERR_SENDBUFERR 308
#define ERR_DISCONNERR 309
#define ERR_AQCSINGLE 310 
#define ERR_AQCCONTIN 311
#define ERR_STOPCONTI 312

#endif