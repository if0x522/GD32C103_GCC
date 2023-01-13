#include "QCAN.h"
#include "main.h"

char TGA[] = "QCAN";

void Qcan1_gpio(void){
    // 使能相关时钟
    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    // 设定gpio
	gpio_init(GPIOB,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ,GPIO_PIN_8);
    Q_logi(TGA,"GPIOB8 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
	gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);
    Q_logi(TGA,"GPIOB9 GPIO_MODE_AF_PP GPIO_OSPEED_50MHZ");
    // can0引脚重映射
	gpio_pin_remap_config(GPIO_CAN0_PARTIAL_REMAP,ENABLE);
}
void Qcan2_gpio(void){
    /*// 使能相关时钟
    rcu_periph_clock_enable(RCU_CAN1);
    rcu_periph_clock_enable(RCU_GPIOB);
    // 设定gpio
	gpio_init(GPIOB,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ,GPIO_PIN_12);
    Q_logi(TGA,"GPIOB12 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
	gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_13);
    Q_logi(TGA,"GPIOB13 GPIO_MODE_AF_PP GPIO_OSPEED_50MHZ");
    // can0引脚重映射
	// gpio_pin_remap_config(GPIO_CAN1_PARTIAL_REMAP,ENABLE);
    */
    // 使能相关时钟
    rcu_periph_clock_enable(RCU_CAN1);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    // 设定gpio
	gpio_init(GPIOB,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ,GPIO_PIN_5);
    Q_logi(TGA,"GPIOB5 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
	gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    Q_logi(TGA,"GPIOB6 GPIO_MODE_AF_PP GPIO_OSPEED_50MHZ");
    // can0引脚重映射
	gpio_pin_remap_config(GPIO_CAN1_REMAP,ENABLE);
}

// 接口函数
Q_s QCAN1_config(uint8_t ff,uint8_t fd){
    Q_s flg = Q_ERR;
    can_parameter_struct can1_parameter;    // can配置结构体
    Q_logi(TGA,"can1 config start!");
    Qcan1_gpio();
    Q_logi(TGA,"can1 GPIO config OK!");
    can_struct_para_init(CAN_INIT_STRUCT,&can1_parameter);  // CAN外设库使用到的各类结构体初始化
    // 复位can外设
    can_deinit(CAN0);
    Q_logi(TGA,"can1 deinit");
    // 设定配置项
    can1_parameter.time_triggered = DISABLE;
    can1_parameter.auto_bus_off_recovery = DISABLE;
    can1_parameter.auto_wake_up = DISABLE;
    can1_parameter.auto_retrans = DISABLE;
    can1_parameter.rec_fifo_overwrite = DISABLE;
    can1_parameter.trans_fifo_order = DISABLE;
    can1_parameter.working_mode = CAN_NORMAL_MODE;  
    if(can_init(CAN0,&can1_parameter) == SUCCESS){
        flg = Q_OK;
        Q_logi(TGA,"can1 init ok");
    }else{
        flg = Q_ERR;
        Q_logi(TGA,"can1 init error");
    }
    can_frequency_set(CAN0, DEV_CAN1_BAUD_RATE);
    Q_logi(TGA,"can1 baud set");
    if(ff == EXTENDED){
        can_filter_mask_mode_init(CAN1_REC_EFID, 0xFFFFFFFF, CAN_EXTENDED_FIFO0, 2);
    }else if(ff == STANDARD){
        can_filter_mask_mode_init(CAN1_REC_SFID, 0xFFFFFFFF, CAN_STANDARD_FIFO0, 2);
    }
    if(fd != DISABLE){
        can_fdframe_struct can_fd_parameter;	// canfd 传输延迟补偿配置结构体
        can_fd_tdc_struct can_fd_tdc_parameter;	// canfd功能设置结构体
        can_struct_para_init(CAN_FD_FRAME_STRUCT, &can_fd_parameter);
        can_fd_parameter.fd_frame = ENABLE;
        can_fd_parameter.excp_event_detect = ENABLE;
        can_fd_parameter.delay_compensation = ENABLE;
        can_fd_tdc_parameter.tdc_filter = 0x04; 
        can_fd_tdc_parameter.tdc_mode = CAN_TDCMOD_CALC_AND_OFFSET;
        can_fd_tdc_parameter.tdc_offset = 0x04;
        can_fd_parameter.p_delay_compensation = &can_fd_tdc_parameter;
        can_fd_parameter.iso_bosch = CAN_FDMOD_ISO;
        can_fd_parameter.esi_mode = CAN_ESIMOD_HARDWARE;
        if(can_fd_init(CAN0, &can_fd_parameter) == SUCCESS){
            flg = Q_OK;
            Q_logi(TGA,"can1 fd init ok");
        }else{
            flg = Q_ERR;
            Q_logi(TGA,"can1 fd init error");
        }
        can_fd_frequency_set(CAN0, DEV_CANFD1_BAUD_RATE);
        Q_logi(TGA,"can1 fd baud set");
    }
    return flg;
}

Q_s QCAN2_config(uint8_t ff,uint8_t fd){
    Q_s flg = Q_ERR;
    can_parameter_struct can2_parameter;    // can配置结构体
    Q_logi(TGA,"can2 config start!");
    Qcan2_gpio();
    Q_logi(TGA,"can2 GPIO config OK!");
    can_struct_para_init(CAN_INIT_STRUCT,&can2_parameter);  // CAN外设库使用到的各类结构体初始化
    // 复位can外设
    can_deinit(CAN1);
    Q_logi(TGA,"can2 deinit");
    // 设定配置项
    can2_parameter.time_triggered = DISABLE;
    can2_parameter.auto_bus_off_recovery = DISABLE;
    can2_parameter.auto_wake_up = DISABLE;
    can2_parameter.auto_retrans = DISABLE;
    can2_parameter.rec_fifo_overwrite = DISABLE;
    can2_parameter.trans_fifo_order = DISABLE;
    can2_parameter.working_mode = CAN_NORMAL_MODE;  
    if(can_init(CAN1,&can2_parameter) == SUCCESS){
        flg = Q_OK;
        Q_logi(TGA,"can2 init ok");
    }else{
        flg = Q_ERR;
        Q_logi(TGA,"can2 init error");
    }
    can_frequency_set(CAN1, DEV_CAN2_BAUD_RATE);
    Q_logi(TGA,"can2 baud set");
    if(ff == EXTENDED){
        can_filter_mask_mode_init(CAN2_REC_EFID, 0xFFFFFFFF, CAN_EXTENDED_FIFO1, 15);
    }else if(ff == STANDARD){
        can_filter_mask_mode_init(CAN2_REC_SFID, 0xFFFFFFFF, CAN_STANDARD_FIFO1, 15);
    }
    if(fd != DISABLE){
        can_fdframe_struct can_fd_parameter;	// canfd 传输延迟补偿配置结构体
        can_fd_tdc_struct can_fd_tdc_parameter;	// canfd功能设置结构体
        can_struct_para_init(CAN_FD_FRAME_STRUCT, &can_fd_parameter);
        can_fd_parameter.fd_frame = ENABLE;
        can_fd_parameter.excp_event_detect = ENABLE;
        can_fd_parameter.delay_compensation = ENABLE;
        can_fd_tdc_parameter.tdc_filter = 0x04; 
        can_fd_tdc_parameter.tdc_mode = CAN_TDCMOD_CALC_AND_OFFSET;
        can_fd_tdc_parameter.tdc_offset = 0x04;
        can_fd_parameter.p_delay_compensation = &can_fd_tdc_parameter;
        can_fd_parameter.iso_bosch = CAN_FDMOD_ISO;
        can_fd_parameter.esi_mode = CAN_ESIMOD_HARDWARE;
        if(can_fd_init(CAN1, &can_fd_parameter) == SUCCESS){
            flg = Q_OK;
            Q_logi(TGA,"can2 fd init ok");
        }else{
            flg = Q_ERR;
            Q_logi(TGA,"can2 fd init error");
        }
        can_fd_frequency_set(CAN1, DEV_CANFD1_BAUD_RATE);
        Q_logi(TGA,"can2 fd baud set");
    }
    return flg;
}

// can1接收消息
Q_s QCAN1_receive(can_receive_message_struct * msg,uint32_t timeout){
    Q_s flg = Q_ERR;
    uint8_t i;
    uint32_t tt;
    tt = timeout;
    // 检查接收邮箱是否为空
    i = can_receive_message_length_get(CAN0,CAN_FIFO0);
    if(i > 0){
        flg = Q_ERR;
    }else{
        flg = Q_OK;
    }
    // 若没收到报文则进入等待
    while(flg){
        vTaskDelay(1);
        i = can_receive_message_length_get(CAN0,CAN_FIFO0);
        tt--;
        if(i > 0){
            flg = Q_ERR;
        }
        if(tt == 0){
            // 超时返回错误
            return Q_ERR;
        }
    }
    // 接收邮箱有报文接收并返回
    can_message_receive(CAN0,CAN_FIFO0,msg);
    return Q_OK;
}
// can2接收消息
Q_s QCAN2_receive(can_receive_message_struct * msg,uint32_t timeout){
    Q_s flg = Q_ERR;
    uint8_t i;
    uint32_t tt;
    tt = timeout;
    // 检查接收邮箱是否为空
    i = can_receive_message_length_get(CAN1,CAN_FIFO1);
    if(i > 0){
        flg = Q_ERR;
    }else{
        flg = Q_OK;
    }
    // 若没收到报文则进入等待
    while(flg){
        vTaskDelay(1);
        i = can_receive_message_length_get(CAN1,CAN_FIFO1);
        tt--;
        if(i > 0){
            flg = Q_ERR;
        }
        if(tt == 0){
            // 超时返回错误
            return Q_ERR;
        }
    }
    // 接收邮箱有报文接收并返回
    can_message_receive(CAN1,CAN_FIFO1,msg);
    return Q_OK;
}
