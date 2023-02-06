#ifndef QTTL_H
#define QTTL_H

#include "QBSP_S.h"

#define TTL_BAUD    (115200)    // ttl 串口波特率
#define ttl_data_len    (60)
#define TTL_NUM     USART2

extern SemaphoreHandle_t TTL_m;

void TTL_Init(void);
void TTL_Write(uint16_t addr,uint8_t len,uint8_t *buf);
void TTL_writer_bytes(uint8_t * ttl_tx_buf,uint8_t len);
void TTL_recevice(uint8_t * buf);
uint8_t TTL_get_num(void);
#endif
