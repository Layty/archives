#include <iostm8s105c6.h>
 #include <stdio.h>

#define BAUD_RATE     9600         //波特率
#define F_MASTER      16000000     //主频率

#define DS18B20_DQ_OUT          PG_DDR_DDR0 = 1   //输出
#define DS18B20_DQ_IN           PG_DDR_DDR0 = 0   //输入
#define DS18B20_DQ_HIGH         PG_ODR_ODR0 = 1   //拉高
#define DS18B20_DQ_LOW          PG_ODR_ODR0 = 0   //拉低
#define DS18B20_DQ_PULL_UP      PG_CR1_C10  = 1   //上拉
#define DS18B20_DQ_FLOATING     PG_CR1_C10  = 0   //浮空
#define DS18B20_DQ_PUSH_PULL    PG_CR1_C10  = 1   //推挽
#define DS18B20_DQ_OPEN_DRAIN   PG_CR1_C10  = 0   //开漏
#define DS18B20_DQ_VALUE        PG_IDR_IDR0       //DQ值

//串口配置
//数据位:8
//停止位:1
//校验位:None
void UART_Init(void)
 {
     UART2_CR2_TEN = 1;
     UART2_CR2_REN = 1;
     UART2_CR2_RIEN = 1;
     UART2_BRR2 = ((unsigned char)((F_MASTER / BAUD_RATE) & 0x0f)) + (((unsigned char)((F_MASTER / BAUD_RATE) >> 8)) & 0xf0);
     UART2_BRR1 = ((unsigned char)((F_MASTER / BAUD_RATE) >> 4));
 }

void UART_TxByte(unsigned char _data)
 {
     while (UART2_SR_TXE == 0);
     UART2_DR = _data;
 }

int putchar(int c)
 {
     UART_TxByte(c);
     return c;
 }

void _delay_us(unsigned int i)
 {
     i *= 3; 
     while(--i);
 }

void _delay_ms(unsigned int i)
 {
     while(i--)
     {
         _delay_us(1000);
     }
 }

void DS18B20_Init(void)
 {
     DS18B20_DQ_OUT;   
     DS18B20_DQ_PUSH_PULL;    
     DS18B20_DQ_HIGH;   
     _delay_us(10);
     DS18B20_DQ_LOW;   
     _delay_us(600);     //复位脉冲
    
     DS18B20_DQ_IN;   
     DS18B20_DQ_PULL_UP;    
     _delay_us(100);     
     while(DS18B20_DQ_VALUE == 1);
     _delay_us(400);
 }

void DS18B20_WriteByte(unsigned char _data)
 {
     unsigned char i = 0;

     DS18B20_DQ_OUT;
     for (i = 0; i < 8; i++)
     {
         DS18B20_DQ_LOW;
         _delay_us(2);
         if (_data & 0x01)
         {
             DS18B20_DQ_HIGH;
         }
         _data >>= 1;
         _delay_us(60);
         DS18B20_DQ_HIGH;
     }
 }

 unsigned char DS18B20_ReadByte(void)
 {
     unsigned char i = 0, _data = 0;

     for (i = 0; i < 8; i++)
     {
         DS18B20_DQ_OUT;
         DS18B20_DQ_LOW;
         _delay_us(5);
         _data >>= 1;
         DS18B20_DQ_HIGH;
         DS18B20_DQ_IN;
         if (DS18B20_DQ_VALUE)
         {
             _data |= 0x80;
         }
         DS18B20_DQ_OUT; 
         DS18B20_DQ_HIGH;
         _delay_us(60);
     }

     return _data;
 }

float DS18B20_ReadTemperature(void)
 {
     unsigned char temp = 0;
     float t = 0;
     
     DS18B20_Init();
     DS18B20_WriteByte(0xcc);
     DS18B20_WriteByte(0x44);

     DS18B20_Init();
     DS18B20_WriteByte(0xcc);
     DS18B20_WriteByte(0xbe);

     temp = DS18B20_ReadByte();
     t = (((temp & 0xf0) >> 4) + (temp & 0x07) * 0.125); 
     temp = DS18B20_ReadByte();
     t += ((temp & 0x0f) << 4);
     
     return t;
 }

int main(void)
 {  
     CLK_SWCR_SWEN = 1;
     CLK_SWR = 0xB4;    //HSE selected as master clock source
    
     UART_Init();
     printf("********* STM8S-Discovery DS18B20 Test *********\r\n");
     printf("Build: %s  %s\r\n", __DATE__, __TIME__);
     
     asm("rim");
     
     while(1)
     {
         _delay_ms(1000);
         printf("%.3f ", DS18B20_ReadTemperature());
     }
 }

#pragma vector = UART2_R_RXNE_vector
 __interrupt void UART2_IRQHandler(void)
 {
     if (UART2_SR_RXNE == 1)
     {
         UART_TxByte(UART2_DR);
     }
 }
