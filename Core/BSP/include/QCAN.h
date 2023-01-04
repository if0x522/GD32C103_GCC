#ifndef Q_CAN_H
#define Q_CAN_H

/*
    can0 使用PA11 和PA12引脚
    can1 remap到PB5 和PB6
*/

#define CAN0_RX GPIO_PIN_11
#define CAN0_TX GPIO_PIN_12
#define CAN1_RX GPIO_PIN_5
#define CAN1_TX GPIO_PIN_6

#include "QBSP_S.h"
// 内部使用函数
void Qcan1_gpio(void);
void Qcan2_gpio(void);

// 接口函数
Q_s QCAN1_config(void);
Q_s QCAN2_config(void);

#endif
