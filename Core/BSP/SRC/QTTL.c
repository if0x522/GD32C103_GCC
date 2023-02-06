#include "QTTL.h"

uint8_t ttl_rx_buf[64];
uint8_t ttl_rx_size = 0;

SemaphoreHandle_t TTL_m;

const char TTLTGA[] = "Q_TTL";

// 初始化TTL
void TTL_Init(void){
    // 创建消息
    TTL_m = xSemaphoreCreateBinary();
     // 进入临界区，
    taskENTER_CRITICAL();
    // 配置时钟
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_USART2);
	// 数据引脚设定
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    Q_logi(TTLTGA,"GPIOB10 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	Q_logi(TTLTGA,"GPIOB11 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
    // UART配置
    usart_deinit(TTL_NUM);
    usart_baudrate_set(TTL_NUM, TTL_BAUD);
    usart_receive_config(TTL_NUM, USART_RECEIVE_ENABLE);
    usart_transmit_config(TTL_NUM, USART_TRANSMIT_ENABLE);
    usart_enable(TTL_NUM);
	// 配置中断
    nvic_irq_enable(USART2_IRQn,15,0);   // 中断优先级12
    usart_interrupt_enable(TTL_NUM,USART_INT_RBNE);
    // 退出临界区
    taskEXIT_CRITICAL();
}

// TTL 发送bytes
void TTL_writer_bytes(uint8_t * ttl_tx_buf,uint8_t len){
    for(int i = 0;i<len;i++){
        usart_data_transmit(USART2, ttl_tx_buf[i]);
 		while(RESET == usart_flag_get(USART2,USART_FLAG_TC));
    }
}

// TTL 写数据
void TTL_Write(uint16_t addr,uint8_t len,uint8_t *buf){
    uint8_t ttl_tx_buf[ttl_data_len] = {0};
    ttl_tx_buf[0] = 0x5A;
    ttl_tx_buf[1] = 0xA5;
    ttl_tx_buf[2] = (len+3);
    ttl_tx_buf[3] = 0x82;
    ttl_tx_buf[4] = (uint8_t)(addr>>8);
    ttl_tx_buf[5] = (uint8_t)addr;
    for(int i = 0;i<len;i++){
        ttl_tx_buf[6+i] = buf[i];
    }
    TTL_writer_bytes(ttl_tx_buf,(len+6));
}

// TTL接收数据
void TTL_recevice(uint8_t * buf){
    for(int i = 0;i<ttl_rx_size;i++){
        buf[i] = ttl_rx_buf[i];
    }
    ttl_rx_size = 0;    // 清除接收计数
}

// 获取ttl是否有数据
uint8_t TTL_get_num(void){
    return ttl_rx_size;
}

// 中断接收
void USART2_IRQHandler(void){
    if(RESET != usart_interrupt_flag_get(TTL_NUM, USART_INT_FLAG_RBNE)){
        /* receive data */
        xSemaphoreGiveFromISR(TTL_m,pdFALSE);
        ttl_rx_buf[ttl_rx_size++] = usart_data_receive(USART2);
    }
}


