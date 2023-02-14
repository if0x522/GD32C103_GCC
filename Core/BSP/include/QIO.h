#ifndef QIO_H
#define QIO_H

#include"QBSP_S.h"

#define RELAYP_LOC GPIO_PIN_6  // GPIOC6
#define RELAYP_DA  GPIO_PIN_15 // GPIOB15
#define RELAYP_SCK GPIO_PIN_14 // GPIOB14

#define SWP_LOC     GPIO_PIN_12 // GPIOA12
#define SWP_DA      GPIO_PIN_11 // GPIOA11
#define SWP_CLK     GPIO_PIN_10 // GPIOA10

#define RELAY_LOC(x)  gpio_bit_write(GPIOC,RELAYP_LOC,x)
#define RELAY_DA(x)   gpio_bit_write(GPIOB,RELAYP_DA,x)
#define RELAY_SCK(x)  gpio_bit_write(GPIOB,RELAYP_SCK,x)

#define SW_LOC(x)   gpio_bit_write(GPIOA,SWP_LOC,x)
#define SW_CLK(x)   gpio_bit_write(GPIOA,SWP_CLK,x)
#define SW_DA()     gpio_input_bit_get(GPIOA,SWP_DA)

// relay数据长度 单位byte
#define QRELAY_L  2
// GPIO输入长度 单位byte
#define QSW_L   1

// RELAY 初始化
void QRELAY_Init(void);
// 设置输出状态
void QRELAY_setall(uint8_t flg,uint8_t sta);

// 开关输入初始化
void QSW_Init(void);
// 获取开关状态
void QSW_Updata(void);
// 获取某一个开关状态
uint8_t QSW_GET(uint8_t num);

#endif
