/* 
 * File:   rf_app.h
 * Author: Administrator
 *
 * Created on 2016?12?8?, ?? 9:38
 */

#ifndef RF_APP_H
#define	RF_APP_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    INSTANDBY = 0, //
    UNLINK, //
    LINKED, //
    PAIRING, //
} _rf_mode;    
    
uint8_t rfTaskBuf[16384];
    
void init_rf_data_fifo(void);

uint16_t get_rf_data_fifo_space(void);
void write_rf_data_fifo(uint8_t* data, uint16_t len);
uint16_t get_rf_data_fifo_length(void);
void read_rf_data_fifo(uint8_t* data, uint16_t len);

void rf_device_serch(void);
void rf_application(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RF_APP_H */

