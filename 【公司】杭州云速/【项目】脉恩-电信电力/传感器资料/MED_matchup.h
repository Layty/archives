#ifndef IR_MATCHUP_H
#define IR_MATCHUP_H
#ifdef __cplusplus
extern "C"
{
#endif
/*********************************************************************
 * INCLUDES
 */
#include "hal_board.h"
#include "MED.h"
/*********************************************************************
 * MACROS
 */
#define     ONOff_POS      0
#define     SYSMODE_POS    1
#define     WINDLEVEL_POS  2       //�Բ��ϣ�Ҫת��һ��
#define     TEMP_POS       3
#define     OPPOSITE       2       //���ػ���ȡ����
#define     DAIKIN_MARK    20000   //����������еĳ�������ֵ

#define     ONOff_SCOPE      4
#define     SYSMODE_SCOPE    5
#define     WINDLEVEL_SCOPE  6
#define     TEMP_SCOPE       7
#define     MATCHUP_UPLOAD_LENGTH   19
  
//#define     TRANSFER       21         //ת�������38k�ز�  
#define     DATA_ERROR               3     //ʶ��������������ݴ���
#define     LENGTH_ERROR             2     //ʶ����������������
#define     GREE_PULSE_NUM           65    //��������������Ч����
#define     MIDEA_PULSE_NUM_LONGER   140   //  ����������Ч����
#define     MIDEA_PULSE_NUM_SHORTER  90    //  ����������Ч����
#define     CHIGO_PULSE_NUM          90    //  ����������Ч����
#define     AUX_PULSE_NUM            100   //  ����������Ч����
#define     HISENSE_PULSE_NUM        130   //  ����������Ч����
#define     HAIER_PULSE_NUM          105   //  ����������Ч����
#define     AUCMA_PULSE_NUM          92    //  ����������Ч����
#define     SHINCO_PULSE_NUM         38    //  ����������Ч����
#define     MITSUBISHI_PULSE_NUM     45    //  ����������Ч����
#define     MITSUBISHI_PULSE_NUM1    87    //  ����������Ч����
#define     YAIR_PULSE_NUM           110    //  ����������Ч����
#define     HUALING_PULSE_NUM        110   //  ����������Ч����
#define     KELON_PULSE_NUM          110   //  ����������Ч����
#define     CHUNLAN_PULSE_NUM        70   //  ����������Ч����
#define     TCL_PULSE_NUM            100   //  ����������Ч���� 
#define     DAIKIN_PULSE_NUM         120   //  ����������Ч���� 
#define     CHANGHONG_PULSE_NUM      115   //  ����������Ч���� CHANGHONG
#define     TRANSFER_PULSE_NUM       10
#define     GALANZ_PULSE_NUM         110   //  ����������Ч���� 
#define     HITACHI_PULSE_NUM        260   //  ����������Ч����
#define     PANASONIC_PULSE_NUM      215   //  ����������Ч����
#define     LG_PULSE_NUM             26    //  ����������Ч����  
#define     SUMSUNG_PULSE_NUM        110   //  ����������Ч����  
#define     SUMSUNG_PULSE_NUM_LONG   142   //  ����������Ч����  
#define     FUJITSU_PULSE_NUM        119   //  ����������Ч����  

  
#define     FUJITSU_LABEL           0X28//Ʒ�ƴ����ֽ�  
#define     SUMSUNG_LABEL           0X40//Ʒ�ƴ����ֽ�  
#define     LG_LABEL                0X88//Ʒ�ƴ����ֽ�  
#define     PANASONIC_LABEL         0X40//Ʒ�ƴ����ֽ�   
#define     HITACHI_LABEL           0X7F//Ʒ�ƴ����ֽ�     
#define     GALANZ_LABEL            0Xc4//Ʒ�ƴ����ֽ�   


#define     CHANGHONG_LABEL         0x6A//Ʒ�ƴ����ֽ�   
#define     DAIKIN_LABEL            0x88//Ʒ�ƴ����ֽ�   
#define     TCL_LABEL               0xC4//Ʒ�ƴ����ֽ�   
#define     CHUNLAN_LABEL           0x30//Ʒ�ƴ����ֽ�  
#define     KELON_LABEL             0xC1//Ʒ�ƴ����ֽ�  
#define     HUALING_LABEL           0xC4//Ʒ�ƴ����ֽ�  
#define     YAIR_LABEL              0xC4//Ʒ�ƴ����ֽ�
#define     MITSUBISHI_LABEL        0x54//Ʒ�ƴ����ֽ�
#define     MITSUBISHI_LABEL1       0x4A//Ʒ�ƴ����ֽ�  
#define     MIDEA_LABEL_ONE         0xB2//Ʒ�ƴ����ֽ�
#define     MIDEA_LABEL_TWO         0X4D//Ʒ�ƴ����ֽ�
#define     MIDEA_LABEL_ONE_ONE     0xB3//Ʒ�ƴ����ֽ�
#define     MIDEA_LABEL_THREE       0x7B//Ʒ�ƴ����ֽ�
#define     MIDEA_LABEL_FOUR        0XE0//Ʒ�ƴ����ֽ�
#define     AUX_LABEL               0xC3//Ʒ�ƴ����ֽ�
#define     HISENSE_LABEL           0x02//Ʒ�ƴ����ֽ� 
#define     HAIER_LABEL             0xA6//Ʒ�ƴ����ֽ�
#define     AUCMA_LABEL             0x55//Ʒ�ƴ����ֽ�
#define     SHINCO_LABEL            0x18//Ʒ�ƴ����ֽ�
#define     CHIGO_LABEL_ONE         0XFF//Ʒ�ƴ����ֽ�   
#define     CHIGO_LABEL_TWO         0X00//Ʒ�ƴ����ֽ�
 

//PERCFG
#define T1CFG       6   //��ʱ��1��IOλ�ã�0-����λ��1,1-����λ��2
#define T3CFG       5   //��ʱ��3��IOλ�ã�0-����λ��1,1-����λ��2
#define T4CFG       4   //��ʱ��4��IOλ�ã�0-����λ��1,1-����λ��2
#define U1CFG       1   //USART1��IOλ�ã�0-����λ��1,1-����λ��2
#define U0CFG       0   //USART0��IOλ�ã�0-����λ��1,1-����λ��2

//P0SEL-�˿�0����ѡ��
#define SELP0_0     0   //0-ͨ��IO��1-���蹦��
#define SELP0_1     1   //0-ͨ��IO��1-���蹦��
#define SELP0_2     2   //0-ͨ��IO��1-���蹦��
#define SELP0_3     3   //0-ͨ��IO��1-���蹦��
#define SELP0_4     4   //0-ͨ��IO��1-���蹦��
#define SELP0_5     5   //0-ͨ��IO��1-���蹦��
#define SELP0_6     6   //0-ͨ��IO��1-���蹦��
#define SELP0_7     7   //0-ͨ��IO��1-���蹦��

//P0DIR-�˿�0����
#define DIRP0_0     0   //0-���룬1-���
#define DIRP0_1     1   //0-���룬1-���
#define DIRP0_2     2   //0-���룬1-���
#define DIRP0_3     3   //0-���룬1-���
#define DIRP0_4     4   //0-���룬1-���
#define DIRP0_5     5   //0-���룬1-���
#define DIRP0_6     6   //0-���룬1-���
#define DIRP0_7     7   //0-���룬1-���

//P0INP-�˿�0����ģʽ
#define MDP0_0      0   //0-����/������1-��̬
#define MDP0_1      1   //0-����/������1-��̬
#define MDP0_2      2   //0-����/������1-��̬
#define MDP0_3      3   //0-����/������1-��̬
#define MDP0_4      4   //0-����/������1-��̬
#define MDP0_5      5   //0-����/������1-��̬
#define MDP0_6      6   //0-����/������1-��̬
#define MDP0_7      7   //0-����/������1-��̬

//P2INP
#define MDP2_0      0   //P2.0��IO����ģʽ��0-����/������1-��̬
#define MDP2_1      1   //P2.1��IO����ģʽ��0-����/������1-��̬
#define MDP2_2      2   //P2.2��IO����ģʽ��0-����/������1-��̬
#define MDP2_3      3   //P2.3��IO����ģʽ��0-����/������1-��̬
#define MDP2_4      4   //P2.4��IO����ģʽ��0-����/������1-��̬
#define PDUP0       5   //�˿�0����/����ѡ��0-������1-����
#define PDUP1       6   //�˿�1����/����ѡ��0-������1-����
#define PDUP2       7   //�˿�2����/����ѡ��0-������1-����

//T1STAT
#define CH0IF       0   //��ʱ��1ͨ��0�жϱ�־
#define CH1IF       1   //��ʱ��1ͨ��1�жϱ�־
#define CH2IF       2   //��ʱ��1ͨ��2�жϱ�־
#define CH3IF       3   //��ʱ��1ͨ��3�жϱ�־
#define CH4IF       4   //��ʱ��1ͨ��4�жϱ�־
#define OVFIF       5   //��ʱ��1����жϱ�־

//TxCCTLn
#define T1_RFIRQ    7   //����ʱʹ��RF��������ǳ��沶�����룬0-���沶�����룬1-RF��������
#define Tx_IM       6   //ͨ��n�ж����Ρ�����ʱʹ���ж�����0-�ж����Σ�1-�ж�ʹ��
#define Tx_CMP000   0   //000:�ڱȽ��������
#define Tx_CMP001   1   //001:�ڱȽ�������
#define Tx_CMP010   2   //010:�ڱȽ��л����
#define Tx_CMP011   3   //011:�����ϱȽ������������0����������ڱȽ������������0���
#define Tx_CMP100   4   //100:�����ϱȽ�����������0���á������ڱȽ�����������0����
#define Tx_CMP101   5   //101:������T1CC0ʱ�����������T1CCnʱ����
#define Tx_CMP110   6   //110:������T1CC0ʱ���ã�������T1CCnʱ���
#define Tx_CMP111   7   //111:��ʼ��������š�CMP[2:0]����
#define Tx_MODE     2   //ģʽ��ѡ��ʱ��1ͨ��n������߱Ƚ�ģʽ��0-����1-�Ƚ�
#define Tx_CAP00    0   //00:δ����
#define Tx_CAP01    1   //01:�����ز���
#define Tx_CAP10    2   //10:�½��ز���
#define Tx_CAP11    3   //11: �����ز���

//P2DIR
#define PRIP0_00    0   //00:��1���ȼ�-USART0����2���ȼ�-USART1����3���ȼ�-��ʱ��1
#define PRIP0_01    1   //01:��1���ȼ�-USART1����2���ȼ�-USART0����3���ȼ�-��ʱ��1
#define PRIP0_10    2   //00:��1���ȼ�-��ʱ��1ͨ��0-1����2���ȼ�-USART1����3���ȼ�-USART0����4���ȼ�-��ʱ��1ͨ��2-3
#define PRIP0_11    3   //00:��1���ȼ�-��ʱ��1ͨ��2-3����2���ȼ�-USART0����3���ȼ�-USART1����4���ȼ�-��ʱ��1ͨ��0-1

//T1CTL
#define T1DIV_1     0   //(3:2)���Ƶ��/1
#define T1DIV_8     1   //(3:2)���Ƶ��/8
#define T1DIV_32    2   //(3:2)���Ƶ��/32
#define T1DIV_128   3   //(3:2)���Ƶ��/128
#define T1MODE_STOP 0   //(1:0)��������
#define T1MODE_FREE 1   //(1:0)������
#define T1MODE_MODEL    2   //(1:0)ģ
#define T1MODE_UPDOWN   3   //(1:0)��������


#define IR_RECEIVE_PORT      P0_7

#define IR_RECEIVE_PORT_INIT()    st(IO_FUNC_PORT_PIN(0,7,IO_FUNC_GIO);   \
                    IO_DIR_PORT_PIN(0,7,IO_OUT);    \
                    IR_RECEIVE_PORT = true;	\
                    P2INP  |= (1<<PDUP0);)

#define IR_RECEIVE_PORT_SET() st(PERCFG |= (1<<T1CFG);        \
                    IO_FUNC_PORT_PIN(0,7,IO_FUNC_PERIPH);   \
                    IO_DIR_PORT_PIN(0,7,IO_IN); \
                    P0INP   &= ~(1<<MDP0_7);    \
                    P2INP   &= ~(1<<PDUP0);     \
                    P2DIR   |= (PRIP0_11<<6);)


#define IR_RECEIVE_PORT_SET2() st(PERCFG |= (1<<T1CFG);        \
                    IO_FUNC_PORT_PIN(0,5,IO_FUNC_PERIPH);   \
                    IO_DIR_PORT_PIN(0,5,IO_IN); \
                    P0INP   &= ~(1<<MDP0_5);    \
                    P2INP   &= ~(1<<PDUP0);     \
                    P2DIR   |= (PRIP0_11<<6);)
/*********************************************************************
 * VARIABLES
 */
extern bool   matchup_result;
extern bool   g_bCapture1st;
extern uint16 dev_mark;
#ifdef IR_MATCH_UP
extern void  ir_matchup_init(void);
extern  volatile  irda_matchup_t    ir_matchup_result;
#endif

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
*********************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* IR_MATCHUP */
