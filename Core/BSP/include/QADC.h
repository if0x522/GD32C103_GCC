#ifndef Q_ADC_H
#define Q_ADC_H

#define QADC0   GPIO_PIN_0      // GPIOC0
#define QADC1   GPIO_PIN_1      // GPIOC1
#define QADC2   GPIO_PIN_2      // GPIOC2
#define QADC3   GPIO_PIN_3      // GPIOC3
#define QADC4   GPIO_PIN_0      // GPIOA0
#define QADC5   GPIO_PIN_1      // GPIOA1
#define QADC6   GPIO_PIN_2      // GPIOA2
#define QADC7   GPIO_PIN_3      // GPIOA3
#define QADC8   GPIO_PIN_4      // GPIOA4
#define QADC9   GPIO_PIN_5      // GPIOA5
#define QADC10   GPIO_PIN_6      // GPIOA6
#define QADC11   GPIO_PIN_7      // GPIOA7
#define QADC12   GPIO_PIN_4      // GPIOC4
#define QADC13   GPIO_PIN_5      // GPIOC5
#define QADC14   GPIO_PIN_0      // GPIOB0
#define QADC15   GPIO_PIN_1      // GPIOB1




// 获取IO脚实际电压值
#define vref(d)	(uint16_t)(d * 3.3/4096 * 1000)
// 结果转换位实际电压值
#define actual(v)	(uint16_t)(v * 23.3/3.3)

#include"QBSP_S.h"

// 初始化ADC
void QADC_Init(void);

// 获取特定通道数值
uint16_t QADC_GET(uint8_t channel);

// 获取特定通道电压值
uint16_t QADC_GET_Vol(uint8_t channel);

#endif
