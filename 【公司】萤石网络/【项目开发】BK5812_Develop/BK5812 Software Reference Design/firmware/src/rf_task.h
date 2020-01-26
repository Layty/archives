/* 
 * File:   rf_task.h
 * Author: Administrator
 *
 * Created on 2016?12?10?, ?? 10:47
 */

#ifndef RF_TASK_H
#define	RF_TASK_H

extern uint32_t sendDataLen;
extern uint32_t rfTempData;
extern uint32_t rfGetData;
extern uint32_t rfGetError;

extern uint64_t rfTestTick;

void rf_task_init(uint8_t type, uint8_t rate);
void rf_task(void);

#endif	/* RF_TASK_H */

