/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Filename:       MED_IRsend.c
Revised:        $Date: 2011-12-28 12:53:18 -0800 (Wed, 28 Dec 2011) $
Revision:       $Revision: 21388 $

Description:    This module implements the MED functionality and contains the
init and event loop functions

contact MYENERGYDOMAIN Incorporated at www.myenergydomain.com.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*========================================================================
* INCLUDES
========================================================================*/
#include "MED_IRSEND.h"
#include "MED_matchup.h"
#include "MED_NV.h"
/*========================================================================
* MACROS
========================================================================*/

/*========================================================================
* CONSTANTS
========================================================================*/
#define IRCODE_PULSENUM_ADDR 0
#define IRCODE_LOGIC1H_ADDR  2
#define IRCODE_LOGIC1L_ADDR  4
#define IRCODE_LOGIC0H_ADDR  6
#define IRCODE_LOGIC0L_ADDR  8
#define IRCODE_CODE_ADDR     10
/*========================================================================
* TYPEDEFS
========================================================================*/



/*========================================================================
* GLOBAL VARIABLES
========================================================================*/
IRsend_Parameter_t  PIrSend;
/*========================================================================
* LOCAL FUNCTIONS
========================================================================*/

/*******************************************************************************
* @fn          MEDIRSendInit
*
* @brief       ��ʼ�����ⷢ��
*
* @param       none
*
* @return      none
********************************************************************************/
void MEDIRSendInit(void)
{
  osal_memset(&PIrSend, 0x00, sizeof(IRsend_Parameter_t));
  osal_memset(pRecvAndSendPulseBuf, 0x00, sizeof(pRecvAndSendPulseBuf));
  Carrierwave_port = true;
}


/************************************************
* @fn          IRsend_Process
*
* @brief       ���������룬��ʱ����ʽ
*
* @param       none
*
* @return      none
************************ ******************/
static void  IRsend_Process(void)
{
  PIrSend.iIRSendPulseCount += 1;
  if( PIrSend.iIRSendPulseCount<10)
  {
    return;
  }
#ifdef IR_MATCH_UP  
  Timer1_CaptureOff();
#endif
  tSysCfgInRam.iIRSendCount++;
  g_cTimer1_Mode  = IRSEND_MODE;
  HAL_ENABLE_INTERRUPTS();  
  T1CNTH  = 0;
  T1CNTL  = 0;
  IRCON   &= ~(1<<IRCON_T1IF); //�嶨ʱ��T1�ж�����
  T1STAT  &= 0xE0;
  T1CTL  |= ((T1DIV_32<<2)|(T1MODE_MODEL));
  T1CCTL0 |=((1<< Tx_IM) | (1 << Tx_MODE));//ʹ��ͨ��0�Ƚ�ģʽ��ע�⣡�������޹أ���ʹ������жϣ�
  TIMIF  &= ~(1<<TMF_OVFIM);   //����жϣ��ر�
  PIrSend.iIRSendInd = 0;  
  T1CC0H = (pRecvAndSendPulseBuf[PIrSend.iIRSendInd]>>8);
  T1CC0L =  pRecvAndSendPulseBuf[PIrSend.iIRSendInd++];
  IEN1 |= (1<<IEN1_T1IE); //��ʼ������ʱ��1          
  Carrierwave_port = true;                 //��,����  
  if (PIrSend.iLowPulseLen>0)    
  {                                    //�Ƿ��Ƕ����ķ���
    PIrSend.bIsNeedSecondSend = 1;
  }else {
    PIrSend.bIsNeedSecondSend = 0;
  }
}


/********************************************************************************
* @fn          MED_IRsend_Process
*
* @brief       ���ⷢ�䴦��
*
* @param       none
*
* @return      none
*******************************************************************************/
void  MED_IRsend_Process(void)
{
  CARRIERWAVE_PORT_INIT();
  IRsend_Process();	
}


/*******************************************************************************
* @fn          Lshift_Bit
*
* @brief       λ����
*
* @param       logic_bit , *pshift
*
* @return      none
*******************************************************************************/
static void  Lshift_Bit(uint8 logic_bit, uint8 *pshift, uint16 data_len)
{
  uint16 j;
  
  for (j=0; j<(data_len-1); ++j)
  {
    *(pshift+j) = ((*(pshift+j)) << 2) | (((*(pshift+j+1)) & 0xC0) >> 6);
  }
  *(pshift+data_len-1)  = ((*(pshift+data_len-1))<<2);
  *(pshift+data_len-1) |= logic_bit;
}


/*************************************************************************
* @fn          Lshift_Byte
*
* @brief       �ֽ�����
*
* @param       logic_byte
*              *pshift
*
* @return      none
************************************************************************/
static void  Lshift_Byte(uint16 logic_byte, uint8 *pshift, uint16 data_len)
{
  uint16 j=0;
  
  while (j<(data_len-1))
  {
    *(pshift+j)   = *(pshift+j+2);
    *(pshift+j+1) = *(pshift+j+3);
    j += 2;
  }
  *(pshift+data_len-2) = LO_UINT16(logic_byte);
  *(pshift+data_len-1) = HI_UINT16(logic_byte);
}


/*******************************************************************************
* @fn          IRsend_Convert_Pulse
*
* @brief       ������ⷢ������ʱ��
*
* @param       *pIRcode
*
* @return      none
**********************************************************************************/
bool  IRsend_Convert_Pulse(uint8 *pSend_data, uint8 data_len)
{
  uint16  jj = 0;
  PIrSend.iIRSendPulseCount = BUILD_UINT16((*(pSend_data+IRCODE_PULSENUM_ADDR)),
                                           (*(pSend_data+IRCODE_PULSENUM_ADDR+1))) + 1;

  if (PIrSend.iIRSendPulseCount <= 0)
  {
    PIrSend.iIRSendErrorType = true;
    return false;
  }

  if (PIrSend.iIRSendPulseCount > IR_MAX_PULSE)
  {
    PIrSend.iIRSendErrorType = true;
    PIrSend.iIRSendPulseCount = 0;
    return false;
  }

  while (jj < PIrSend.iIRSendPulseCount)
  {
    if ((((*(pSend_data+IRCODE_CODE_ADDR))&0xC0) == 0x80) ||
        (((*(pSend_data+IRCODE_CODE_ADDR))&0xC0) == 0xC0))
    {//���߼����ݱ���
      Lshift_Bit(0x00, pSend_data+IRCODE_CODE_ADDR, data_len);
      pRecvAndSendPulseBuf[jj] = BUILD_UINT16((*(pSend_data+IRCODE_CODE_ADDR)),
                                              (*(pSend_data+IRCODE_CODE_ADDR+1)));
      Lshift_Byte(0x0000, pSend_data+IRCODE_CODE_ADDR, data_len);
    }
    else if (((*(pSend_data+IRCODE_CODE_ADDR))&0xC0) == 0x40)
    {//�߼�����1
      pRecvAndSendPulseBuf[jj] = BUILD_UINT16((*(pSend_data+IRCODE_LOGIC1H_ADDR)),
                                              (*(pSend_data+IRCODE_LOGIC1H_ADDR+1)));
      ++jj;
      pRecvAndSendPulseBuf[jj] = BUILD_UINT16((*(pSend_data+IRCODE_LOGIC1L_ADDR)),
                                              (*(pSend_data+IRCODE_LOGIC1L_ADDR+1)));
      Lshift_Bit(0x00, pSend_data+IRCODE_CODE_ADDR, data_len);
    }
    else
    {//�߼�����0
      pRecvAndSendPulseBuf[jj] = BUILD_UINT16((*(pSend_data+IRCODE_LOGIC0H_ADDR)),
                                              (*(pSend_data+IRCODE_LOGIC0H_ADDR+1)));
      ++jj;
      pRecvAndSendPulseBuf[jj] = BUILD_UINT16((*(pSend_data+IRCODE_LOGIC0L_ADDR)),
                                              (*(pSend_data+IRCODE_LOGIC0L_ADDR+1)));
      Lshift_Bit(0x00, pSend_data+IRCODE_CODE_ADDR, data_len);
    }
    ++jj;
  }
  
  //�ظ��ȴ���
  AuxHeatControl(false, System_ConfigInface.iAirBrand);
  return true;
}


/*******************************************************************************
* @fn          GetIRsend_Array_Len
*
* @brief       Get the array number
*
* @param       *pcompdata ,  byte_number
*
* @return      none
********************************************************************************/
uint8 GetIRsend_Array_Len(uint8 *pcompdata, uint8 byte_number)
{
  uint8 i,len;

  len  = 0;
  for (i = 0; i < byte_number; i++)
  {
    if ((*(pcompdata+i)) != 0)
    {
      len = i+1;
    }
  }
  return len;
}

/*******************************************************************************
* @fn         IRsend_Shutdown
*
* @brief      �رն�ʱ��3
*
* @param      none
*
* @return     none
*******************************************************************************/
void  IRsend_Shutdown(void)
{
  MEDIRSendInit();
  g_cTimer1_Mode = 0;
}


/*************************************************
* @fn          get_ir_big_length
*
* @brief       ��ȡ1,0��������ȽϿ������ֵ
*
* @param       uint8  *irdata
*
* @return      uint8
***********************************************/
uint16  get_ir_big_length(uint16  *irdata)
{
  uint8  m = 0;
  for(m = 0; m<20; m+=2)
  {
    if(((((*(irdata + m)) / (*(irdata + m + 2))) > 1) &&
       (((*(irdata + m)) / (*(irdata + m + 2))) < 5)) || //��������1,��С��5
       ((((*(irdata + m)) / (*(irdata + m + 2))) == 1) && //��������0����������̫С
       (((*(irdata + m)) % (*(irdata + m + 2))) > (*(irdata + m + 2) / 2))))//����ж���ؼ���
    {
      break;
    }
  }
  return  *(irdata + m );
}


/*************************************************
* @fn          get_ir_narrow_length
*
* @brief       ��ȡ1,0��������Ƚ�խ������ֵ
*
* @param       uint8  *irdata
*
* @return      uint8
***********************************************/
uint16  get_ir_narrow_length(uint16  *irdata)
{
  uint8   m = 0;
  for(m = 0; m<20; m+=2)
  {
    if(((((*(irdata + m)) / (*(irdata + m + 2))) > 1) &&
       (((*(irdata + m)) / (*(irdata + m + 2))) < 5)) || //��������1,��С��5
       (((*(irdata + m) / *(irdata + m + 2)) == 1) && //��������0����������̫С
       (((*(irdata + m)) % (*(irdata + m + 2))) > (*(irdata + m + 2) / 2))))//����ж���ؼ���
    {
      break;
    }
  }
  return  *(irdata + m + 2);
}



/*******************************************************************************
* @fn          AuxHeatControl
*
* @brief       ��/�ظ��ȡ�
*
* @param       bool bAuxStatus,uint8 AirBrand
*
* @return      none
*******************************************************************************/
void AuxHeatControl(bool bAuxStatus,uint8 AirBrand)
{
  if ((System_ConfigInface.SystemIrda.sysMode == SYS_HEAT) || 
      (System_ConfigInface.SystemIrda.sysMode == SYS_AUTO))
  {
    if(AirBrand == GREE)
    {
      //�ظ��ȿ���
      if(PIrSend.iIRSendPulseCount>=250)
      {
        pRecvAndSendPulseBuf[GREE_AUX_POS2] = get_ir_big_length(pRecvAndSendPulseBuf + 5);
      }
      pRecvAndSendPulseBuf[GREE_AUX_POS] = get_ir_big_length(pRecvAndSendPulseBuf + 5);
    }
    else if (AirBrand == HISENSE)
    {
      pRecvAndSendPulseBuf[HISENSE_AUX_POS] = get_ir_narrow_length(pRecvAndSendPulseBuf + 5);
      pRecvAndSendPulseBuf[HISENSE_AUX_POS2] = get_ir_big_length(pRecvAndSendPulseBuf + 5);
    }
  }
}


/****************end of MED_IRSend.c*****************************************/
