#include "main.h"

void task1(void * arg){
	int i = 0;
	for(;;){
		printf("task1 : %d\n",i++);
		vTaskDelay(600);
	}
}

void task2(void * arg){
	int i = 0;
	for(;;){
		printf("task2 : %d\n",i++);
		vTaskDelay(200);
	}
}

void vmain(void){
    rcu_periph_clock_enable(RCU_GPIOC); //打开GPIOC时钟
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13); //将PC13配置为推挽输出，最高支持50MHz
	gpio_bit_reset(GPIOC, GPIO_PIN_13); //将PC13置低电平，点亮LED
	xTaskCreate(task1,"task1",128,NULL,0,NULL);
	xTaskCreate(task2,"task2",128,NULL,0,NULL);
    for(;;){
		gpio_bit_reset(GPIOC, GPIO_PIN_13); //将PC13置低电平，点亮LED
		vTaskDelay(500);
		gpio_bit_set(GPIOC, GPIO_PIN_13); //将PC13置高电平，熄灭LED
		vTaskDelay(500);
	}
}