/*******Filename: hik_tset.h *******************************/
/*******Brief:  hikvision test interface *******************/
/*******Author: fenglilei@hikvison.com.cn *****************/
/*******Data: 2017-4-11 10:37:54 ***************************/

#ifndef HIK_TEST_H
#define HIK_TEST_H

/********TEST OPTION*******/
#define HIK_DEBUG       1
#define HIK_DEBUG_TX    1
#define HIK_DEBUG_RX    1




//#define CONST_RF_PAYLOAD_SET 0
//#define RF_PAYLOAD_ACK 1


//define test type
#define TEST_NON 0
#define TEST_TX 1
#define TEST_RX 2


#define TICK_UNIT 3.2                               // 1 TICK = 3.2us

typedef struct {
    uint64_t first_start_tick;
    uint64_t tick_start;
    uint64_t tick_end; 
    uint64_t bytes;
} record_t;

typedef struct{
    record_t record;
    uint8_t test_type;
}hik_test_t;


uint64_t start_time;

uint8_t init_test(hik_test_t *test);

uint8_t record_tick(uint64_t *tick_start);

double cal_speed(hik_test_t test, int bytes);

void hik_tx_packet_limit(uint8_t ch, uint8_t limit, uint8_t pkt_length);

void hik_test_ack(uint8_t mode);

void hik_tx_packet_limit_ack(uint8_t ch, uint8_t limit, uint8_t pkt_length);

void Hik_initFSKReg();


#endif