#ifndef Q_LIN_H
#define Q_LIN_H

#include "QBSP_S.h"

#define LIN1_UART_NUM	(UART3)
#define LIN2_UART_NUM	(UART4)
#define LIN_BAUD		(19200)


// LIN 报文结构体
typedef struct {
	uint8_t uiID;
	uint8_t uiDLC;
	uint8_t * uiData;
}LIN_msg_t;

// 初始化LIN1
void LIN1_Init(void);
// LIN1 发送数据报文
void LIN1_SendMsg(LIN_msg_t LINm);
// LIN1 接收数据报文
Q_s LIN1_ReceiveMsg(LIN_msg_t LINm,uint16_t timeo);

// 初始化LIN1
void LIN2_Init(void);
// LIN1 发送数据报文
void LIN2_SendMsg(LIN_msg_t LINm);
// LIN1 接收数据报文
Q_s LIN2_ReceiveMsg(LIN_msg_t LINm,uint16_t timeo);

// 计算check sum
uint8_t LIN_CheckSum(LIN_msg_t LINm);

#endif