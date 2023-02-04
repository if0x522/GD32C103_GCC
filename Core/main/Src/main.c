#include "main.h"
// 
 extern void vmain(void);    // 根任务函数
 
 void root_task(void * arg){
     printf("start root stask\n");
     vmain();
     vTaskDelete(NULL);
 }

// 初始化串口
void USART_print(void){
   // 初始化串口，定位printf
	/* 使能相关时钟 */
    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    gpio_pin_remap_config(GPIO_USART0_REMAP,ENABLE);
    /* 配置串口引脚 */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    /* USART配置 */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}

// 配置系统时钟，精准定时
void timer_config(void){
    timer_parameter_struct timer_initpara;
    // 使能时钟
    rcu_periph_clock_enable(RCU_TIMER1);
    // 重置外设及结构体
    timer_deinit(TIMER1);
    timer_struct_para_init(&timer_initpara);
    // 结构体设置中断周期为1ms
    timer_initpara.prescaler            = 119;
    timer_initpara.alignedmode          = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection     = TIMER_COUNTER_UP;
    timer_initpara.period               = 999;
    timer_initpara.clockdivision        = TIMER_CKDIV_DIV1;
    // 初始化时钟
    timer_init(TIMER1,&timer_initpara);
    // 使能中断
    timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER1,TIMER_INT_UP);
    // 使能时钟
    timer_enable(TIMER1);
    // 设定优先级
    nvic_irq_enable(TIMER1_IRQn,1,0);
}

//延时函数1ms 阻塞式，不可长时间阻塞系统
void Qdelay_ms(uint32_t t){
    uint16_t ms_o;
    uint32_t s_o;
    ms_o = t%1000;
    s_o = (uint32_t)t/1000;
    s_o = s_o + sys_s;
    ms_o = ms_o + sys_ms;
    if(ms_o >= 1000){
        s_o++;
        ms_o = ms_o - 1000;
    }
    while(s_o > sys_s){
        ;
    }
    while(ms_o > sys_ms){
        ;
    }
}

int main(void){
    // 设定中断优先级为4位抢占式
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
    // 初始化串口
    USART_print();

    printf("Qpyue system start!\n");

    // 配置系统时钟，精准定时
    timer_config();
    printime();
    printf("system time start\n");

	// 开始任务调度
	
	xTaskCreate(root_task,"root_task",1024,NULL,configMAX_PRIORITIES,NULL);
	vTaskStartScheduler();
	// 不会运行到这里
    SystemInit();
}
// 重定位标准输出---正式核心板
 int _write(int fd,char *ptr,int len){
    // 进入临界区
    taskENTER_CRITICAL();
    for(int i = 0;i<len;i++){
 		usart_data_transmit(USART0, (uint8_t)ptr[i]);
 		while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
 	}
    // 退出临界区
    taskEXIT_CRITICAL();
    return len;
}

// 栈溢出钩子函数
void vApplicationStackOverflowHook( TaskHandle_t xTask,char * pcTaskName ){
	printf("%ld:%dtask : %s stack error. delete\n",sys_s,sys_ms,pcTaskName);
    vTaskDelete(xTask);
}
