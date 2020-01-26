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
#define     WINDLEVEL_POS  2       //对不上，要转换一下
#define     TEMP_POS       3
#define     OPPOSITE       2       //开关机是取反的
#define     DAIKIN_MARK    20000   //大金码序列中的长脉冲限值

#define     ONOff_SCOPE      4
#define     SYSMODE_SCOPE    5
#define     WINDLEVEL_SCOPE  6
#define     TEMP_SCOPE       7
#define     MATCHUP_UPLOAD_LENGTH   19
  
//#define     TRANSFER       21         //转换板接收38k载波  
#define     DATA_ERROR               3     //识别过程数据区数据错误
#define     LENGTH_ERROR             2     //识别过程脉冲个数错误
#define     GREE_PULSE_NUM           65    //格力脉冲最少有效数量
#define     MIDEA_PULSE_NUM_LONGER   140   //  脉冲最少有效数量
#define     MIDEA_PULSE_NUM_SHORTER  90    //  脉冲最少有效数量
#define     CHIGO_PULSE_NUM          90    //  脉冲最少有效数量
#define     AUX_PULSE_NUM            100   //  脉冲最少有效数量
#define     HISENSE_PULSE_NUM        130   //  脉冲最少有效数量
#define     HAIER_PULSE_NUM          105   //  脉冲最少有效数量
#define     AUCMA_PULSE_NUM          92    //  脉冲最少有效数量
#define     SHINCO_PULSE_NUM         38    //  脉冲最少有效数量
#define     MITSUBISHI_PULSE_NUM     45    //  脉冲最少有效数量
#define     MITSUBISHI_PULSE_NUM1    87    //  脉冲最少有效数量
#define     YAIR_PULSE_NUM           110    //  脉冲最少有效数量
#define     HUALING_PULSE_NUM        110   //  脉冲最少有效数量
#define     KELON_PULSE_NUM          110   //  脉冲最少有效数量
#define     CHUNLAN_PULSE_NUM        70   //  脉冲最少有效数量
#define     TCL_PULSE_NUM            100   //  脉冲最少有效数量 
#define     DAIKIN_PULSE_NUM         120   //  脉冲最少有效数量 
#define     CHANGHONG_PULSE_NUM      115   //  脉冲最少有效数量 CHANGHONG
#define     TRANSFER_PULSE_NUM       10
#define     GALANZ_PULSE_NUM         110   //  脉冲最少有效数量 
#define     HITACHI_PULSE_NUM        260   //  脉冲最少有效数量
#define     PANASONIC_PULSE_NUM      215   //  脉冲最少有效数量
#define     LG_PULSE_NUM             26    //  脉冲最少有效数量  
#define     SUMSUNG_PULSE_NUM        110   //  脉冲最少有效数量  
#define     SUMSUNG_PULSE_NUM_LONG   142   //  脉冲最少有效数量  
#define     FUJITSU_PULSE_NUM        119   //  脉冲最少有效数量  

  
#define     FUJITSU_LABEL           0X28//品牌代码字节  
#define     SUMSUNG_LABEL           0X40//品牌代码字节  
#define     LG_LABEL                0X88//品牌代码字节  
#define     PANASONIC_LABEL         0X40//品牌代码字节   
#define     HITACHI_LABEL           0X7F//品牌代码字节     
#define     GALANZ_LABEL            0Xc4//品牌代码字节   


#define     CHANGHONG_LABEL         0x6A//品牌代码字节   
#define     DAIKIN_LABEL            0x88//品牌代码字节   
#define     TCL_LABEL               0xC4//品牌代码字节   
#define     CHUNLAN_LABEL           0x30//品牌代码字节  
#define     KELON_LABEL             0xC1//品牌代码字节  
#define     HUALING_LABEL           0xC4//品牌代码字节  
#define     YAIR_LABEL              0xC4//品牌代码字节
#define     MITSUBISHI_LABEL        0x54//品牌代码字节
#define     MITSUBISHI_LABEL1       0x4A//品牌代码字节  
#define     MIDEA_LABEL_ONE         0xB2//品牌代码字节
#define     MIDEA_LABEL_TWO         0X4D//品牌代码字节
#define     MIDEA_LABEL_ONE_ONE     0xB3//品牌代码字节
#define     MIDEA_LABEL_THREE       0x7B//品牌代码字节
#define     MIDEA_LABEL_FOUR        0XE0//品牌代码字节
#define     AUX_LABEL               0xC3//品牌代码字节
#define     HISENSE_LABEL           0x02//品牌代码字节 
#define     HAIER_LABEL             0xA6//品牌代码字节
#define     AUCMA_LABEL             0x55//品牌代码字节
#define     SHINCO_LABEL            0x18//品牌代码字节
#define     CHIGO_LABEL_ONE         0XFF//品牌代码字节   
#define     CHIGO_LABEL_TWO         0X00//品牌代码字节
 

//PERCFG
#define T1CFG       6   //定时器1的IO位置，0-备用位置1,1-备用位置2
#define T3CFG       5   //定时器3的IO位置，0-备用位置1,1-备用位置2
#define T4CFG       4   //定时器4的IO位置，0-备用位置1,1-备用位置2
#define U1CFG       1   //USART1的IO位置，0-备用位置1,1-备用位置2
#define U0CFG       0   //USART0的IO位置，0-备用位置1,1-备用位置2

//P0SEL-端口0功能选择
#define SELP0_0     0   //0-通用IO，1-外设功能
#define SELP0_1     1   //0-通用IO，1-外设功能
#define SELP0_2     2   //0-通用IO，1-外设功能
#define SELP0_3     3   //0-通用IO，1-外设功能
#define SELP0_4     4   //0-通用IO，1-外设功能
#define SELP0_5     5   //0-通用IO，1-外设功能
#define SELP0_6     6   //0-通用IO，1-外设功能
#define SELP0_7     7   //0-通用IO，1-外设功能

//P0DIR-端口0方向
#define DIRP0_0     0   //0-输入，1-输出
#define DIRP0_1     1   //0-输入，1-输出
#define DIRP0_2     2   //0-输入，1-输出
#define DIRP0_3     3   //0-输入，1-输出
#define DIRP0_4     4   //0-输入，1-输出
#define DIRP0_5     5   //0-输入，1-输出
#define DIRP0_6     6   //0-输入，1-输出
#define DIRP0_7     7   //0-输入，1-输出

//P0INP-端口0输入模式
#define MDP0_0      0   //0-上拉/下拉，1-三态
#define MDP0_1      1   //0-上拉/下拉，1-三态
#define MDP0_2      2   //0-上拉/下拉，1-三态
#define MDP0_3      3   //0-上拉/下拉，1-三态
#define MDP0_4      4   //0-上拉/下拉，1-三态
#define MDP0_5      5   //0-上拉/下拉，1-三态
#define MDP0_6      6   //0-上拉/下拉，1-三态
#define MDP0_7      7   //0-上拉/下拉，1-三态

//P2INP
#define MDP2_0      0   //P2.0的IO输入模式，0-上拉/下拉，1-三态
#define MDP2_1      1   //P2.1的IO输入模式，0-上拉/下拉，1-三态
#define MDP2_2      2   //P2.2的IO输入模式，0-上拉/下拉，1-三态
#define MDP2_3      3   //P2.3的IO输入模式，0-上拉/下拉，1-三态
#define MDP2_4      4   //P2.4的IO输入模式，0-上拉/下拉，1-三态
#define PDUP0       5   //端口0上拉/下拉选择，0-上拉，1-下拉
#define PDUP1       6   //端口1上拉/下拉选择，0-上拉，1-下拉
#define PDUP2       7   //端口2上拉/下拉选择，0-上拉，1-下拉

//T1STAT
#define CH0IF       0   //定时器1通道0中断标志
#define CH1IF       1   //定时器1通道1中断标志
#define CH2IF       2   //定时器1通道2中断标志
#define CH3IF       3   //定时器1通道3中断标志
#define CH4IF       4   //定时器1通道4中断标志
#define OVFIF       5   //定时器1溢出中断标志

//TxCCTLn
#define T1_RFIRQ    7   //设置时使用RF捕获而不是常规捕获输入，0-常规捕获输入，1-RF捕获输入
#define Tx_IM       6   //通道n中断屏蔽。设置时使能中断请求，0-中断屏蔽，1-中断使能
#define Tx_CMP000   0   //000:在比较设置输出
#define Tx_CMP001   1   //001:在比较清除输出
#define Tx_CMP010   2   //010:在比较切换输出
#define Tx_CMP011   3   //011:在向上比较设置输出，在0清除。否则在比较设置输出，在0清除
#define Tx_CMP100   4   //100:在向上比较清除输出，在0设置。否则在比较清除输出，在0设置
#define Tx_CMP101   5   //101:当等于T1CC0时清除，当等于T1CCn时设置
#define Tx_CMP110   6   //110:当等于T1CC0时设置，当等于T1CCn时清除
#define Tx_CMP111   7   //111:初始化输出引脚。CMP[2:0]不变
#define Tx_MODE     2   //模式。选择定时器1通道n捕获或者比较模式，0-捕获，1-比较
#define Tx_CAP00    0   //00:未捕获
#define Tx_CAP01    1   //01:上升沿捕获
#define Tx_CAP10    2   //10:下降沿捕获
#define Tx_CAP11    3   //11: 所有沿捕获

//P2DIR
#define PRIP0_00    0   //00:第1优先级-USART0，第2优先级-USART1，第3优先级-定时器1
#define PRIP0_01    1   //01:第1优先级-USART1，第2优先级-USART0，第3优先级-定时器1
#define PRIP0_10    2   //00:第1优先级-定时器1通道0-1，第2优先级-USART1，第3优先级-USART0，第4优先级-定时器1通道2-3
#define PRIP0_11    3   //00:第1优先级-定时器1通道2-3，第2优先级-USART0，第3优先级-USART1，第4优先级-定时器1通道0-1

//T1CTL
#define T1DIV_1     0   //(3:2)标记频率/1
#define T1DIV_8     1   //(3:2)标记频率/8
#define T1DIV_32    2   //(3:2)标记频率/32
#define T1DIV_128   3   //(3:2)标记频率/128
#define T1MODE_STOP 0   //(1:0)自由运行
#define T1MODE_FREE 1   //(1:0)倒计数
#define T1MODE_MODEL    2   //(1:0)模
#define T1MODE_UPDOWN   3   //(1:0)正倒计数


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
