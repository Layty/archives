#include <stdio.h>
#include <string.h>
#include "app.h"
#include "uart_cmd.h"
#include "rf_task.h"
#include "rf_config.h"
#include "sch_task.h"
#include <stdint.h>



/*CONST DEFINE*/
#define MAX_TIMER_COUNT 10000
#define NAME_SIZE 32
#define MAX_EVENT_COUNT 10


/*ERROR NUMBER*/
//register
#define INVAILD_NAME 0x0001


//delete
#define EVENT_NOT_EXIST 0x0001




typedef struct{
    uint16_t active;                                            // the event is active?      1---active          0---non-active
    char name[NAME_SIZE];                                       // event name
    uint16_t period;                                            // the unit is 1s
    uint16_t just_once;                                         // the event just run once   1---just run once    0---run periodly
    void (*handle)(void *input_arg, void *output);              // the function to handle the event
    void *input_arg;
    void *output_arg;
} Event_t;



Event_t events[MAX_EVENT_COUNT];



void sch_delay_us(uint32_t usec) {
    uint64_t t;
    t = RF_TICK()+((usec * 10) / 32);
    while (t > RF_TICK()) {
    }   
}



uint8_t events_init(){
    uint8_t i = 0;
    for(i = 0; i < sizeof(events); i++){
        events[i].active = 0;
    }
    return 0;
}

uint8_t reg_event( char *name, uint16_t period, uint16_t just_once, void (*handle)(void *input_arg, void *output), void *input_arg, void *output_arg){
    uint16_t err_flag = 0;
    uint16_t i = 0;
    for(i = 0; i < MAX_EVENT_COUNT; i++){
        if(events[i].active == 0){
            break;
        }
    }
    if(i < MAX_EVENT_COUNT){
        events[i].active = 1;
        if(strlen(name)<NAME_SIZE-1){
            strcpy(events[i].name, name);
        }else{
            err_flag |= INVAILD_NAME;
        }
        events[i].period = period;
        events[i].just_once = just_once;
        events[i].handle = handle;
        events[i].input_arg = input_arg;
        events[i].output_arg = output_arg;        
    }
    return err_flag;
}

uint8_t del_event( char *name){
    uint16_t err_flag = 0;
    uint16_t i = 0;
    for(i = 0; i < MAX_EVENT_COUNT; i++){
        if(strcmp(name, events[i].name) == 0){
            events[i].active = 0;
        }
    }
    if(i > MAX_EVENT_COUNT){
        err_flag |= EVENT_NOT_EXIST;
    }
    return err_flag;
}

static uint16_t timer_count = 0;

//static uint64_t a = 0;

void SCH_Tasks(void){
    uint16_t i = 0;
    timer_count++;
#if 1
    for(i = 0; i < MAX_EVENT_COUNT; i++){      // sizeof(events)/sizeof(Event_t)
        if(events[i].period && events[i].active && timer_count % events[i].period == 0){               
            events[i].handle(events[i].input_arg, NULL);        
        }
    } 
#endif
    if(timer_count == MAX_TIMER_COUNT){
        timer_count = 0;
    }
}