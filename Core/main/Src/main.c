#include "main.h"

// printf函数输出缓冲区
uint8_t printbuf[64];
#define bootsize    0x5000
extern void vmain(void);    // 根任务函数

void root_task(void * arg){
    printf("start root stask\n");
    vmain();
    vTaskDelete(NULL);
}

int main(void)
{
    // 重映射中断向量表
    // SCB->VTOR = 0x08000000|bootsize;
    // jump2app=(void(*)())*(vu32*)(appxaddr+4);
    dma_parameter_struct dma_init_struct;
   // 初始化串口，定位printf
	/* 使能相关时钟 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(RCU_DMA0);
    /* 配置串口引脚 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    /* USART配置 */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
    // 串口使能
    // 配置dma
    dma_deinit(DMA0,DMA_CH3);   // 初始化通道
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)printbuf;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init_struct.number = 0;
    dma_init(DMA0,DMA_CH3,&dma_init_struct);
    dma_circulation_disable(DMA0, DMA_CH3);
    dma_memory_to_memory_disable(DMA0, DMA_CH3);
    // 开启串口dma传输
    usart_dma_transmit_config(USART0,USART_DENT_ENABLE);
    dma_channel_enable(DMA0,DMA_CH3);
    printf("Initialization completen\n");
	// 开始任务调度
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
	xTaskCreate(root_task,"root_task",4069,NULL,configMAX_PRIORITIES,NULL);
	vTaskStartScheduler();
	// 不会运行到这里
    while(1)
	{
		;;
	}
}

int _write(int fd,char *ptr,int len){
	/*for(int i = 0;i<len;i++){
		usart_data_transmit(USART0, (uint8_t)ptr[i]);
		while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
	}
    */
    int i = 0;
    /*等待DMA通道传输完成*/
    while(dma_flag_get(DMA0,DMA_CH3,DMA_FLAG_FTF) == RESET){
        ;
    }
    dma_channel_disable(DMA0,DMA_CH3);
    // 将数据写入缓冲区
    for(i = 0;i<len;i++){
        printbuf[i] = (uint8_t)ptr[i];
    }
    dma_transfer_number_config(DMA0,DMA_CH3,len);
    dma_channel_enable(DMA0,DMA_CH3);
    return len;
}
// 栈溢出钩子函数
void vApplicationStackOverflowHook( TaskHandle_t xTask,char * pcTaskName ){
	printf("task : %s stack error\n task %s delete\n",pcTaskName,pcTaskName);
    vTaskDelete(xTask);
}
