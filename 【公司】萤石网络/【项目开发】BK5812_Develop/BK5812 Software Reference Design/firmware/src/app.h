/*******************************************************************************
  MPLAB Harmony Application

  Application Header
  
  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
	Application definitions. 

  Description:
	 This file contains the  application definitions.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

#ifndef _APP_HEADER_H
#define _APP_HEADER_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "peripheral/usart/plib_usart.h"
#include "peripheral/devcon/plib_devcon.h"
#include "system/debug/sys_debug.h"
#include "driver/usart/src/drv_usart_static_local.h"
#include "system_config/pic32mx795_pim_e16_freertos/framework/driver/tmr/drv_tmr_static.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Application States

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* USART Enable State */
    USART_ENABLE,

    /* USART Transmit First string */
    USART_TRANSMIT_FIRST_STRING,

    /* USART Receive State */
    USART_RECEIVE_DONE

} APP_STATES;





// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct {
    /* The application's current state */
    APP_STATES state;

    /* Flag to indicate an interrupt has occured */
    bool InterruptFlag;

    /* Pointer to hold the present character of string to be transmitted */
    const char *stringPointer;

    /* Data received from UART */
    char data;

    /* rf tick */
    uint64_t rfTick;
    uint64_t irqTick;

    /* system tick */
    //uint64_t sysTick;

} APP_DATA;


// *****************************************************************************

/* Driver objects.

  Summary:
    Holds driver objects.

  Description:
    This structure contains driver objects returned by the driver init routines
    to the application. These objects are passed to the driver tasks routines.

  Remarks:
    None.
*/

typedef struct
{
	//SYS_MODULE_OBJ   drvObject;
	 
} APP_DRV_OBJECTS;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony Demo application initialization routine

  Description:
    This routine initializes Harmony Demo application.  This function opens
    the necessary drivers, initializes the timer and registers the application
    callback with the USART driver.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    APP_Initialize();


  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks ( void );

/*******************************************************************************
  Function:
    bool PutCharacter(const char character)

  Summary:
    Function to put a character to UART

  Description:
    This function for UART transmits a character on UART.
    This function is non-blocking and will return the control back to calling
    function if the transmit buffer is not available for a new transmission. The
    function will return a "true" if the character is transmitted. Otherwise,
    the function will return "false".

  Precondition:
     UART should be enabled and TX function should be enabled before calling this
     function.

  Parameters:
    Character to be transmitted to UART.

  Returns:
    true: If the character could be transmitted.
    false: If the character could not be transmitted.

  Example:
    while(false == PutCharacter('A'));


  Remarks:
    This routine must be called from the APP_Tasks function.
*/
bool PutCharacter(const char character);

/*******************************************************************************
  Function:
    bool WriteString(void)

  Summary:
    Function to put a string to UART

  Description:
    This function transmits a string to UART. The string pointer is passed using
    a global pointer "stringPointer" define in structure "APP_DATA". 
    This function is non-blocking and will return the control back to calling
    function if the transmit buffer is not available for a new transmission. The
    function will return a "true" if the entire string is transmitted. Otherwise,
    the function will return "false".

  Precondition:
    UART should be enabled and TX function should be enabled before calling this
    function.
    The string to be transmitted should be terminated with NULL character.

  Parameters:
    Character to be transmitted to UART.

  Returns:
    true: If the function encounters a NULL character (string is completed).
    false: If the string is not yet completely transmitted.

  Example:
    const char *str = "Hello World";
    APP_DATA appData;

    appData.stringPointer = str;

    while(false == PutCharacter());
    


  Remarks:
    This routine must be called from the APP_Tasks function.
*/
bool WriteString(void);

// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************

extern APP_DRV_OBJECTS appDrvObject;

extern APP_DATA appData;


#endif /* _APP_HEADER_H */

/*******************************************************************************
 End of File
 */



