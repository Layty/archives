/******************************************************************************
Filename:       MED_matchup.c
Revised:        $Date: 2015-5-22 12:53:18 -0800 (Tue, 22 Dec 2009) $
Revision:       $Revision: 21388 $
Description:    This module implements the MED functionality and contains the
init and event loop functions

*******************************************************************************/

/*******************************************************************************
* INCLUDES
*/
#include "MED_NV.h"
#include "MED_IRSEND.h"
#include "MED_matchup.h"
/*******************************************************************************
* TYPEDEFS
*/

/*******************************************************************************
* GLOBAL VARIABLES
*/
volatile  irda_matchup_t   ir_matchup_result;       
typedef  struct
{
  union
  {
    uint8    capture_char[2];
    uint16   capture_int;
  };
  uint16  count;
  uint16  count_last;
  uint8   time_count;
  uint16  iLogic1HTime;       //数据逻辑1高时间
  uint16  iLogic1LTime;       //数据逻辑1低时间
  uint16  iLogic0HTime;       //数据逻辑0高时间
  uint16  iLogic0LTime;       //数据逻辑0低时间
#ifdef   MATCH_BRAND
  uint8   IRCodeRule;
  uint8   cIRDevType;         //红外设备类型
  uint8   ovfif_times;        //型号识别任务超时退出
  bool    matchup_source;
#endif
  bool    matchup_state;
  bool    time_flag;
}capture_data;

/*******************************************************************************
* LOCAL VARIABLES
*/
bool    g_bCapture1st;
uint8   g_cTimer1_Mode = 0;
static  capture_data  capture_temp;
uint16  dev_mark = 0;
static  const uint8  CODE ir_para[][8]=
{//温度风力等参数在数组里面的位置（从1开始）（4个），以及位宽（4个）。每一排代表一个品牌遥控器。
  4, 1,  5,  9, 1, 3, 2, 4, //格力
  50,70, 54, 65, 1, 3, 2, 5, //志高
  30,37, 17, 33, 1, 2, 3, 4, //美的  正常
  78,85, 65, 81, 1, 2, 3, 4, //美的  扩展（定时,省电）
  78,53, 37, 12, 1, 4, 4, 5, //奥克斯
  70,47, 43, 50, 1, 2, 2, 5, //海信
  34,57, 41,  9, 1, 3, 3, 4,  //海尔      第33位是辅热开关     
  48,37, 33,  9, 1, 3, 2, 4,  //澳柯玛
  38,38, 34, 29, 1, 3, 3, 4,  //新科
  28,25, 14, 29, 1, 3, 3, 4,  //三菱重工  
  43,49, 65, 57, 1, 2, 3, 4,  //YAIR   //扬子空调
  43,49, 65, 57, 1, 4, 3, 4,   //HUALING 华凌空调   
  19,25, 17, 29, 1, 4, 2, 4,   //科龙空调        //注意：开关机不准确，是取反的！。。。。。。。。
  41,45, 69, 50, 1, 4, 4, 4,   //大金空调   模式，BUG，已經修正。
  4,  1, 17,  9, 1, 2, 2, 5,   //春兰空调
  19,25, 17, 29, 1, 3, 2, 4,    //海信（新款DG11J1-03）第39位是辅热开关，自动通风除湿三种模式都没有温度 //注意：开关机不准确，是取反的！。。。。。。。。
  9, 50, 55, 13, 1, 4, 2, 4,   //新科  柳月款
  43, 49, 65, 57, 1, 3, 3,4,   //TCL空调  
  48, 37, 33, 9, 1, 3, 2, 5,   //CHANGHONG 长虹
  34, 17, 27, 41,1, 4, 2, 4,   //留给海尔（YR-C03）
  43, 49, 65, 57, 1, 4, 3, 4,  //格兰仕
  221, 201, 205, 107,1,3,3,4,     //日立空调 或者温度位置+8；开关机没有看出来
  105,109,134,114, 1, 3, 3, 4,     //松下空調 
  9, 14, 22, 17, 1, 3, 3, 4,     //LG空調 
  14, 101, 98, 93, 1, 3, 3, 4,    //三星空調 
  14, 157, 154, 149, 1, 3, 3,4,   //留给三星空調  
  14, 73,  81, 69, 1, 3, 3, 4,    //富士通
  16,  9,  13, 17, 1, 3, 2, 4,    //留给奥克斯
  21, 22, 81, 17, 1, 3, 4, 4,     //留给新科     
  76, 73, 70, 77,  1, 3, 3, 4,    //留给三菱 (泰国)
  16, 49, 41,  9,  1, 3, 2, 4     //留给海尔（YR-D03）
};

static const uint16  CODE ir_lead[][12]=
{                     //前导码值，每一排代表一个品牌遥控器。第二竖排是引导码判断使用的偏差值
  9230,4430,700,923, 9230,4430,700,923,0,0,0,0,   //格力
  6220,7320,600,622, 6220,7320,600,622,0,0,0,0,   //志高
  4520,4380,580,452, 4520,4380,580,452,0,0,0,0,   //美的  正常
  4520,4380,580,452, 4520,4380,580,452,0,0,0,0,   //美的  扩展（定时,省电）
  9120,4440,700,912, 8700,4300,580,400,0,0,0,0,  //奥克斯     
  3500,1620,450,400, 8970,4470,580,400,0,0,0,0,  //海信（后一种是DG11J1-03）
  3120,3010,3010,400,3120,3010,3010,400,0,0,0,0,  //海尔
  3130,4410,604, 400,3130,4410,604, 400,0,0,0,0,  //澳柯玛
  1100,10890,1100,300,10140,5050,600,300,3150,3250,350,300,//新科 后一款是柳月款遥控器
  6127,7450,600,400,  3150,1580,400,400,0,0,0,0,  //三菱重工
  3340,1670,450,400,  3340,1670,450,400,0,0,0,0,  //扬子空调  
  3350,1700,380,400,  4520,4380,580,452,0,0,0,0,  //华凌空调
  9090,4580,589,400,  9090,4580,589,400,0,0,0,0,  //科龙空调 
  3510,1650,454,400,  3460,1710,454,400,0,0,0,0,  //大金空调 
  8930,4500,600,400,  8930,4500,600,400,0,0,0,0,  //春兰空调
   0,      0,  0,  0,     0,   0,  0, 0,0,0,0,0,  //留给海信新款
   0,      0,  0,  0,     0,   0,  0, 0,0,0,0,0,  //新科  柳月款
  3050,1700,525,400,   3050,1700,525,400,0,0,0,0, //TCL空调  
  8460,4200,530,400,   8460,4200,530,400,0,0,0,0, //CHANGHONG 长虹
   0,      0,  0,  0,     0,   0,  0, 0,0,0,0,0,  //海尔   
  3780,1420,545,300,  3780,1420,545,300,0,0,0,0,  //格兰仕
  3350,1670,440,300,  3350,1670,440,300,0,0,0,0,  //日立空调 
  3510,1720,450,300,  3550,3460,900,300,0,0,0,0,  //松下空調 
  8500,4300,550,300,  8500,4300,550,300,0,0,0,0,  //LG空調   
  3000,9000,500,300,  3000,9000,500,300,0,0,0,0,   //三星空調  
  3000,9000,500,300,  3000,9000,500,300,0,0,0,0,   //三星空調   
  3300,1600,420,300,  3300,1600,420,300,0,0,0,0    //富士通
};

static  const uint8 CODE  MIDEAtemperature[15]   = {17,28,24,25,20,29,21,0,18,27,23,26,19,30,22};
static  const uint8 CODE  MIDEAsysmode[8]        = {SYS_COOL,SYS_AUTO,SYS_DIVIDE_WET,SYS_HEAT,SYS_HEAT,SYS_DIVIDE_WET,SYS_AUTO,SYS_COOL};
static  const uint8 CODE  MIDEAwind[8]           = {0,1,2,0,3,0,1,0};
static  const uint8 CODE  GREEsysmode[8]         = {SYS_AUTO,SYS_COOL,SYS_DIVIDE_WET,SYS_SEND_WIND,SYS_HEAT,SYS_AUTO};
static  const uint8 CODE  CHIGOsysmode[8]        = {SYS_AUTO,SYS_AUTO,SYS_AUTO,SYS_HEAT,SYS_SEND_WIND,SYS_DIVIDE_WET,SYS_COOL,SYS_AUTO};
static  const uint8 CODE  CHIGOwind[4]           = {1,2,3,0};
static  const uint8 CODE  AUXsysmode[14]         = {SYS_AUTO,SYS_COMFORT,SYS_COOL,SYS_COMFORT,SYS_DIVIDE_WET,SYS_COMFORT,0,0,SYS_HEAT,SYS_COMFORT,                                                      0,0,SYS_SEND_WIND,SYS_COMFORT};
static  const uint8 CODE  AUXwind[11]            = {0,0,3,0,2,0,1,0,0,0,0};
static  const uint8 CODE  AUXwind_011[11]        = {0,0,2,0,2,0,1,0,0,0,3};
static  const uint8 CODE  HISENSEsysmode[8]      = {SYS_AUTO,SYS_HEAT,SYS_COOL,SYS_DIVIDE_WET,0,0,0,0};
static  const uint8 CODE  HISENSE_NEWsysmode[5]  = {SYS_HEAT,SYS_AUTO,SYS_COOL,SYS_DIVIDE_WET,SYS_SEND_WIND};
static  const uint8 CODE  HISENSEwind[4]         = {0,3,1,2};
static  const uint8 CODE  HISENSE_NEWwind[4]     = {0,3,2,1};
static  const uint8 CODE  HAIERsysmode[7]        = {SYS_AUTO,SYS_COOL,SYS_DIVIDE_WET,0,SYS_HEAT,0,SYS_SEND_WIND};
static  const uint8 CODE  HAIERwind[7]           = {0,0,2,0,3,0,1};
static  const uint8 CODE  AUCMAsysmode[6]        = {0,SYS_HEAT,SYS_COOL,SYS_DIVIDE_WET,SYS_SEND_WIND,SYS_AUTO};
static  const uint8 CODE  SHINCOsysmode[7]       = {0,0,SYS_COOL,SYS_HEAT,0,0,SYS_DIVIDE_WET};
static  const uint8 CODE  SHINCOwind[8]          = {0,0,0,2,0,3,0,1};
static  const uint8 CODE  SHINCO_NEWsysmode[7]   = {SYS_AUTO,SYS_HEAT,SYS_DIVIDE_WET,SYS_HEAT,SYS_COOL,0,SYS_SEND_WIND};
static  const uint8 CODE  SHINCO_NEWwind[4]      = {0,2,1,3};
static  const uint8 CODE  SHINCO_NEW2sysmode[7]  = {SYS_AUTO,SYS_HEAT,SYS_COOL,SYS_HEAT,0,SYS_SEND_WIND,SYS_DIVIDE_WET};
static  const uint8 CODE  SHINCO_NEW2wind[9]     = {0,0,3,0,2,0,0,0,1};
static  const uint8 CODE  MITSUBISHI_WIND[6]     =  {0,3,2,1,3,3}; 
static  const uint8 CODE  MITSUBISHI_SYSMODE[5]  =  {SYS_AUTO,SYS_COOL,SYS_DIVIDE_WET,0,SYS_HEAT}; 
static  const uint8 CODE  MITSUBISHI_WIND1[6]    =  {0,0,1,2,3,0}; 
static  const uint8 CODE  MITSUBISHI_SYSMODE1[8] =  {0,0,0,SYS_HEAT,0,SYS_DIVIDE_WET,SYS_COOL,SYS_SEND_WIND}; 
static  const uint8 CODE  YAIR_WIND[]            =  {0,0,1,2,1,3,2}; 
static  const uint8 CODE  YAIR_SYSMODE[]         =  {0,SYS_HEAT,SYS_DIVIDE_WET,SYS_COOL,0};
static  const uint8 CODE  HUALING_WIND[]         =  {0,0,1,2,1,3,2}; 
static  const uint8 CODE  HUALING_SYSMODE[]      =  {0,SYS_HEAT,SYS_DIVIDE_WET,SYS_COOL,0,0,0,SYS_SEND_WIND,SYS_AUTO};
static  const uint8 CODE  KELON_WIND[]           =  {0,3,2,1}; 
static  const uint8 CODE  KELON_SYSMODE[]        =  {SYS_HEAT,SYS_AUTO,SYS_COOL,SYS_DIVIDE_WET,SYS_SEND_WIND};
static  const uint8 CODE  CHUNLAN_WIND[]         =  {0,3,2,1}; 
static  const uint8 CODE  CHUNLAN_SYSMODE[]      =  {SYS_COOL,SYS_DIVIDE_WET,SYS_SEND_WIND,SYS_HEAT,SYS_AUTO};
static  const uint8 CODE  CHUNLAN_temperature[]  =  {23,24,25,26,27,28,29,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,16,17,18,19,20,21,22};
static  const uint8 CODE  TCL_SYSMODE[]          =  {SYS_AUTO,SYS_HEAT,SYS_DIVIDE_WET,SYS_COOL,0,0,0,SYS_SEND_WIND};
static  const uint8 CODE  TCL_WIND[]             =  {0,0,1,2,0,3};
static  const uint8 CODE  DAIKIN_SYSMODE[14]     =  {SYS_AUTO,0,SYS_DIVIDE_WET,SYS_COOL,SYS_HEAT,0,SYS_SEND_WIND};
static  const uint8 CODE  DAIKIN_WIND[12]        =  {0,0,0,1,1,2,2,3,0,0,0,0};
static  const uint8 CODE  CHANGHONG_SYSMODE[7]   =  {0,SYS_HEAT,SYS_COOL,SYS_DIVIDE_WET,SYS_AUTO,SYS_SEND_WIND};
static  const uint8 CODE  CHANGHONG_WIND[4]      =  {0,3,1,2};
static  const uint8 CODE  GALANZ_SYSMODE[]       =  {0,SYS_HEAT,SYS_DIVIDE_WET,SYS_COOL,0,0,0,SYS_SEND_WIND,SYS_AUTO};
static  const uint8 CODE  GALANZ_WIND[]          =  {0,0,1,2,0,3};
static  const uint8 CODE  HITACHI_SYSMODE[]      =  {0,SYS_HEAT,SYS_DIVIDE_WET,0,SYS_COOL,0,0,SYS_SEND_WIND,SYS_AUTO};
static  const uint8 CODE  HITACHI_WIND[]         =  {0,0,0,3,2,1};
static  const uint8 CODE  PANASONIC_SYSMODE[]    =  {SYS_AUTO,0,SYS_DIVIDE_WET,SYS_COOL,SYS_HEAT,0,SYS_SEND_WIND};
static  const uint8 CODE  PANASONIC_WIND[]       =  {0,1,2,3,0,0};
static  const uint8 CODE  HAIER_NEWsysmode[7]    =  {0,SYS_DIVIDE_WET,SYS_COOL,SYS_HEAT,0,0,0};
static  const uint8 CODE  HAIER_NEWwind[6]       =  {0,2,3,1,0,0};
static  const uint8 CODE  HAIER_NEW1sysmode[7]   =  {SYS_AUTO,SYS_SEND_WIND,SYS_DIVIDE_WET,0,SYS_COOL,0,SYS_HEAT};
static  const uint8 CODE  HAIER_NEW1wind[6]      =  {0,2,3,1,0,0};
static  const uint8 CODE  LG_SYSMODE[7]          =  {SYS_COOL,SYS_HEAT,SYS_SEND_WIND,0,SYS_DIVIDE_WET,0,SYS_AUTO};
static  const uint8 CODE  LG_WIND[6]             =  {1,3,2,255,255,0};
static  const uint8 CODE  SUMSUNG_SYSMODE[5]     =  {SYS_AUTO,SYS_COOL,SYS_DIVIDE_WET,SYS_SEND_WIND,SYS_HEAT};
static  const uint8 CODE  SUMSUNG_WIND[6]        =  {0,0,1,0,2,3};
static  const uint8 CODE  FUJITSU_SYSMODE[7]     =  {SYS_AUTO,SYS_COOL,SYS_DIVIDE_WET,SYS_SEND_WIND,SYS_HEAT};
static  const uint8 CODE  FUJITSU_WIND[6]        =  {0,3,2,1,0,255};
static  const uint8 CODE  AUXsysmodeNEW[5]       =  {SYS_AUTO,SYS_COOL,SYS_DIVIDE_WET,SYS_SEND_WIND,SYS_HEAT};
/*******************************************************************************
* LOCAL FUNCTIONS
*/
static void gree_analysis(uint16  pos);    //格力
static void midea_analysis(uint16  pos);   //美的
static void chigo_analysis(uint16  pos);   //志高
static void aux_analysis(uint16  pos);     //奥克斯
static void hisense_analysis(uint16  pos); //海信
static void haier_analysis(uint16  pos);   //海尔
static void aucma_analysis(uint16  pos);   //澳柯玛
static void shinco_analysis(uint16  pos) ;  //新科
static void mitsubishi_analysis(uint16  pos);//三菱重工
static void yair_analysis(uint16  pos);      //扬子空调
static void hualing_analysis(uint16  pos);   //华凌空调
static void kelon_analysis(uint16  pos);     //科龙空调
static void daikin_analysis(uint16  pos);    //大金空调 
static void chunlan_analysis(uint16  pos);   //春兰空调
static void tcl_analysis(uint16  pos);       //TCL空调  
static void changhong_analysis(uint16  pos); //CHANGHONG 长虹
static void galanz_analysis(uint16  pos);    //格兰仕 
static void hitachi_analysis(uint16  pos);   //日立空调 
static void panasonic_analysis(uint16  pos); //松下空調
static void LG_analysis(uint16  pos);        //LG空調   
static void sumsung_analysis(uint16  pos);   //三星空調
static void fujitsu_analysis(uint16  pos);   //富士通空調
static void (*pmatchup[])(uint16  pos) = {gree_analysis, chigo_analysis, midea_analysis, 
NULL,aux_analysis,hisense_analysis,haier_analysis,aucma_analysis,shinco_analysis,mitsubishi_analysis,
yair_analysis,hualing_analysis,kelon_analysis,daikin_analysis,chunlan_analysis,NULL,NULL,tcl_analysis,
changhong_analysis,NULL,galanz_analysis,NULL,hitachi_analysis,panasonic_analysis,LG_analysis,sumsung_analysis,
NULL,fujitsu_analysis};
/*******************************************************************************
* @fn       static void shift_data(uint8  value,uint8 bits)
*
* @brief    将 pvalue 数据的低 bits 位，按位颠倒
*
* @param    uint8 pvalue,uint8 bits
*
* @return   none
****************************************************************************/
static uint8 shift_data(uint8 pvalue,uint8 bits)
{
  uint8  v = 0;
  v = pvalue;
  v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);  // 交换每两位
  v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);  // 交换每四位中的前两位和后两位
  v = ( v >> 4) |  (v  << 4);                 // 交换前四位和后四位
  v >>=  (8 - bits);
  return v;
}



/*******************************************************************************
* @fn         static uint8  fetch_data(uint8 pos, uint8 scope)
*
* @brief       从数组中提取出需要的参数,pos位置，取scope位
*
* @param       uint8 pos, uint8 scope
*
* @return      uint8
****************************************************************************/
static uint8  fetch_data(uint8 *pdata,uint8 pos, uint8 scope)
{
  uint8  m = 0,n = 0;
  if(scope > 8)
  {
    return false;
  }
  pos -= 1;//和字节位定义一致。
  m = (*(pdata + (pos >> 3)) >> ((8 - (pos & 7)) - scope));//向右移动合适的位
  while(scope != 0)                                        //取出想要的位到n。
  {
    scope--;
    n <<= 0x01;
    n |= (m & 0x01);
    m >>= 0x01;
  }
  return n;
}



/*******************************************************************************
* @fn        static void  fetch_all(uint8 dev_mark_t)
*
* @brief       从数组中提取出需要的参数,pos位置，取scope位
*
* @param       uint8 pos, uint8 scope
*
* @return      uint8
****************************************************************************/
static void  fetch_all(uint8 dev_mark_t)
{
  ir_matchup_result.curSetTemp   = fetch_data(pTempLargeBuf,ir_para[dev_mark_t][TEMP_POS],ir_para[dev_mark_t][TEMP_SCOPE]);
  ir_matchup_result.bPowerOnOff  = fetch_data(pTempLargeBuf,ir_para[dev_mark_t][ONOff_POS],ir_para[dev_mark_t][ONOff_SCOPE]);
  ir_matchup_result.sysMode      = fetch_data(pTempLargeBuf,ir_para[dev_mark_t][SYSMODE_POS], ir_para[dev_mark_t][SYSMODE_SCOPE]);
  ir_matchup_result.windLevel    = fetch_data(pTempLargeBuf,ir_para[dev_mark_t][WINDLEVEL_POS],ir_para[dev_mark_t][WINDLEVEL_SCOPE]);
}



/*******************************************************************************
* @fn       static void gree_analysis(uint16  pos)
*
* @brief     格力空调有效数据提取
*
* @param      void
*
* @return      void
****************************************************************************/
static void gree_analysis(uint16  pos)
{
  if((35 == pos) || (65 < pos))///电视机遥控器对格力遥控器形成干扰
  {
    fetch_all(dev_mark);
    if((ir_matchup_result.sysMode < 9) &&
       (ir_matchup_result.windLevel < 5)&&
         (ir_matchup_result.curSetTemp < 16))
    { 
      ir_matchup_result.curSetTemp = (ir_matchup_result.curSetTemp+16);
      ir_matchup_result.sysMode = GREEsysmode[ir_matchup_result.sysMode];
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
    }
  }
  else
  {
    ir_matchup_result.matchup_result =  LENGTH_ERROR;
  }
}



/*******************************************************************************
* @fn        void midea_analysis( void)
*
* @brief      美的空调有效数据提取
*
* @param      void
*
* @return     void
****************************************************************************/
static void midea_analysis(uint16  pos)
{
  if((pTempLargeBuf[0] ==  0xB2) && (pTempLargeBuf[1] ==  0X4D))//
  {
    if(140< pos)
    {            //省电或者定时指令
      fetch_all(dev_mark+1);
    }
    else if(90< pos)
    {            //正常指令
      fetch_all(dev_mark);
    }
    else
    {
      ir_matchup_result.matchup_result = LENGTH_ERROR;
    }
  }
  else
    if(pTempLargeBuf[0] ==  0xB3)
    {                             //舒睡功能
      asm("NOP");
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
    }

  if(pTempLargeBuf[0] ==  0xB3)
  {                             //舒睡功能
    ir_matchup_result.bPowerOnOff = 1;
    ir_matchup_result.curSetTemp = 0;
    ir_matchup_result.sysMode = SYS_COMFORT;
    ir_matchup_result.windLevel = 0;
    ir_matchup_result.matchup_result = true;
  }
  else
    if((pTempLargeBuf[2] ==  0x7B) && (pTempLargeBuf[4] ==   0XE0))
    {
      ir_matchup_result.bPowerOnOff = 0;
      ir_matchup_result.matchup_result = true;
    }else
      if((ir_matchup_result.matchup_result == true) && (ir_matchup_result.sysMode < 8) &&
         (ir_matchup_result.windLevel < 8) && (ir_matchup_result.curSetTemp < 16))
      {
        ir_matchup_result.bPowerOnOff = 1;
        ir_matchup_result.curSetTemp = MIDEAtemperature[ir_matchup_result.curSetTemp];
        ir_matchup_result.sysMode = MIDEAsysmode[ir_matchup_result.sysMode];
        ir_matchup_result.windLevel = MIDEAwind[ir_matchup_result.windLevel];
        if((SYS_DIVIDE_WET == ir_matchup_result.sysMode) && (0 == ir_matchup_result.curSetTemp))
        {
          ir_matchup_result.sysMode = SYS_SEND_WIND;
	        ir_matchup_result.curSetTemp=25;
        }
      }
}



/*******************************************************************************
* @fn        static void chigo_analysis(void)
*
* @brief      志高空调有效数据提取
*
* @param      void
*
* @return     void
****************************************************************************/
static void chigo_analysis(uint16  pos)
{
  if((0xff == pTempLargeBuf[0]) && (0x00 == pTempLargeBuf[1]) && (0xff == pTempLargeBuf[2])&& (0x00 == pTempLargeBuf[3]) && (90 < pos))
  {
    fetch_all(dev_mark);
    ir_matchup_result.bPowerOnOff = !ir_matchup_result.bPowerOnOff;     //二次调整
    if((ir_matchup_result.sysMode < 8) &&
       (ir_matchup_result.windLevel < 8)&&
       (ir_matchup_result.curSetTemp < 33))
    {
      ir_matchup_result.sysMode = CHIGOsysmode[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel = CHIGOwind[ir_matchup_result.windLevel];
      ir_matchup_result.curSetTemp = (47 - ir_matchup_result.curSetTemp);
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
    }
  }
  else
  {
    ir_matchup_result.matchup_result =  LENGTH_ERROR;
  }
}



/*******************************************************************************
* @fn        static void aux_analysis(void)
*
* @brief      奥克斯空调有效数据提取
*
* @param      void
*
* @return     void
****************************************************************************/
static void aux_analysis(uint16  pos)
{
  if(0xC3 == pTempLargeBuf[0] && (100 < pos))
  {
    fetch_all(dev_mark);
    if((ir_matchup_result.sysMode < 0x0f) &&
       (ir_matchup_result.windLevel < 0x0f)&&
       (ir_matchup_result.curSetTemp < 26))
    {
      ir_matchup_result.curSetTemp += 8;
      ir_matchup_result.sysMode = AUXsysmode[ir_matchup_result.sysMode];
      if(System_ConfigInface.iAirModel == 235667456)
      {
        ir_matchup_result.windLevel = AUXwind_011[ir_matchup_result.windLevel];  
      }else{
        ir_matchup_result.windLevel = AUXwind[ir_matchup_result.windLevel];
      }
      if(ir_matchup_result.curSetTemp < 16)
      {
        ir_matchup_result.curSetTemp = 0;
      }
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
    }
  }else
    if(0x03 == pTempLargeBuf[0] && (30 < pos))
  {
    fetch_all(dev_mark+24);
    if((ir_matchup_result.sysMode < 0x06) &&
       (ir_matchup_result.windLevel < 0x06)&&
         (ir_matchup_result.curSetTemp < 16))
    {
      ir_matchup_result.bPowerOnOff = !ir_matchup_result.bPowerOnOff;     
      ir_matchup_result.curSetTemp += 16;
      ir_matchup_result.sysMode = AUXsysmodeNEW[ir_matchup_result.sysMode];
      //ir_matchup_result.windLevel = AUXwind[ir_matchup_result.windLevel];
      if(ir_matchup_result.curSetTemp < 16)
      {
        ir_matchup_result.curSetTemp = 0;
      }
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
    }
    
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}



/*******************************************************************************
* @fn        static void hisense_analysis(void)
*
* @brief      海信空调有效数据提取
*
* @param      void
*
* @return     void
****************************************************************************/
static void hisense_analysis(uint16  pos)
{
  if((0x02 == pTempLargeBuf[0]) && (130 < pos))//TEST
  {
    fetch_all(dev_mark);
    if((ir_matchup_result.sysMode < 0x0a) &&
       (ir_matchup_result.windLevel < 0x04)&&
       (ir_matchup_result.curSetTemp < 30))
    {
      ir_matchup_result.curSetTemp += 4;
      ir_matchup_result.windLevel = HISENSEwind[ir_matchup_result.windLevel];
      ir_matchup_result.sysMode = HISENSEsysmode[ir_matchup_result.sysMode];
      if(0x10 == pTempLargeBuf[12])
      {
        ir_matchup_result.sysMode = SYS_COMFORT;
      }
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
    }
  }else
    if((0xC1 == pTempLargeBuf[0]) && (130 < pos)) 
    {
      fetch_all(dev_mark + 10); //海信 新款（型号DG11J1-03）
      if((ir_matchup_result.sysMode < 0x06) &&
         (ir_matchup_result.windLevel < 0x04)&&
         (ir_matchup_result.curSetTemp < 15))
      {
        ir_matchup_result.bPowerOnOff = !ir_matchup_result.bPowerOnOff;
        if(0 == ir_matchup_result.bPowerOnOff)
        {
          //r_matchup_result.bPowerOnOff = OPPOSITE;//切忌：当是开机状态时，对于空调来说是取反的！
          if(System_ConfigInface.SystemIrda.bPowerOnOff)
          {
            ir_matchup_result.bPowerOnOff = 0;
          }
          else
          {
            ir_matchup_result.bPowerOnOff = 1;
          }
        }
        ir_matchup_result.curSetTemp += 18;
        ir_matchup_result.windLevel = HISENSE_NEWwind[ir_matchup_result.windLevel];
        ir_matchup_result.sysMode = HISENSE_NEWsysmode[ir_matchup_result.sysMode];
      }
      else
      {
        ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
      }  
    }
    else
    {
      ir_matchup_result.matchup_result = LENGTH_ERROR;
    }
}


/*******************************************************************************
* @fn        static void hisense_analysis(void)
*
* @brief      海尔空调有效数据提取
*
* @param      void
*
* @return     void
****************************************************************************/
static void haier_analysis(uint16  pos)
{
  if((0xA6 == pTempLargeBuf[0]) && (105 < pos))//TEST
  {
    fetch_all(dev_mark);
    ir_matchup_result.sysMode = shift_data(ir_matchup_result.sysMode,3);
    if((ir_matchup_result.sysMode < 0x07) &&     //另一款遥控器要改此参数 
       (ir_matchup_result.windLevel < 0x07)&&
       (ir_matchup_result.curSetTemp < 17))
    {                                                                  //温度，数据顺序要反过来
      ir_matchup_result.curSetTemp = shift_data(ir_matchup_result.curSetTemp,4) ;
      ir_matchup_result.curSetTemp += 16;
      ir_matchup_result.windLevel = HAIERwind[ir_matchup_result.windLevel];
      ir_matchup_result.sysMode   = HAIERsysmode[ir_matchup_result.sysMode];
      if(0xA8 == pTempLargeBuf[5])
      {
        ir_matchup_result.sysMode = SYS_COMFORT;
      }
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR;  
    } 
  }else
    if((0xA5 == pTempLargeBuf[0]) && (71 < pos))//TEST
    {
      fetch_all(dev_mark+25);
      // ir_matchup_result.sysMode = shift_data(ir_matchup_result.sysMode,3);
      if((ir_matchup_result.sysMode < 0x07) &&     //áíò???ò￡???÷òa??′?2?êy 
         (ir_matchup_result.windLevel < 0x07)&&
          (ir_matchup_result.curSetTemp < 17))
      {                                                                  //???è￡?êy?Y?3Dòòa·′1yà′
        ir_matchup_result.curSetTemp = shift_data(ir_matchup_result.curSetTemp,4) ;
        ir_matchup_result.curSetTemp += 16;
        ir_matchup_result.windLevel = HAIER_NEW1wind[ir_matchup_result.windLevel];
        ir_matchup_result.sysMode   = HAIER_NEW1sysmode[ir_matchup_result.sysMode];
        
        if(0x00 == (pTempLargeBuf[1]& 0x0f))
        {
          ir_matchup_result.bPowerOnOff = 0;
        }
        else
        {
         ir_matchup_result.bPowerOnOff = 1;
        }
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR;  
    } 
  }else
   if((0x0E == pTempLargeBuf[0]) && (105 < pos))  // 兼容海尔YR-C02
   {
      fetch_all(dev_mark+13);    
      ir_matchup_result.curSetTemp = shift_data(ir_matchup_result.curSetTemp,4) ;
      if((ir_matchup_result.sysMode < 0x05) &&      
         (ir_matchup_result.windLevel < 0x05)&&
         (ir_matchup_result.curSetTemp < 17))
    {                                                
      ir_matchup_result.curSetTemp += 16;
      ir_matchup_result.windLevel = HAIER_NEWwind[ir_matchup_result.windLevel];
      ir_matchup_result.sysMode   = HAIER_NEWsysmode[ir_matchup_result.sysMode];
      if(0x00 == ir_matchup_result.sysMode)
      {
        ir_matchup_result.bPowerOnOff = 0;
      }
      else
      {
        ir_matchup_result.bPowerOnOff = 1;
      }
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR;  
    }
  }
  else
  {
    ir_matchup_result.matchup_result =  LENGTH_ERROR;
  }
}



/*******************************************************************************
* @fn       static void aucma_analysis(uint16  pos)
*
* @brief      澳柯玛空调有效数据提取
*
* @param     uint16  pos
*
* @return     void
****************************************************************************/
static void aucma_analysis(uint16  pos)
{
  if( (92 < pos)&&(0x55 == pTempLargeBuf[0]))//TEST
  {
    fetch_all(dev_mark);
    if((ir_matchup_result.sysMode < 0x06) &&
       (ir_matchup_result.windLevel < 0x04)&&
         (ir_matchup_result.curSetTemp < 17))
    {
      ir_matchup_result.curSetTemp += 16;
      ir_matchup_result.sysMode   = AUCMAsysmode[ir_matchup_result.sysMode];
      if(0x10 == (pTempLargeBuf[6] & 0x10))
      {
        ir_matchup_result.sysMode = SYS_COMFORT;
      }
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
    }
  }
  else
  {
    ir_matchup_result.matchup_result =  LENGTH_ERROR;
  }
}



/*******************************************************************************
* @fn         static void shinco_analysis(uint16  pos)
*
* @brief      新科空调有效数据提取
*
* @param     uint16  pos
*
* @return     void
****************************************************************************/
static void shinco_analysis(uint16  pos) //
{
   if( (190 < pos)&&(0x18 == pTempLargeBuf[0]))
  {
    fetch_all(dev_mark+21);
    ir_matchup_result.curSetTemp = shift_data(ir_matchup_result.curSetTemp,4);
    if((ir_matchup_result.sysMode < 0x07) &&
       (ir_matchup_result.windLevel < 0x09)&&
       (ir_matchup_result.curSetTemp < 17))
    {
      asm("NOP");
      ir_matchup_result.curSetTemp += 16;
      ir_matchup_result.sysMode   = SHINCO_NEW2sysmode[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel = SHINCO_NEW2wind[ir_matchup_result.windLevel];
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
    }
  }else
  if( (70 < pos)&&(0x55 == pTempLargeBuf[0]))
  {
    fetch_all(dev_mark+8);
    ir_matchup_result.curSetTemp = shift_data(ir_matchup_result.curSetTemp,4);
    if((ir_matchup_result.sysMode < 0x07) &&
       (ir_matchup_result.windLevel < 0x08)&&
         (ir_matchup_result.curSetTemp < 15))
    {
      asm("NOP");
      ir_matchup_result.curSetTemp += 16;
      ir_matchup_result.sysMode   = SHINCO_NEWsysmode[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel = SHINCO_NEWwind[ir_matchup_result.windLevel];
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
    }
  }else
  if( (38 < pos)&&(0x18 == pTempLargeBuf[0]))
  {
    fetch_all(dev_mark);
    ir_matchup_result.curSetTemp = shift_data(ir_matchup_result.curSetTemp,4);
    if((ir_matchup_result.sysMode < 0x07) &&
       (ir_matchup_result.windLevel < 0x08)&&
       (ir_matchup_result.curSetTemp < 17))
    {
      asm("NOP");
      ir_matchup_result.curSetTemp += 16;
      ir_matchup_result.sysMode   = SHINCOsysmode[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel = SHINCOwind[ir_matchup_result.windLevel];
      if(0 == ir_matchup_result.sysMode)
      {
        ir_matchup_result.bPowerOnOff = 0;
      }else
      {
        ir_matchup_result.bPowerOnOff = 1;
      }
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; //LENGTH_ERROR;
    }
  }
  else 
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}



/*******************************************************************************
* @fn        static void mitsubishi_analysis(uint16  pos) 
*
* @brief     三菱重工空调有效数据提取 MITSUBISHI
*
* @param     uint16  pos
*  
* @return     void 
****************************************************************************/
static void mitsubishi_analysis(uint16  pos) //
{
 if((MITSUBISHI_PULSE_NUM < pos)&&(MITSUBISHI_LABEL == pTempLargeBuf[4])) //38判断前导码和红外码脉冲数量范围  
  {
    fetch_all(dev_mark); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 0x05) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 0x06)&& //判断风力等级数值范围
       (ir_matchup_result.curSetTemp < 17))
    {
       asm("NOP");
       ir_matchup_result.curSetTemp += 17;
       ir_matchup_result.sysMode   = MITSUBISHI_SYSMODE[ir_matchup_result.sysMode];
       ir_matchup_result.windLevel = MITSUBISHI_WIND[ir_matchup_result.windLevel];
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }else
   if((MITSUBISHI_PULSE_NUM1 < pos)&&(MITSUBISHI_LABEL1 == pTempLargeBuf[0]))
   {
     fetch_all(dev_mark + 21); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 0x09) &&  //?D???μí3?￡ê?êy?μ·??§
       (ir_matchup_result.windLevel < 0x08)&& //?D??·?á|μè??êy?μ·??§
       (ir_matchup_result.curSetTemp < 20))
    {
       asm("NOP");
       ir_matchup_result.bPowerOnOff = !ir_matchup_result.bPowerOnOff;
       ir_matchup_result.curSetTemp = (32 - ir_matchup_result.curSetTemp);
       ir_matchup_result.sysMode   = MITSUBISHI_SYSMODE1[ir_matchup_result.sysMode];
       ir_matchup_result.windLevel = MITSUBISHI_WIND1[ir_matchup_result.windLevel];
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
     
   }
  else
  {
   ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}



/*******************************************************************************
* @fn        static void yair_analysis(uint16  pos) 
*
* @brief     扬子空调有效数据提取  
*
* @param     uint16  pos
*  
* @return     void 
****************************************************************************/
static void yair_analysis(uint16  pos) //扬子空调 
{
  if((YAIR_PULSE_NUM < pos)&&(YAIR_LABEL == pTempLargeBuf[0])) //&&(YAIR_LABEL == pTempLargeBuf[0])38判断前导码和红外码脉冲数量范围  
  {
    fetch_all(dev_mark); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 0x05) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 0x06)&& //判断风力等级数值范围
       (ir_matchup_result.curSetTemp < 32))
    {
       asm("NOP");
       ir_matchup_result.curSetTemp = (31 - ir_matchup_result.curSetTemp);
       ir_matchup_result.sysMode    = YAIR_SYSMODE[ir_matchup_result.sysMode];
       ir_matchup_result.windLevel  = YAIR_WIND[ir_matchup_result.windLevel];
       asm("NOP");
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}



/*******************************************************************************
* @fn        static void hualing_analysis(uint16  pos) 
*
* @brief     扬子空调有效数据提取  
*
* @param     uint16  pos
*  
* @return     void 
****************************************************************************/
static void hualing_analysis(uint16  pos) 
{
  if((pTempLargeBuf[0] ==  0xB2) && (pTempLargeBuf[1] ==  0X4D))//
  { 
    fetch_all(3); //兼容美的
    if((pTempLargeBuf[2] ==  0x7B) && (pTempLargeBuf[4] == 0XE0))
    {
      ir_matchup_result.bPowerOnOff = 0;
      ir_matchup_result.matchup_result = true;
    }else
      if((ir_matchup_result.matchup_result == true) && (ir_matchup_result.sysMode < 8) &&
         (ir_matchup_result.windLevel < 8) && (ir_matchup_result.curSetTemp < 16))
      {
        ir_matchup_result.bPowerOnOff = 1;
        ir_matchup_result.curSetTemp = MIDEAtemperature[ir_matchup_result.curSetTemp];
        ir_matchup_result.sysMode = MIDEAsysmode[ir_matchup_result.sysMode];
        ir_matchup_result.windLevel = MIDEAwind[ir_matchup_result.windLevel];
        if((SYS_DIVIDE_WET == ir_matchup_result.sysMode) && (0 == ir_matchup_result.curSetTemp))
        {
          ir_matchup_result.sysMode = SYS_SEND_WIND;
    	  ir_matchup_result.curSetTemp = 25;
        }
      }
  }
  else if((HUALING_PULSE_NUM < pos)&&(HUALING_LABEL == pTempLargeBuf[0])) //&&(YAIR_LABEL == pTempLargeBuf[0])38?D???°μ???oíoìía????3?êyá?·??§  
  {
    fetch_all(dev_mark); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 0x09) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 0x06)&& //判断风力等级数值范围
       (ir_matchup_result.curSetTemp < 32))
    {
       asm("NOP");  
       ir_matchup_result.curSetTemp = (31 - ir_matchup_result.curSetTemp);
       ir_matchup_result.sysMode   = HUALING_SYSMODE[ir_matchup_result.sysMode];
       ir_matchup_result.windLevel = HUALING_WIND[ir_matchup_result.windLevel];
       asm("NOP");
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  } 
}


/*******************************************************************************
* @fn        static void kelon_analysis(uint16  pos) 
*
* @brief     科龙空调有效数据提取  
*
* @param     uint16  pos
*  
* @return     void 
****************************************************************************/
static void kelon_analysis(uint16  pos) 
{
  if(( KELON_PULSE_NUM < pos) && (KELON_LABEL == pTempLargeBuf[0])) //&&(YAIR_LABEL == pTempLargeBuf[0])38判断前导码和红外码脉冲数量范围  
  {
    fetch_all(dev_mark); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 6) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 4)&& //判断风力等级数值范围 
       (ir_matchup_result.curSetTemp < 15))
    {
      asm("NOP");  
      ir_matchup_result.bPowerOnOff = !ir_matchup_result.bPowerOnOff;
      if(0 == ir_matchup_result.bPowerOnOff)
      {
          // ir_matchup_result.bPowerOnOff = OPPOSITE;//切忌：当是开机状态时，对于空调来说是取反的！opposite
          if(System_ConfigInface.SystemIrda.bPowerOnOff)
          {
            ir_matchup_result.bPowerOnOff = 0;
          }
          else
          {
            ir_matchup_result.bPowerOnOff = 1;
          }
      }
      ir_matchup_result.curSetTemp +=18;
      ir_matchup_result.sysMode   = KELON_SYSMODE[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel = KELON_WIND[ir_matchup_result.windLevel];
      asm("NOP");
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}



/*******************************************************************************
* @fn        static void chunlan_analysis(uint16  pos)  
*
* @brief     春兰空调有效数据提取  
*
* @param     uint16  pos
*  
* @return     void 
****************************************************************************/
static void chunlan_analysis(uint16  pos) 
{
  if(( CHUNLAN_PULSE_NUM < pos) && (CHUNLAN_LABEL == pTempLargeBuf[8])) //
  {
    fetch_all(dev_mark); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 6) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 4)&& //判断风力等级数值范围 
       (ir_matchup_result.curSetTemp < 32))
    { 
       ir_matchup_result.curSetTemp = CHUNLAN_temperature[ir_matchup_result.curSetTemp];
       ir_matchup_result.sysMode    = CHUNLAN_SYSMODE[ir_matchup_result.sysMode];
       ir_matchup_result.windLevel  = CHUNLAN_WIND[ir_matchup_result.windLevel];
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}



/*******************************************************************************
* @fn        static void tcl_analysis(uint16  pos)  
*
* @brief     TCL空调有效数据提取  
*
* @param     uint16  pos
*  
* @return     void 
****************************************************************************/
static void tcl_analysis(uint16  pos)
{
   if((TCL_PULSE_NUM < pos) && (TCL_LABEL == pTempLargeBuf[0])) //
  {
    fetch_all(dev_mark); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 8) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 6)&& //判断风力等级数值范围 
       (ir_matchup_result.curSetTemp < 20))
    { 
       ir_matchup_result.curSetTemp = (31-ir_matchup_result.curSetTemp);
       ir_matchup_result.sysMode    = TCL_SYSMODE[ir_matchup_result.sysMode];
       ir_matchup_result.windLevel  = TCL_WIND[ir_matchup_result.windLevel];
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  } 
}



/*******************************************************************************
* @fn        static void daikin_analysis(uint16  pos)  
*
* @brief     大金空调 有效数据提取 
*
* @param     uint16  pos
*  
* @return     void 
****************************************************************************/
static void daikin_analysis(uint16  pos)
{
  if((DAIKIN_PULSE_NUM < pos) && (DAIKIN_LABEL == pTempLargeBuf[0])) //
  {
    fetch_all(dev_mark); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 15) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 12)&& //判断风力等级数值范围 
       (ir_matchup_result.curSetTemp < 16))
    { 
       ir_matchup_result.curSetTemp += 16;
       ir_matchup_result.sysMode    = DAIKIN_SYSMODE[ir_matchup_result.sysMode];
       ir_matchup_result.windLevel  = DAIKIN_WIND[ir_matchup_result.windLevel];
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  } 
}



/*******************************************************************************
* @fn        static void changhong_analysis(uint16  pos)  
*
* @brief     长虹空调有效数据提取 
*
* @param     uint16  pos
*  
* @return    void  
****************************************************************************/
static void changhong_analysis(uint16  pos)  
{
  if((CHANGHONG_PULSE_NUM < pos) && (CHANGHONG_LABEL == pTempLargeBuf[0])) //
  {
    fetch_all(dev_mark); 
    asm("NOP");
    ir_matchup_result.bPowerOnOff = !ir_matchup_result.bPowerOnOff;
    // ir_matchup_result.curSetTemp = shift_data(ir_matchup_result.curSetTemp,4);
    if((ir_matchup_result.sysMode < 7) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 5)&& //判断风力等级数值范围 
       (ir_matchup_result.curSetTemp < 28))
    { 
      ir_matchup_result.curSetTemp += 4;
      ir_matchup_result.sysMode    = CHANGHONG_SYSMODE[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel  = CHANGHONG_WIND[ir_matchup_result.windLevel];
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}




/*******************************************************************************
* @fn        static void galanz_analysis(uint16  pos)  
*
* @brief     ??à?ê?  ??μ÷óDD§êy?Yìáè? 
*
* @param     uint16  pos
*  
* @return    void  
****************************************************************************/
static void galanz_analysis(uint16  pos)    
{
  if((GALANZ_PULSE_NUM < pos) && (GALANZ_LABEL == pTempLargeBuf[0])) //
  {
    fetch_all(dev_mark); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 10) &&  
       (ir_matchup_result.windLevel < 7)&& 
         (ir_matchup_result.curSetTemp < 20))
    { 
      ir_matchup_result.curSetTemp  = 31-ir_matchup_result.curSetTemp;
      ir_matchup_result.sysMode    = GALANZ_SYSMODE[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel  = GALANZ_WIND[ir_matchup_result.windLevel];
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}
/*******************************************************************************
* @fn        static void hitachi_analysis(uint16  pos)
*
* @brief     è?á￠??μ÷??μ÷óDD§êy?Yìáè? 
*
* @param     uint16  pos
*  
* @return    void  
****************************************************************************/
static void hitachi_analysis(uint16  pos) 
{
  if((HITACHI_PULSE_NUM < pos) && (HITACHI_LABEL == pTempLargeBuf[0])) //
  {
    fetch_all(dev_mark); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 8) &&  
       (ir_matchup_result.windLevel < 7)&& 
       (ir_matchup_result.curSetTemp < 20))
    { 
      ir_matchup_result.bPowerOnOff = !ir_matchup_result.bPowerOnOff;
      ir_matchup_result.curSetTemp  = 31-ir_matchup_result.curSetTemp;
      ir_matchup_result.sysMode    = HITACHI_SYSMODE[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel  = HITACHI_WIND[ir_matchup_result.windLevel];
      asm("NOP");
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}



/*************************************************
* @fn          static void panasonic_analysis(uint16  pos)   
*
* @brief        松下空調
*
* @param       uint16  pos
*  
* @return      void
***********************************************/
static void panasonic_analysis(uint16  pos)  
{
  if((PANASONIC_PULSE_NUM < pos) && (PANASONIC_LABEL == pTempLargeBuf[0])) //
  {
    fetch_all(dev_mark); 
    asm("NOP");
    if((ir_matchup_result.sysMode < 8) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 7)&& //判断风力等级数值范围 
         (ir_matchup_result.curSetTemp < 16))
    { 
      ir_matchup_result.curSetTemp  += 16;
      ir_matchup_result.sysMode    = PANASONIC_SYSMODE[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel  = PANASONIC_WIND[ir_matchup_result.windLevel];
      if((SYS_AUTO == ir_matchup_result.sysMode) && (31 == ir_matchup_result.curSetTemp))
      {
       ir_matchup_result.curSetTemp = 16; 
      }
      asm("NOP");
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}
/*************************************************
* @fn          get_ir_big_length
*
* @brief       ??è?1,0?¨ò?à???±è???íμ????í?μ
*
* @param       uint8  *irdata
*
* @return      uint8
***********************************************/
static void LG_analysis(uint16  pos)        //LG空調  
{
  if((LG_PULSE_NUM < pos) && (LG_LABEL == pTempLargeBuf[0])) //
  {
    fetch_all(dev_mark); 
    ir_matchup_result.curSetTemp = shift_data(ir_matchup_result.curSetTemp,4);   
    if((ir_matchup_result.sysMode < 8) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 7)&& //判断风力等级数值范围 
         (ir_matchup_result.curSetTemp < 16))
    { 
      ir_matchup_result.bPowerOnOff = !ir_matchup_result.bPowerOnOff;
      ir_matchup_result.curSetTemp  += 15;
      ir_matchup_result.sysMode    = LG_SYSMODE[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel  = LG_WIND[ir_matchup_result.windLevel];
   
      asm("NOP");
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}



/*************************************************
* @fn          get_ir_big_length
*
* @brief       ??è?1,0?¨ò?à???±è???íμ????í?μ
*
* @param       uint8  *irdata
*
* @return      uint8     
***********************************************/
static void sumsung_analysis(uint16  pos)  //三星空調
{
  if((SUMSUNG_PULSE_NUM < pos) && (SUMSUNG_LABEL == pTempLargeBuf[0])) //
  {
    if(SUMSUNG_PULSE_NUM_LONG < pos)
    {
      fetch_all(dev_mark+1);
    }else
    {
      fetch_all(dev_mark); 
    }
    if((ir_matchup_result.sysMode < 8) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 7)&& //判断风力等级数值范围 
         (ir_matchup_result.curSetTemp < 16))
    { 
      ir_matchup_result.bPowerOnOff = !ir_matchup_result.bPowerOnOff;
      ir_matchup_result.curSetTemp  += 16;
      ir_matchup_result.sysMode    = SUMSUNG_SYSMODE[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel  = SUMSUNG_WIND[ir_matchup_result.windLevel];
      asm("NOP");
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}



/*************************************************
* @fn          fujitsu_analysis
* 
* @brief        
*
* @param       uint16  pos
*
* @return      void   
***********************************************/
static void  fujitsu_analysis(uint16  pos)  //富士通空調
{
  if((FUJITSU_PULSE_NUM < pos) && (FUJITSU_LABEL == pTempLargeBuf[0])) //
  {
    fetch_all(dev_mark); 
    if((ir_matchup_result.sysMode < 8) &&  //判断系统模式数值范围
       (ir_matchup_result.windLevel < 7)&& //判断风力等级数值范围 
         (ir_matchup_result.curSetTemp < 16))
    { 
      ir_matchup_result.bPowerOnOff =1;
      ir_matchup_result.curSetTemp  += 16;
      ir_matchup_result.sysMode    = FUJITSU_SYSMODE[ir_matchup_result.sysMode];
      ir_matchup_result.windLevel  = FUJITSU_WIND[ir_matchup_result.windLevel];
      asm("NOP");
    }
    else
    {
      ir_matchup_result.matchup_result = DATA_ERROR; 
    }
  }else if((47 < pos) && ( 60 > pos) && (FUJITSU_LABEL == pTempLargeBuf[0]))
  {
     ir_matchup_result.bPowerOnOff =0;
  }
  else
  {
    ir_matchup_result.matchup_result = LENGTH_ERROR;
  }
}


/*******************************************************************************
* @fn          get_ir_length
*
* @brief       获取红外脉冲中逻辑数緈脉宽的长度。
*
* @param       IRlearn_t *IRcode
*
* @return      none
*******************************************************************************/
static void  get_ir_length(void)
{
#ifdef   MATCH_BRAND //自动匹配型号功能需要取出首位无用脉冲
  uint16  i;
  if(capture_temp.matchup_source)
  {
    for(i=0;i< capture_temp.count_last;i++)
    {
      *(pRecvAndSendPulseBuf + i) = *(pRecvAndSendPulseBuf + i + 1);
    }
    capture_temp.count_last -= 1;
  }
#endif
  capture_temp.iLogic0HTime = *(pRecvAndSendPulseBuf + 6);
  capture_temp.iLogic0LTime = get_ir_narrow_length(pRecvAndSendPulseBuf + 5);
  capture_temp.iLogic1HTime = *(pRecvAndSendPulseBuf + 6);
  capture_temp.iLogic1LTime = get_ir_big_length(pRecvAndSendPulseBuf + 5);  
}


/*************************************************************************
* @fn          ircode_capture_init
*
* @brief       启动红外脉冲信号捕捉
*
* @param       none
*
* @return      none
************************************************************************/
static void  ircode_capture_init(void)
{ 
  Carrierwave_port = true;
  capture_temp.count_last = false;
  capture_temp.count = false;
  capture_temp.time_count = false;
  capture_temp.time_flag  = false;
  capture_temp.matchup_state = 0;
  capture_temp.iLogic1HTime = 0;
  capture_temp.iLogic1LTime = 0;
  capture_temp.iLogic0HTime = 0;
  capture_temp.iLogic0LTime = 0; 
  capture_temp.ovfif_times  = 0;
  g_cTimer1_Mode  = IRSTUDY_MODE;
  dev_mark = System_ConfigInface.iAirBrand;
}


/*******************************************************************************
* @fn          Timer1_process
*
* @brief       定时器1处理
*
* @param       none
*
* @return      none
******************************************************************************/
/*++++++关闭定时器1+++++*/
void    Timer1_CaptureOff(void)
{
  T1CTL   &= 0xF0;
  T1STAT  &= 0xE0;
  T1CCTL3  = 0; //禁止通道3中断
  T1CCTL4  = 0; //禁止通道3中断
  IRCON   &= ~(1<<IRCON_T1IF); //清定时器T1中断请求
  IEN1    &= ~(1<<IEN1_T1IE); //定时器3中断使能
  g_cTimer1_Mode = 0;
}


/*******************************************************************************
* @fn          Timer1_process
*
* @brief      设置定时器1捕捉模式
*
* @param       none
*
* @return      none
******************************************************************************/
static void    timer_capture_setting(void)
{
  T1CNTH  = 0;
  T1CNTL  = 0;
  T1CCTL0 = 0;
  IRCON  &= ~(1<<IRCON_T1IF);         //清定时器T1中断请求
  T1STAT  = ~((1<<CH3IF)|(1<<OVFIF)); //清定时器T1通道3中断请求
  T1CCTL3 = Tx_CAP11;                 //使能通道3中断，所有沿捕获
  T1CTL   = ((T1DIV_32<<2)|(T1MODE_FREE));
  TIMIF  |= (1<<TMF_OVFIM);   //溢出中断，用于解码的延时计数
  T1CCTL3 |= (1<<Tx_IM);      //启动定时器捕捉
  IEN1    |= (1<<IEN1_T1IE);  //开始启动定时器1通道3捕获
  HAL_ENABLE_INTERRUPTS();
}


/*******************************************************************************
* @fn       void  ir_matchup_init(void)
*
* @brief   初始化红外识别
*
* @param   void
*
* @return   void
*******************************************************************************/
void  ir_matchup_init(void)
{
  IR_RECEIVE_PORT_SET();
  CARRIERWAVE_PORT_INIT();
  ircode_capture_init();
  timer_capture_setting();
}


/*******************************************************************************
 * @fn      MED_ProcIRStudyCmd
 *
 * @brief   处理0x0303指令
 *
 * @param   uint8 airType
 *
 * @return  none
*******************************************************************************/
#ifdef   MATCH_BRAND
void MED_ProcIRStudyCmd(uint8 airType);
void MED_ProcIRStudyCmd(uint8 airType)
{
  capture_temp.matchup_source = true;
  capture_temp.cIRDevType = airType;
}
#endif


/*******************************************************************************
* @fn         void analyse_ircode_matchup(void)
*
* @brief       将脉宽数据里面的数据区数据提取出来，
* @param    none
*
* @return      none
**************************************************/
bool  ir_KeyPress = false;
static void analyse_ircode_matchup(void)
{
  uint16  mm        = 1;
  uint8   n_one     = 0;
  uint8   n_zero    = 0;
  uint16  k         = 0;
  uint16  pos_local = 0;  
  ir_matchup_result.matchup_result = true;
  osal_memset(pTempLargeBuf, 0, sizeof(pTempLargeBuf));
  n_one = ((capture_temp.iLogic1LTime / capture_temp.iLogic1HTime) * 10 +     //得到比例值（比如2.7），为了便于比较，乘以10倍（等于27）。
           (((capture_temp.iLogic1LTime % capture_temp.iLogic1HTime) * 10) / capture_temp.iLogic1HTime));
  n_zero  = ((capture_temp.iLogic0LTime / capture_temp.iLogic0HTime) * 10 +    
             (((capture_temp.iLogic0LTime % capture_temp.iLogic0HTime) * 10) / capture_temp.iLogic0HTime));
  while(mm < capture_temp.count_last)
  {                                                                   //数据区数据提取
    k = ((pRecvAndSendPulseBuf[mm] / capture_temp.iLogic1HTime) * 10 + 
         (((pRecvAndSendPulseBuf[mm] % capture_temp.iLogic1HTime) * 10)/capture_temp.iLogic1HTime));
    if(sub_abs(pRecvAndSendPulseBuf[mm-1],capture_temp.iLogic1HTime)> ir_lead[dev_mark][3])
    {                                                                       //滤掉前导码脉冲。当前脉冲前面的脉冲必须是pIRLearnProc.iLogic1HTime
     asm("NOP");
    }else  
    if(sub_abs(k,n_one) < 6)  //6                                          //比例乘以10倍，比较 
    {                                                                                                        
      *(pTempLargeBuf+(pos_local >>3)) |=  (0x01 << (7-(pos_local & 7))); //将数组第pos_local位数据置位
      pos_local++;
    }
    else if(sub_abs(k,n_zero)<6)   //6
    {
      pos_local++;
    }
    mm += 2;
  }
  asm("NOP");
#ifdef   MATCH_BRAND  
  if(capture_temp.matchup_source)//
  {
    capture_temp.matchup_source = 0;
    capture_temp.IRCodeRule = 0;
    mm =  pRecvAndSendPulseBuf[0];
    pRecvAndSendPulseBuf[6] = 0;
    pRecvAndSendPulseBuf[6]|=(mm << 8);
    pRecvAndSendPulseBuf[7] = 0;
    pRecvAndSendPulseBuf[7] |= ( mm >> 8);  
    mm =  pRecvAndSendPulseBuf[1];  
    pRecvAndSendPulseBuf[7]|=((mm <<8) );
    pRecvAndSendPulseBuf[8] = 0;
    pRecvAndSendPulseBuf[8] |=((mm >>8));
    pRecvAndSendPulseBuf[0] = 0;
    pRecvAndSendPulseBuf[0] |= (uint16)capture_temp.cIRDevType<<8;
    pRecvAndSendPulseBuf[0] |= (uint16)ir_matchup_result.matchup_result; 
    pRecvAndSendPulseBuf[1] = 0;
    pRecvAndSendPulseBuf[1] |= (capture_temp.count_last << 8); 
    pRecvAndSendPulseBuf[1] |= ((uint16)capture_temp.IRCodeRule & 0x00ff);
    pRecvAndSendPulseBuf[2] = 0;
    pRecvAndSendPulseBuf[2] |=  (capture_temp.iLogic1HTime << 8);
    pRecvAndSendPulseBuf[2] |=  (capture_temp.count_last >> 8);
    pRecvAndSendPulseBuf[3] = 0;
    pRecvAndSendPulseBuf[3] |= (capture_temp.iLogic1LTime << 8);
    pRecvAndSendPulseBuf[3] |= (capture_temp.iLogic1HTime >> 8);
    pRecvAndSendPulseBuf[4] = 0;
    pRecvAndSendPulseBuf[4] |= (capture_temp.iLogic0HTime << 8);
    pRecvAndSendPulseBuf[4] |= (capture_temp.iLogic1LTime >> 8);   
    pRecvAndSendPulseBuf[5] = 0;
    pRecvAndSendPulseBuf[5] |= (capture_temp.iLogic0LTime <<8);
    pRecvAndSendPulseBuf[5] |= (capture_temp.iLogic0HTime >>8);
    pRecvAndSendPulseBuf[6] |= (capture_temp.iLogic0LTime >>8);
    pRecvAndSendPulseBuf[8] |= ((uint16)pTempLargeBuf[0]  <<8);
    pRecvAndSendPulseBuf[9] = 0;
    pRecvAndSendPulseBuf[9]|=((uint16)pTempLargeBuf[1]);
    //数据上传
    MEDNotifyIRStudyResult(MATCHUP_UPLOAD_LENGTH);
  }else
#endif
  {
     pmatchup[dev_mark](pos_local);
     if(ir_matchup_result.matchup_result == true)
     {
        if(ir_matchup_result.sysMode == 0)
        {
          ir_matchup_result.sysMode = SYS_AUTO;
        }
        if(ir_matchup_result.curSetTemp<18) 
        {
          ir_matchup_result.curSetTemp = 18;
        }
        if(ir_matchup_result.curSetTemp>30)
        {
          ir_matchup_result.curSetTemp = 30;
        }
        ir_KeyPress = true;
        if(ir_KeyPress)
        {
           osal_start_timerEx(MEDTaskID, MED_IR_MATCHUP, 200);
           ir_KeyPress = false;
        }
     }
  }
}

/*******************************************************************************
* @fn          med_irlearn_process
*
* @brief       红外编码自学习程序处理
*
* @param       none
*
* @return      none
********************************************************************************/
void MED_IR_Receive_Process(void)
{
  if(g_cTimer1_Mode != IRSEND_MODE)
  {                                       //最后一次捕获后1秒后没有新脉冲，就开始进入识别程序。
    if( capture_temp.count_last>10)
    {
      Timer1_CaptureOff();
      get_ir_length();                    //获取红外脉冲中逻辑数据的长度
      analyse_ircode_matchup();           //识别
    }
    ircode_capture_init();
    timer_capture_setting();
  }
  else
  {
    osal_start_timerEx(MEDTaskID, MED_STUDY_EVT,TIMER_ONE_SECOND);
  }
}
/*******************************************************************************
* @fn          HAL_ISR_FUNCTION
*
* @brief       定时器1通道3捕获处理;`通道0发射处理
*
* @param       none
*
* @return      none

*******************************************************************************/
HAL_ISR_FUNCTION(halTimer1Isr,T1_VECTOR)
{
  uint8 time_count_temp = 15;
  /********************进入红外捕获******************************/
  if( T1STAT & (1<<CH3IF))
  {   
    T1CNTH  = 0;
    T1CNTL  = 0;                                         
    capture_temp.capture_char[1]   = T1CC3H;
    capture_temp.capture_char[0]   = T1CC3L;
    if (capture_temp.count> (IR_MAX_PULSE-2))//改进，防止数组越界。
    {
      capture_temp.count= (IR_MAX_PULSE-2);
    }
    if (capture_temp.matchup_source || ((0 == capture_temp.count) && ((((ir_lead[dev_mark][0] > capture_temp.capture_int)?(ir_lead[dev_mark][0] - capture_temp.capture_int):(capture_temp.capture_int - ir_lead[dev_mark][0])) < ir_lead[dev_mark][3])||
                                       (((ir_lead[dev_mark][4] > capture_temp.capture_int)?(ir_lead[dev_mark][4] - capture_temp.capture_int):(capture_temp.capture_int - ir_lead[dev_mark][4])) < ir_lead[dev_mark][7])||
                                       (((ir_lead[dev_mark][8] > capture_temp.capture_int)?(ir_lead[dev_mark][8] - capture_temp.capture_int):(capture_temp.capture_int - ir_lead[dev_mark][8])) < ir_lead[dev_mark][11]))))
    {                                                       //判断引导码，若符合，立即更新计数器
      capture_temp.matchup_state = true;//OK，继续捕获  实际上有不符合脉宽的进入
      capture_temp.time_flag  = true;
    }
    
    if((3 == capture_temp.count) && (!capture_temp.matchup_source))//二次滤波    
    {
      if(((((ir_lead[dev_mark][0]>pRecvAndSendPulseBuf[0])?(ir_lead[dev_mark][0] - pRecvAndSendPulseBuf[0]):(pRecvAndSendPulseBuf[0] -ir_lead[dev_mark][0])) < ir_lead[dev_mark][3])&&
         (((ir_lead[dev_mark][1]>pRecvAndSendPulseBuf[1])?(ir_lead[dev_mark][1] - pRecvAndSendPulseBuf[1]):(pRecvAndSendPulseBuf[1] -ir_lead[dev_mark][1])) < ir_lead[dev_mark][3])&&
         ((ir_lead[dev_mark][2]>pRecvAndSendPulseBuf[2])?(ir_lead[dev_mark][2] - pRecvAndSendPulseBuf[2]):(pRecvAndSendPulseBuf[2] -ir_lead[dev_mark][2])) < (ir_lead[dev_mark][3]>>1))||
         ((((ir_lead[dev_mark][4]>pRecvAndSendPulseBuf[0])?(ir_lead[dev_mark][4] - pRecvAndSendPulseBuf[0]):(pRecvAndSendPulseBuf[0] -ir_lead[dev_mark][4])) < ir_lead[dev_mark][7])&&
         (((ir_lead[dev_mark][5]>pRecvAndSendPulseBuf[1])?(ir_lead[dev_mark][5] - pRecvAndSendPulseBuf[1]):(pRecvAndSendPulseBuf[1] -ir_lead[dev_mark][5])) < ir_lead[dev_mark][7])&&
         ((ir_lead[dev_mark][6]>pRecvAndSendPulseBuf[2])?(ir_lead[dev_mark][6] - pRecvAndSendPulseBuf[2]):(pRecvAndSendPulseBuf[2] -ir_lead[dev_mark][6])) < (ir_lead[dev_mark][7]>>1))||
         ((((ir_lead[dev_mark][8]>pRecvAndSendPulseBuf[0])?(ir_lead[dev_mark][8] - pRecvAndSendPulseBuf[0]):(pRecvAndSendPulseBuf[0] -ir_lead[dev_mark][8])) < ir_lead[dev_mark][11])&&
         (((ir_lead[dev_mark][9]>pRecvAndSendPulseBuf[1])?(ir_lead[dev_mark][9] - pRecvAndSendPulseBuf[1]):(pRecvAndSendPulseBuf[1] -ir_lead[dev_mark][9])) < ir_lead[dev_mark][11])&&
         ((ir_lead[dev_mark][10]>pRecvAndSendPulseBuf[2])?(ir_lead[dev_mark][10] - pRecvAndSendPulseBuf[2]):(pRecvAndSendPulseBuf[2] -ir_lead[dev_mark][10])) < (ir_lead[dev_mark][11]>>1)))
      {
        asm("NOP");
        capture_temp.time_count = 0;
      }else{
        pRecvAndSendPulseBuf[0]=pRecvAndSendPulseBuf[1];
        pRecvAndSendPulseBuf[1]=pRecvAndSendPulseBuf[2];
        capture_temp.count = 2;
      }
    }
    if(capture_temp.matchup_state || capture_temp.matchup_source)
    {
      pRecvAndSendPulseBuf[capture_temp.count] = capture_temp.capture_int;
      capture_temp.count++;
      if((DAIKIN == dev_mark) && (DAIKIN_MARK < capture_temp.capture_int))//大金空调专用，滤除过多脉冲
      {
        capture_temp.count = 0; 
      }
    }
  }
  /***************************CH3  红外捕获部分*************************/
  if( T1STAT & (1<<OVFIF))
  {
    if(capture_temp.time_flag)
    {
      capture_temp.time_count++; 
      if(capture_temp.count > 3)            // >3时赋值，避免干扰脉冲。 
      {
        capture_temp.count_last =  capture_temp.count;
      }
      capture_temp.count= 0;
      capture_temp.matchup_state = 0;
      if(capture_temp.time_count>time_count_temp)
      { 
        osal_start_timerEx(MEDTaskID, MED_STUDY_EVT,1);
      }
    }
    if(capture_temp.matchup_source)//延时退出功能
    {
      capture_temp.ovfif_times++;
      if(capture_temp.ovfif_times > 228)//15秒自动退出型号识别
      {
        capture_temp.matchup_source = 0;
        capture_temp.ovfif_times = 0;
      }
    }
  }
  
  /***************************CH0 红外发射部分****************************/
  if( T1STAT & (1<<CH0IF))
  {     
    /**********************发射 过程********************/
    if(PIrSend.iIRSendInd < PIrSend.iIRSendPulseCount)//继续发射,切换电平，赋值
    {                                       
      Carrierwave_port = !Carrierwave_port;//取反值
      T1CC0H =  (pRecvAndSendPulseBuf[PIrSend.iIRSendInd]>>8);
      T1CC0L =  pRecvAndSendPulseBuf[PIrSend.iIRSendInd++];
    }
    /******************多段码延时过程***********************/
    if ((PIrSend.iLowPulseLen > 0) && (0 == PIrSend.iIRSendPulseCount)) //低脉冲大于零，就有第二段未发射
    {                                                                   //发射多段码，注意，iLowPulseLen单位是ms
      PIrSend.iLowPulseLen--;                                           //毫秒，即中断次数，一次间隔1ms，1000us。
      if(!PIrSend.iLowPulseLen)
      {                                                                 //从待发送区取数据  
        /********************多段码延时完毕，取第二段数据*********************/        
        Carrierwave_port = true;                                        //高,不射 
        PIrSend.bIsNeedSecondSend = 0;
        PIrSend.iIRSendPulseCount = PIrSend.iNextIRPulseCount;
        MED_GetIRPulseDataInFlash(IR_MAX_PULSE * 2);
        PIrSend.iIRSendInd = 0;
        pRecvAndSendPulseBuf[PIrSend.iIRSendInd]-=20;                   //结合实际二次调整    
        T1CC0H = (pRecvAndSendPulseBuf[PIrSend.iIRSendInd]>>8);
        T1CC0L =  pRecvAndSendPulseBuf[PIrSend.iIRSendInd++];  
       }
    } 
    /******************进入多段码判断***********************/
    if(PIrSend.bIsNeedSecondSend && (PIrSend.iIRSendInd == PIrSend.iIRSendPulseCount))
    {                                                //多段码时一次性设置
      T1CC0H = 0x03; 
      T1CC0L = 0xD9;                                 //小于 0x03e8,  == 1000us,==1ms
      PIrSend.iIRSendPulseCount = 0;                 //多重作用       
      Carrierwave_port = true;                       //高电平   
    }
    /********************全部发射完毕**********************/
    if((!PIrSend.bIsNeedSecondSend  && (PIrSend.iIRSendInd == PIrSend.iIRSendPulseCount)) || 
        (!PIrSend.bIsNeedSecondSend && (PIrSend.iIRSendPulseCount == 0)) ||
        (PIrSend.iIRSendInd > IR_MAX_PULSE) )                         //增加异常退出判断
    { 
      Carrierwave_port = true;
#ifdef IR_MATCH_UP
      ir_matchup_init();//发射完毕，立即初始化为匹配红外码状态。
#endif
      IRsend_Shutdown();
    }
  }
  T1STAT  = ~((1<<CH3IF)|(1<<OVFIF)|(1<<CH0IF)); //清定时器T1通道3中断请求
}
/*********************************END***************************************/






