/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Filename:       MED_IRSEND.H
Revised:        $Date: 2011-12-28 12:53:18 -0800 (Wed, 28 Dec 2011) $
Revision:       $Revision: 21388 $

Description:    This module implements the MED functionality and contains the
                init and event loop functions

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef MED_IRSEND_H
#define MED_IRSEND_H

#ifdef __cplusplus
extern "C"
{
#endif


/*========================================================================
 * INCLUDES
========================================================================*/
#include "MED.h"

/*========================================================================
 * CONSTANTS
========================================================================*/
//IEN1
#define IEN1_P0IE     5       //�˿�0�ж�ʹ�ܱ�־��0-�жϽ�ֹ��1-�ж�ʹ��
#define IEN1_T4IE     4       //��ʱ��4�ж�ʹ�ܱ�־��0-�жϽ�ֹ��1-�ж�ʹ��
#define IEN1_T3IE     3       //��ʱ��3�ж�ʹ�ܱ�־��0-�жϽ�ֹ��1-�ж�ʹ��
#define IEN1_T2IE     2       //��ʱ��2�ж�ʹ�ܱ�־��0-�жϽ�ֹ��1-�ж�ʹ��
#define IEN1_T1IE     1       //��ʱ��1�ж�ʹ�ܱ�־��0-�жϽ�ֹ��1-�ж�ʹ��
#define IEN1_DMAIE    0       //DMA�����ж�ʹ�ܱ�־��0-�жϽ�ֹ��1-�ж�ʹ��
//TIMIF
#define TMF_OVFIM     6       //��ʱ��1����ж����α�־��0-�ж�ʹ�ܣ�1-�жϽ�ֹ
#define TMF_T4CH1IF   5       //��ʱ��4ͨ��1�жϱ�־��0-���ж�δ����1-�ж�δ��
#define TMF_T4CH0IF   4       //��ʱ��4ͨ��0�жϱ�־��0-���ж�δ����1-�ж�δ��
#define TMF_T4OVFIF   3       //��ʱ��4����жϱ�־��0-���ж�δ����1-�ж�δ��
#define TMF_T3CH1IF   2       //��ʱ��3ͨ��1�жϱ�־��0-���ж�δ����1-�ж�δ��
#define TMF_T3CH0IF   1       //��ʱ��3ͨ��0�жϱ�־��0-���ж�δ����1-�ж�δ��
#define TMF_T3OVFIF   0       //��ʱ��3����жϱ�־��0-���ж�δ����1-�ж�δ��
//IRCON
#define IRCON_STIF    7       //˯�߶�ʱ���жϱ�־��0-���ж�δ����1-�ж�δ��
#define IRCON_P0IF    5       //�˿�0�жϱ�־��0-���ж�δ����1-�ж�δ��
#define IRCON_T4IF    4       //��ʱ��4�жϱ�־��0-���ж�δ����1-�ж�δ��
#define IRCON_T3IF    3       //��ʱ��3�жϱ�־��0-���ж�δ����1-�ж�δ��
#define IRCON_T2IF    2       //��ʱ��2�жϱ�־��0-���ж�δ����1-�ж�δ��
#define IRCON_T1IF    1       //��ʱ��1�жϱ�־��0-���ж�δ����1-�ж�δ��
#define IRCON_DMAIF   0       //DMA����жϱ�־��0-���ж�δ����1-�ж�δ��

/*========================================================================
 * MACROS
========================================================================*/
#define INT_GLOBAL_ENABLE(on) EA=(!!on)

#define INUM_RFERR 0
#define INUM_ADC   1
#define INUM_URX0  2
#define INUM_URX1  3
#define INUM_ENC   4
#define INUM_ST    5
#define INUM_P2INT 6
#define INUM_UTX0  7
#define INUM_DMA   8
#define INUM_T1    9
#define INUM_T2    10
#define INUM_T3    11
#define INUM_T4    12
#define INUM_P0INT 13
#define INUM_UTX1  14
#define INUM_P1INT 15
#define INUM_RF    16
#define INUM_WDT   17


#define IO_FUNC_GIO     0 // General purpose I/O
#define IO_FUNC_PERIPH  1 // Peripheral function
#define Carrierwave_port  P1_2
// Macros for configuring IO direction:
// Example usage:
//   IO_DIR_PORT_PIN(0, 3, IO_IN);    // Set P0_3 to input
//   IO_DIR_PORT_PIN(2, 1, IO_OUT);   // Set P2_1 to output

#define IO_DIR_PORT_PIN(port, pin, dir) \
  do                                    \
  {                                     \
    if (dir == IO_OUT)                  \
      P##port##DIR |= (0x01<<(pin));    \
    else                                \
      P##port##DIR &= ~(0x01<<(pin));   \
  }while(0)
// Where port={0,1,2}, pin={0,..,7} and dir is one of:
#define IO_IN   0
#define IO_OUT  1

// Macros for function select (General purpose I/O / Peripheral function):
// Example usage:
//   IO_FUNC_PORT0_PIN0(0, 0, IO_FUNC_PERIPH);
//   IO_FUNC_PORT0_PIN1(0, 1, IO_FUNC_GIO);
//   IO_FUNC_PORT2_PIN3(2, 3, IO_FUNC_PERIPH);

#define IO_FUNC_PORT_PIN(port, pin, func) \
  do                                      \
  {                                       \
    if((port == 2) && (pin == 3))         \
    {                                     \
      if (func)                           \
      {                                   \
        P2SEL |= 0x02;                    \
      }                                   \
      else                                \
      {                                   \
        P2SEL &= ~0x02;                   \
      }                                   \
    }                                     \
    else if((port == 2) && (pin == 4))    \
    {                                     \
      if (func)                           \
      {                                   \
        P2SEL |= 0x04;                    \
      }                                   \
      else                                \
      {                                   \
        P2SEL &= ~0x04;                   \
      }                                   \
    }                                     \
    else                                  \
    {                                     \
      if (func)                           \
      {                                   \
        P##port##SEL |= (0x01<<(pin));    \
      }                                   \
      else                                \
      {                                   \
        P##port##SEL &= ~(0x01<<(pin));   \
      }                                   \
    }                                     \
  } while (0)
  
  
#define CARRIERWAVE_PORT_INIT() st(IO_FUNC_PORT_PIN(1,2,IO_FUNC_GIO);   \
                                   IO_DIR_PORT_PIN(1,2,IO_OUT);         \
                                   Carrierwave_port = true;)
/*========================================================================
 * TYPEDEFS
========================================================================*/

/*========================================================================
 * VARIABLES
========================================================================*/
typedef struct
{
  uint8   iIRSendErrorType;
  uint16  iIRSendPulseCount;          //��ǰ��Ҫ���͵ĺ����봮����
  uint16  iIRSendInd;
#if defined(IR_MULT_CODE)
  uint16  iLowPulseLen;               //������м������ʱ��ms
  uint16  iNextIRPulseCount;          //��һ�ν�Ҫ���͵��봮����
  bool    bIsNeedSecondSend;          //�Ƿ�ڶ��η���
#endif
}IRsend_Parameter_t;

extern IRsend_Parameter_t PIrSend;
extern uint8   g_cTimer1_Mode;
/*========================================================================
 * FUNCTIONS
========================================================================*/
void   MED_IRsend_Process(void);
void   MEDIRSendInit(void);
bool   IRsend_Convert_Pulse(uint8 *pSend_data, uint8 data_len);
void   IRsend_Shutdown(void);
uint8  GetIRsend_Array_Len(uint8 *pcompdata, uint8 byte_number);
uint16 get_ir_narrow_length(uint16  *irdata);
uint16 get_ir_big_length(uint16  *irdata);
#ifdef IR_MATCH_UP
extern void  Timer1_CaptureOff(void);
#endif
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif




#endif /* MED_IRSEND_H */
