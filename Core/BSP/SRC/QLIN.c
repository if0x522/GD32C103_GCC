#include "QLIN.h"

char LINTGA[] = "QLIN";

const unsigned char cucPARITY_ID[] ={
    /*  00  */  0x80,
    /*  01  */  0xC1,
    /*  02  */  0x42,
    /*  03  */  0x03,
    /*  04  */  0xC4,
    /*  05  */  0x85,
    /*  06  */  0x06,
    /*  07  */  0x47,
    /*  08  */  0x08,
    /*  09  */  0x49,
    /*  0A  */  0xCA,
    /*  0B  */  0x8B,
    /*  0C  */  0x4C,
    /*  0D  */  0x0D,
    /*  0E  */  0x8E,
    /*  0F  */  0xCF,
    /*  10  */  0x50,
    /*  11  */  0x11,
    /*  12  */  0x92,
    /*  13  */  0xD3,
    /*  14  */  0x14,
    /*  15  */  0x55,
    /*  16  */  0xD6,
    /*  17  */  0x97,
    /*  18  */  0xD8,
    /*  19  */  0x99,
    /*  1A  */  0x1A,
    /*  1B  */  0x5B,
    /*  1C  */  0x9C,
    /*  1D  */  0xDD,
    /*  1E  */  0x5E,
    /*  1F  */  0x1F,
    /*  20  */  0x20,
    /*  21  */  0x61,
    /*  22  */  0xE2,
    /*  23  */  0xA3,
    /*  24  */  0x64,
    /*  25  */  0x25,
    /*  26  */  0xA6,
    /*  27  */  0xE7,
    /*  28  */  0xA8,
    /*  29  */  0xE9,
    /*  2A  */  0x6A,
    /*  2B  */  0x2B,
    /*  2C  */  0xEC,
    /*  2D  */  0xAD,
    /*  2E  */  0x2E,
    /*  2F  */  0x6F,
    /*  30  */  0xF0,
    /*  31  */  0xB1,
    /*  32  */  0x32,
    /*  33  */  0x73,
    /*  34  */  0xB4,
    /*  35  */  0xF5,
    /*  36  */  0x76,
    /*  37  */  0x37,
    /*  38  */  0x78,
    /*  39  */  0x39,
    /*  3A  */  0xBA,
    /*  3B  */  0xFB,
    /*  3C  */  0x3C,
    /*  3D  */  0x7D,
    /*  3E  */  0xFE,
    /*  3F  */  0xBF

};

// 初始化LIN1
void LIN1_Init(void){
     // 进入临界区，
    taskENTER_CRITICAL();
    // 配置时钟
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_UART3);
	// 数据引脚设定
	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    Q_logi(LINTGA,"GPIOC10 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	Q_logi(LINTGA,"GPIOC11 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
    // UART配置
    usart_deinit(LIN1_UART_NUM);
    usart_baudrate_set(LIN1_UART_NUM, LIN_BAUD);
    usart_receive_config(LIN1_UART_NUM, USART_RECEIVE_ENABLE);
    usart_transmit_config(LIN1_UART_NUM, USART_TRANSMIT_ENABLE);
	usart_lin_mode_enable(LIN1_UART_NUM);
    usart_enable(LIN1_UART_NUM);
	// LIN1使能引脚
	gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_2MHZ,GPIO_PIN_8);
	Q_logi(LINTGA,"GPIOC8 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
    gpio_bit_set(GPIOC,GPIO_PIN_8);
    Q_logi(LINTGA,"LIN1 ENABLE");
    // 退出临界区
    taskEXIT_CRITICAL();
}

// LIN1 发送数据报文
void LIN1_SendMsg(LIN_msg_t LINm){
	uint8_t i = 0;
	uint8_t synch = 0x55;
    // 进入临界区，
    taskENTER_CRITICAL();
	// 发送间隔场 Break
    while(usart_flag_get(LIN1_UART_NUM,USART_FLAG_TC) == RESET);    // 等待传输完成
	usart_send_break(LIN1_UART_NUM);
	// 发送同步场 Synch
	while(usart_flag_get(LIN1_UART_NUM,USART_FLAG_TC) == RESET);    // 等待传输完成
	usart_data_transmit(LIN1_UART_NUM,synch);
	// 发送PID PID
	while(usart_flag_get(LIN1_UART_NUM,USART_FLAG_TC) == RESET);    // 等待传输完成
	usart_data_transmit(LIN1_UART_NUM,cucPARITY_ID[LINm.uiID]);
	// 发送数据场 DATA
	for(i = 0;i<LINm.uiDLC;i++){
		while(usart_flag_get(LIN1_UART_NUM,USART_FLAG_TC) == RESET);    // 等待传输完成
	    usart_data_transmit(LIN1_UART_NUM,LINm.uiData[i]);
	}
	// 发送校验和场 CHECK_SUM
	while(usart_flag_get(LIN1_UART_NUM,USART_FLAG_TC) == RESET);    // 等待传输完成
	usart_data_transmit(LIN1_UART_NUM,LIN_CheckSum(LINm));
    while(usart_flag_get(LIN1_UART_NUM,USART_FLAG_TC) == RESET);    // 等待传输完成
	// 退出临界区
    taskEXIT_CRITICAL();
}

// LIN1 接收数据报文
Q_s LIN1_ReceiveMsg(LIN_msg_t LINm,uint16_t timeo){
    uint8_t uctemp;
    uint8_t i;
    uint8_t synch = 0x55;
    uint16_t ms_o;
    uint32_t s_o;
    ms_o = timeo%1000;
    s_o = (uint32_t)timeo/1000;
    s_o = s_o + sys_s;
    ms_o = ms_o + sys_ms;
    if(ms_o >= 1000){
        s_o++;
        ms_o = ms_o - 1000;
    }
    // 进入临界区，
    taskENTER_CRITICAL();
	// 发送间隔场 Break
	while(usart_flag_get(LIN1_UART_NUM,USART_FLAG_TC) == RESET);    // 等待传输完成
	usart_send_break(LIN1_UART_NUM);
	// 发送同步场 Synch
	while(usart_flag_get(LIN1_UART_NUM,USART_FLAG_TC) == RESET);    // 等待传输完成
	usart_data_transmit(LIN1_UART_NUM,synch);
    // 发送PID PID
	while(usart_flag_get(LIN1_UART_NUM,USART_FLAG_TC) == RESET);    // 等待传输完成
	usart_data_transmit(LIN1_UART_NUM,cucPARITY_ID[LINm.uiID]);
    // 等待发送完成
    while(usart_flag_get(LIN1_UART_NUM,USART_FLAG_TC) == RESET);
    usart_flag_clear(LIN1_UART_NUM,USART_FLAG_RBNE);
    // 开始接收数据
    while((s_o > sys_s)||(ms_o > sys_ms)){
        if(usart_flag_get(LIN1_UART_NUM,USART_FLAG_RBNE) == SET){
            if(i<LINm.uiDLC){
                LINm.uiData[i] = usart_data_receive(LIN1_UART_NUM);
            }else{
                uctemp = usart_data_receive(LIN1_UART_NUM);
                break;
            }
            i++;
            ms_o = timeo%1000;
            s_o = (uint32_t)timeo/1000;
            s_o = s_o + sys_s;
            ms_o = ms_o + sys_ms;
            if(ms_o >= 1000){
                s_o++;
                ms_o = ms_o - 1000;
            }
        }
    }
    // 接收完成判断数据完整性
    if(i < LINm.uiDLC){
        // 退出临界区
        taskEXIT_CRITICAL();
        return Q_ERR_TimeOut;
    }
    // 检查校验和
    if(uctemp != LIN_CheckSum(LINm)){
        // 退出临界区
        taskEXIT_CRITICAL();
        return Q_ERR_CheckSum;
    }
    // 退出临界区
    taskEXIT_CRITICAL();
    return Q_OK;
}

// 计算LIN帧校验和
uint8_t LIN_CheckSum(LIN_msg_t LINm){
	uint8_t i;
	uint16_t sum;
	uint8_t carry;
	uint8_t checksum;
	if((LINm.uiID == 0x3C)||(LINm.uiID == 0x3D)){
		checksum = 0;
	}else{
		checksum = cucPARITY_ID[LINm.uiID];
	}
	for(i = 0;i<LINm.uiDLC;i++){
		sum = (uint16_t)checksum + (uint16_t)LINm.uiData[i];
		if(sum > 0xFF){
			carry = 1;
		}else{
			carry = 0;
		}
		checksum = (uint8_t)sum + carry;
	}
	checksum = 0xFF - checksum;
	return checksum;
}
