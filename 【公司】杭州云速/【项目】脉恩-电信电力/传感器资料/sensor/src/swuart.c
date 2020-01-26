/**
  ******************************************************************************
  * @file swuart.c
  * @brief This file contains all functions for sw uart control.
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

/* Includes ------------------------------------------------------------------*/

//#include "stm8s.h"	not used
#include "include.h"
#include <string.h>
#include "swuart.h"
/**
  * @addtogroup SW_UART
  * @{
  */
/* Private constants definition ----------------------------------------------*/

static const u8 MSK_TAB[9]= { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0 };
#define BUFF_SIZE 32
u8 rxBuff[BUFF_SIZE];
u8 rxIndex=0;
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

bool Rx_phase;	// phase of received bit [0-1] (edge, middle)
bool	Tx_phase;	// phase of transmited bit [0-1] (edge, middle)

#ifdef PARITY
bool	Rx_parity;	// received parity [0-1]
bool	Tx_parity;	// transmited parity [0-1]
#endif

#ifdef BIT9
_Bool	Rx_bit9;		// received 9-th data bit [0-1]
_Bool	Tx_bit9;		// transmited 9-th data bit [0-1]
#endif

	u8	Rx_bit,		// counter of received bits [0-11]
		Tx_bit,		// counter of transmited bits [0-11]
		Rx_samp,		// register of samples [0-3]
		Rx_buff,
		Rx_data,		// received byte register
		Tx_data,		// transmited byte register
		UART_sts;	// UART status register (1= active state)
//										bit			description
//										 7 ... transmit in progress
//										 6 ... transmit data register empty
//										 5 ... 9th data bit receive
//										 4 ... receive in progress
//										 3 ... receive buffer overflow
//										 2 ... receive frame error
//										 1 ... receive noise error
//										 0 ... receive frame ready (LSB)

/* Private function prototypes -----------------------------------------------*/
/**
  * @brief Procedure initiates HW pins, variables and timing system
  * @par Parameters:
  * None
  * @retval None
  */
void uart_init(void) {
#ifdef SWUART_TRANSMIT_USED
	init_UART_Tx_pin;			// Tx pin hardware init
#endif

#ifdef SWUART_RECEIVE_USED
	init_UART_Rx_pin;			//	Rx pin hardware init
#endif

#ifdef TEST_PIN_USED
	init_UART_Test_pin;		// Test pin hardware init
#endif

//	UART_sts= transmit_data_reg_empty;	// initialization of variables

	init_UART_timing;			// user initialization of timing system used
    uart_receive_enable;
}
//----------------------------------------------------------
#ifdef SWUART_TRANSMIT_USED
/**
  * @brief Sending byte initialization if it could be buffered
  * @par Parameters:
  * u8 byte to be transmitted
  * @retval TRUE (byte is sent) or FALSE (byte could not be buffered)
  */
u8 uart_send(u8 b) {	//transmition/bufferring possible?
	if(!test_status(transmit_in_progress) || test_status(transmit_data_reg_empty))	{
		Tx_data= b;					//YES - initiate sending procedure
		clr_status(transmit_data_reg_empty);
		if(!test_status(transmit_in_progress)) {
			Tx_phase= Tx_bit= 0;
			set_status(transmit_in_progress);
		};
		return(true);
	}
	else
		return(false);				//NO - no action (transmition in progress)
}
//----------------------------------------------------------
/**
  * @brief Timing of uart Tx - all bits transmit proccess control
  * @par Parameters:
  * None
  * @retval None
  */
void uart_Tx_timing(void) {
	clear_owerflow_flag;
	if(Tx_phase) {
		if(test_status(transmit_in_progress)) {	// edge of current bit (no service for middle)
			switch(Tx_bit) {			// begin of bit transmition
				case 0:					clr_Tx;	//start bit transmition
#ifdef PARITY
											Tx_parity= 0;
#endif
											break;
#ifdef PARITY
				case DATA_LENGTH:		if(Tx_parity) set_Tx;
											else          clr_Tx;
											break;
#else
#ifdef BIT9
case DATA_LENGTH:		if(Tx_bit9) set_Tx;
											else        clr_Tx;
											break;
#endif
#endif
				case DATA_LENGTH+1:	set_status(transmit_data_reg_empty);
				case DATA_LENGTH+2:	set_Tx;	//stop bit(s) transmition
											break;
#ifdef PARITY
				default:	if(Tx_data & MSK_TAB[Tx_bit-1]) { set_Tx; Tx_parity= ~Tx_parity; }
							else										 clr_Tx; // parity transmition
#else
				default:
                    if(Tx_data & MSK_TAB[Tx_bit-1])
                    {
                        set_Tx; //  data bits
                    }
                    else
                    {
                        clr_Tx; // transmition
                    }
#endif
			};
			if(Tx_bit >= DATA_LENGTH + STOP_BITS) {	// end of current transmited bit
				if(test_status(transmit_data_reg_empty)) {	// end of transmition
					clr_status(transmit_in_progress);				// no next Tx request
				}
				else
					Tx_bit= 0;							// next byte is buffered - continue
			}												//			with transmition
			else
				++Tx_bit;				// next bit to transmit
		};
	};
	Tx_phase= ~Tx_phase;
}
#endif

//----------------------------------------------------------
#ifdef SWUART_RECEIVE_USED
/**
  * @brief Receive byte checking and store
  * @par Parameters:
  * u8 pointer to place where the received byte should be stored
  * @retval error code in low nibble (see codes definition in documentation
  * - TRUE meens no error receive) or FALSE (no byte received)
  */
//u8 uart_read(u8 *b) {
//	u8 res;
//	if(test_status(receive_buffer_full))	{
//		*b= Rx_data;
//#ifdef BIT9
//		res= UART_sts & 0x2F;
//#else
//		res= UART_sts & 0x0F;
//#endif
//		UART_sts&= ~0x0F;
//		return(res);
//	}
//	else
//		return(FALSE);
//}
u8 uart_recive(u8 *buff)
{
    u8 ret=0;
    sys_int_disable();
    if(test_status(receive_frame_ready)&&rxIndex>0)
    {
        ret=rxIndex;
        memcpy(buff,rxBuff,rxIndex);
        clr_status(receive_frame_ready);
        rxIndex=0;
    }
    sys_int_enable();
    return ret;
}
u8 uart_sendstr(u8 *dat,u8 len)
{
    while(len>0)
    {
        if(uart_send(*dat)==true)
        {
            while(test_status(transmit_in_progress) != 0);
            dat++;
            len--;
        }
    }
    return len;
}
void uart_timer()
{
    static u8 rx_tmo = 0;
    if(rxIndex>0)
    {
        rx_tmo++;
        if(!test_status(receive_in_progress))
        {
            if(rx_tmo>SWUART_RX_TIMEOUT)
            {
                rx_tmo=0;
                set_status(receive_frame_ready);
            }
        }
        else
        {
            rx_tmo=0;
        }
    }
    else
    {
        clr_status(receive_frame_ready);
    }
}

//----------------------------------------------------------
/**
  * @brief Timing of uart Rx - receive all bits proccess control
  * @par Parameters:
  * None
  * @retval None
  */

void uart_Rx_timing2(void)
{
    clear_cc_flag;
    
#ifdef TEST_PIN_USED
    set_Test_Pin;
#endif
    
    Rx_samp= 0;				// middle of current bit, checking samples
    if(Rx_test)	++Rx_samp; // sampling in the middle of current bit
    if(Rx_test)	++Rx_samp;
    if(Rx_test)	++Rx_samp;
    if(test_status(receive_in_progress))    //check if processing a data byte
    {
        if(Rx_bit <= DATA_LENGTH)
        {
            if(Rx_samp==3)
                Rx_buff|= MSK_TAB[Rx_bit-1];
            else if(Rx_samp==2||Rx_samp==1)
            {
                set_status(receive_noise_error);
                disable_OC_system;		// init next byte receive
                clr_status(receive_in_progress);
                enable_IC_system;
            }
            Rx_bit++;
        }
        else if(Rx_bit>=9)//stop bit,no parity supported
        {
            if(Rx_samp==3)  //stop bit must be high
            {
                rxBuff[rxIndex]= Rx_buff;       //write current byte to buff array
                if(++rxIndex>=BUFF_SIZE)rxIndex=0;  //buff range check
            }
            else
            {
                rxIndex=0;
                set_status(receive_noise_error);  //stop bit not correct,set receive_noise_error
            }
            disable_OC_system;		// init next byte receive
            clr_status(receive_in_progress);
            enable_IC_system;
        }
    }
    else
    {
        {
            if(Rx_samp==0)// start bit!
            {		
                Rx_bit= 1;			// correctly received, continue
                Rx_buff= 0;
                set_status(receive_in_progress); // data overflow!
                disable_IC_system;		// init next byte receive
                enable_OC_system;
            }
            else    //start bit not correct
            {
                disable_OC_system;		// init next byte receive
                clr_status(receive_in_progress);
                enable_IC_system;
            }
        }
    }
}

#if 0
void uart_Rx_timing(void) {
	clear_cc_flag;
	if(test_status(receive_in_progress)) {
		if(!Rx_phase) {		// receive is in progres now
			Rx_samp= 0;				// middle of current bit, checking samples
#ifdef TEST_PIN_USED
			set_Test_Pin;
#endif
			if(Rx_test)	++Rx_samp; // sampling in the middle of current bit
			if(Rx_test)	++Rx_samp;
			if(Rx_test)	++Rx_samp;
#ifdef TEST_PIN_USED
			clr_Test_Pin;
#endif

			if(Rx_bit==0) {
				if(Rx_samp==0) {		// start bit!
					Rx_bit= 1;			// correctly received, continue
					Rx_buff= 0;
				}
				else {					// noise in start bit, find next one
					disable_IC_system;
					enable_OC_system;
					clr_status(receive_in_progress);
				};
			}
			else {
				switch(Rx_samp) {		// any other bit, results?
					case 1:	set_status(receive_noise_error);
								break;	// noise in middle samples, "0" received
					case 2: 	set_status(receive_noise_error);
											// noise in middle samples, "1" received
#ifdef PARITY
					case 3:	if(Rx_bit < DATA_LENGTH)  Rx_buff|= MSK_TAB[Rx_bit-1];
								if(Rx_bit <= DATA_LENGTH) Rx_parity= ~Rx_parity;
#else
#ifdef BIT9
					case 3:	if(Rx_bit < DATA_LENGTH)  Rx_buff|= MSK_TAB[Rx_bit-1];
								if(Rx_bit == DATA_LENGTH) Rx_bit9= 1;
#else
					case 3:	if(Rx_bit <= DATA_LENGTH) Rx_buff|= MSK_TAB[Rx_bit-1];
#endif
#endif
								break;	// "1" correctly received
				};
				if(Rx_bit > DATA_LENGTH) {
#ifdef PARITY
					if(Rx_samp != 3  || Rx_parity)	// stop bit(s) are received, results?
#else
					if(Rx_samp != 3)	// stop bit(s) are received, results?
#endif
						set_status(receive_frame_error);		// noise in stop bit or parity error
					if(Rx_bit >= DATA_LENGTH + STOP_BITS) {
						if(!test_status(receive_buffer_full)) { // end of receive
							Rx_data= Rx_buff;								// new byte in buffer
#ifdef BIT9
							if(Rx_bit9) set_status(receive_9th_data_bit);
							else			clr_status(receive_9th_data_bit);
#endif
							set_status(receive_buffer_full);
						}
						else
							set_status(receive_buffer_overflow); // data overflow!
						disable_OC_system;		// init next byte receive
						clr_status(receive_in_progress);
						enable_IC_system;
					}
					else
						++Rx_bit;
				}
				else
					++Rx_bit;			// init next data bit receive
			}
		}
		Rx_phase=~Rx_phase;
	}
	else {								// receive is not in progres yet
		disable_IC_system;			// IC interrupt - begin of start bit detected
		enable_OC_system;				//	OC interrupt period as a conseqence of start bit falling edge
		set_status(receive_in_progress);	// receive byte initialization
		Rx_bit= 0; Rx_phase= 0;
#ifdef PARITY
		Rx_parity= 0;
#else
#ifdef BIT9
		Rx_bit9= 0;
#endif
#endif
	};
}
#endif
//----------------------------------------------------------
#endif
/**
  * @}
  */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
