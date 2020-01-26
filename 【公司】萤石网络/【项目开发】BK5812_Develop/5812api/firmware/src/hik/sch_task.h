

#ifndef _EXAMPLE_FILE_NAME_H    
#define _EXAMPLE_FILE_NAME_H

void sch_delay_us(uint32_t usec);

uint8_t events_init();

uint8_t reg_event( char *name, uint16_t period, uint16_t just_once, void (*handle)(void *input_arg, void *output), void *input_arg, void *output_arg);

uint8_t del_event( char *name);

void SCH_Tasks();

#endif 
