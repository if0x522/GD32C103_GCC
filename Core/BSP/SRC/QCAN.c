#include "QCAN.h"
#include "gd32c10x.h"

char TGA[] = "QCAN";

void Qcan1_gpio(void){
    // 使能相关时钟
    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    // 设定CAN0GPIO
    gpio_init(GPIOA,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ,CAN0_RX);
    Q_logi(TGA,"GPIOA11 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
    gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,CAN0_TX);
    Q_logi(TGA,"GPIOA12 GPIO_MODE_AF_PP GPIO_OSPEED_50MHZ");
}
void Qcan2_gpio(void){
    ;
}

// 接口函数
Q_s QCAN1_config(void){
    Q_s flg = Q_ERR;
    can_parameter_struct can1_parameter;
    Q_logi(TGA,"can1 config start!");
    Qcan1_gpio();
    Q_logi(TGA,"can1 GPIO config OK!");
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
    // 波特率设定   总线时钟/(7*(14+8+3)) = 500K
    can1_parameter.resync_jump_width = CAN_BT_SJW_3TQ;
    can1_parameter.time_segment_1 = CAN_BT_BS1_14TQ;
    can1_parameter.time_segment_2 = CAN_BT_BS2_8TQ;
    can1_parameter.prescaler = 7;
    if(can_init(CAN0,&can1_parameter) == SUCCESS){
        flg = Q_OK;
        Q_logi(TGA,"can1 init ok");
    }else{
        flg = Q_ERR;
        Q_logi(TGA,"can1 init error");
    }
    return flg;
}
Q_s QCAN2_config(void){
    ;
    return Q_OK;
}