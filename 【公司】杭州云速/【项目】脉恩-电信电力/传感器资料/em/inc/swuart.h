/**
  ******************************************************************************
  * @file swuart.h
  * @brief This file contains all user and system definition of uart software.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.0.0
  * @date 10/13/2008
  ******************************************************************************
  *
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2008 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SW_UART_H
#define __SW_UART_H

// ---------------------------------------------------------
// --------------- user macros definition ------------------
// ---------------------------------------------------------
#ifndef _COSMIC_
#define _Bool   bit
#endif

#define SWUART_TRANSMIT_USED
#define SWUART_RECEIVE_USED
//#define TEST_PIN_USED

#define	DATA_LENGTH		8		// number of data bits in protocol
#define	STOP_BITS		1		// number of stop bits in protocol
//#define  PARITY
// if PARITY flag si defined the most significant data
// 			bit is considered to be parity bit

// user HW definition -------------------------------------
// Macros in following section should be carefully completed by user 
// in according with hardware connection of Tx and Rx pins, used
// protocol and speed (Rx pin is supposed with capture capability,
// Tx pin is any with output capability)
//----------------------------------------------------------

#ifdef SWUART_TRANSMIT_USED
#define	UART_TxPIN	0x02
#define UART_TxPORT_DDR PD_DDR
#define UART_TxPORT_ODR PD_ODR
#define UART_TxPORT_CR1 PD_CR1

#endif

#ifdef SWUART_RECEIVE_USED
#define	UART_RxPIN	0x10
#define UART_RxPORT_DDR PC_DDR
#define UART_RxPORT_IDR PC_IDR
#define UART_RxPORT_CR1 PC_CR1
#define UART_RxPORT_CR2 PC_CR2
#endif

#ifdef TEST_PIN_USED
#define  UART_TestPORT	(GPIOD)
#define	UART_TestPIN	(GPIO_PIN_4)
#endif

#define	OCInit_TypeDef		TIM3_OCInit_TypeDef TIM3_OCInitStruct

/* uncomment the next line to switch the system clock from HSI (16 Mhz) to HSE (24 Mhz) */
#define TO_HSE

//	Here user must ensure calling service interrupt routine at exact intervals 
//	of 1/2 bit of the dedicated baud rate (see also TO_HSE switch above)
// TIM3 overflow period must be set in dependance on fmaster clock selected:
//                                     fmaster= fcpu clock:   16 / 24 Mhz
// 	for 9600Bd speed set overflow every 52.08us - Period=  833 / 1250
//   for 19200Bd speed set overflow every 26,04us - Period=  417 / 625
//   for 57600Bd speed set overflow every  8,68us - Period=  139*/ 208
// *) for half duplex only!
// next macro initializes and enable TIM3 base and interrupt system, divider
// register /1, auto reload register, enable interrupt on update, update on overflow


#define init_UART_timing	{\
    TIM1_PSCRH=0;\
    TIM1_PSCRL=31;\
    TIM1_ARRH=0;\
    TIM1_ARRL=208;\
    TIM1_IER|=0x01;\
    TIM1_CR1|=0x10|0x01;\
}

// input capture system initialization
//filter (4 samples), IC2 mapped on TI2FP2, capture on faling edge
#define	init_ic_setting	{\
    TIM1_CCER2&=~0x10;\
    TIM1_CCMR4=(1<<0)|(2<<4);\
    TIM1_CCER2=0x30;\
}
// output compare system initialization
// frozen (timing only) mode, preload regs disable, no output
#define  init_oc_setting {\
	TIM1_CCER2|= 0x10;\
	TIM1_CCMR4= 0;\
	TIM1_CCMR4|= 0x80;\
}

#define	clear_owerflow_flag	{ TIM1_SR1&=~0x01; }
#define	clear_cc_flag			{ TIM1_SR1 &= ~0x10;	}
#define  enable_cc_interrupt	{ TIM1_IER |=  0x10; }
#define  disable_cc_interrupt	{ TIM1_IER &= ~0x10; }

#define	enable_IC_system		{ /* clear_cc_flag;*/ init_ic_setting; enable_cc_interrupt; }
#define	disable_IC_system	 {\
	disable_cc_interrupt;\
	TIM1_CCER2&=~0x10;\
	TIM1_CCMR4&=~ 0x03;\
};

#define	enable_OC_system		{ /* clear_cc_flag;*/ init_oc_setting; enable_cc_interrupt; }
#define	disable_OC_system	  	  disable_cc_interrupt

// ---------------------------------------------------------
// ------------- private macros definition -----------------
// ---------------------------------------------------------

#if (DATA_LENGTH == 9)
#ifndef PARITY
#define BIT9					// definition of 9-th data bit flag
#define	set_Tx_bit9		{ Tx_bit9= 1; }
#define	clr_Tx_bit9		{ Tx_bit9= 0; }
#endif
#endif

#if (DATA_LENGTH > 9)				// checking the parameters range
#error DATA LENGTH SHOULD NOT BE LONGER THAN NINE BITS
#endif
#if (DATA_LENGTH < 1)
#error DATA LENGTH SHOULD NOT BE SHORTER THAN ONE BIT
#endif
#if (STOP_BITS > 2)
#error NO MORE THAN TWO STOP BITS SHOULD BE DEFINED
#endif
#if (STOP_BITS < 1)
#error AT LEAST ONE STOP BIT SHOULD BE DEFINED
#endif

// bit manipulation definition
#ifdef SWUART_RECEIVE_USED
#define Rx_test	(UART_RxPORT_IDR & UART_RxPIN)
#endif
#ifdef SWUART_TRANSMIT_USED
#define set_Tx		(UART_TxPORT_ODR |= UART_TxPIN)
#define clr_Tx		(UART_TxPORT_ODR &=~UART_TxPIN)
#endif
#ifdef TEST_PIN_USED
#define set_Test_Pin		(UART_TestPORT->ODR |= UART_TestPIN)
#define clr_Test_Pin		(UART_TestPORT->ODR &=~UART_TestPIN)
#endif

// initial definition of HW pins Tx as output push-pull, Rx as input floating
 
#ifdef SWUART_TRANSMIT_USED
#define init_UART_Tx_pin	{ UART_TxPORT_DDR|=UART_TxPIN;UART_TxPORT_CR1|=UART_TxPIN; }

#endif
#ifdef SWUART_RECEIVE_USED
#define init_UART_Rx_pin	{ UART_RxPORT_DDR&=~UART_RxPIN;UART_RxPORT_CR1&=~UART_RxPIN; }
#endif
#ifdef TEST_PIN_USED
#define init_UART_Test_pin	{ GPIO_Init(UART_TestPORT, UART_TestPIN, GPIO_MODE_OUT_PP_LOW_FAST); }
#endif

/* Exported constants --------------------------------------------------------*/
// status masks
#define	transmit_in_progress			0x80
#define	transmit_data_reg_empty		0x40
#define	receive_9th_data_bit			0x20
#define	receive_in_progress			0x10

#define	receive_buffer_overflow		0x08 // low nibble corresponds to error return value
#define	receive_frame_error			0x04
#define	receive_noise_error			0x02
#define	receive_frame_ready			0x01

#define SWUART_RX_TIMEOUT   30

// error return codes
#define OVFL_ERROR	8
#define FRAME_ERROR	4
#define NOISE_ERROR	2
#define RX_BUFF_FULL	1

        
/* Exported macros ------------------------------------------------------------*/
#define test_status(a)		(UART_sts & a)
#define set_status(a)		(UART_sts |= a)
#define clr_status(a)		(UART_sts &=~a)

#define uart_receive_enable 	{ Rx_bit= Rx_phase= 0; disable_OC_system; clear_cc_flag;enable_IC_system; }
#define uart_receive_disable 	{ disable_IC_system; disable_OC_system; clr_status(receive_in_progress); }

/* Exported variables --------------------------------------------------------*/
extern bool Rx_phase;
extern bool Tx_phase;
#ifdef PARITY
extern bool Rx_parity;
extern bool Tx_parity;
#else
#ifdef BIT9
extern bool Rx_bit9;
extern bool Tx_bit9;
#endif
#endif
extern u8 Rx_bit,
			 Rx_samp,
			 Tx_bit,
			 Rx_buff,
			 Rx_data,
			 Tx_data,
			 UART_sts;
			 
/* Exported functions ------------------------------------------------------- */
void uart_init(void);
void uart_Tx_timing(void);
void uart_Rx_timing(void);
void uart_Rx_timing2(void);
u8 uart_recive(u8 *buff);
void uart_timer();
u8 uart_sendstr(u8 *dat,u8 len);
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
