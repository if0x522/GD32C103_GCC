#ifndef Q_CAN_H
#define Q_CAN_H


#define DEV_CAN1_BAUD_RATE    500000
#define DEV_CANFD1_BAUD_RATE  2000000
#define DEV_CAN2_BAUD_RATE    500000
#define DEV_CANFD2_BAUD_RATE  2000000

#define STANDARD    1
#define EXTENDED    0

#include "QBSP_S.h"
#include "gd32c10x_can.h"
#include "userconf.h"
// 内部使用函数
void Qcan1_gpio(void);
void Qcan2_gpio(void);
Q_s QCAN1_config(uint8_t ff,uint8_t fd);
Q_s QCAN2_config(uint8_t ff,uint8_t fd);

// 接口函数
// 声明一个发送用can消息
#define can_msg_t(msg)     can_trasnmit_message_struct msg;  \
                            can_struct_para_init(CAN_TX_MESSAGE_STRUCT,&msg)
// 声明一个接收用can消息
#define can_msg_r(msg)     can_receive_message_struct  msg;    \
                            can_struct_para_init(CAN_RX_MESSAGE_STRUCT,&msg)

// 初始化CAN1接收标准帧模式，不启动CANfd
#define QCAN1_STANDARD_conf()    QCAN1_config(STANDARD,DISABLE)
//  初始化CAN1接收扩展帧模式，不启动CANfd
#define QCAN1_EXTENDED_conf()    QCAN1_config(EXTENDED,DISABLE)
// 初始化CAN1接收标准帧模式，启动CANfd
#define QCAN1_FD_STANDARD_conf()    QCAN1_config(STANDARD,ENABLE)
//  初始化CAN1接收扩展帧模式，启动CANfd
#define QCAN1_FD_EXTENDED_conf()    QCAN1_config(EXTENDED,ENABLE)
// 初始化CAN1接收标准帧模式，不启动CANfd
#define QCAN1_Init()    QCAN1_config(STANDARD,DISABLE)

// 初始化CAN2接收标准帧模式，不启动CANfd
#define QCAN2_STANDARD_conf()    QCAN2_config(STANDARD,DISABLE)
//  初始化CAN2接收扩展帧模式，不启动CANfd
#define QCAN2_EXTENDED_conf()    QCAN2_config(EXTENDED,DISABLE)
// 初始化CAN2接收标准帧模式，启动CANfd
#define QCAN2_FD_STANDARD_conf()    QCAN2_config(STANDARD,ENABLE)
//  初始化CAN2接收扩展帧模式，启动CANfd
#define QCAN2_FD_EXTENDED_conf()    QCAN2_config(EXTENDED,ENABLE)
// 初始化CAN2接收标准帧模式，不启动CANfd
#define QCAN2_Init()    QCAN2_config(STANDARD,DISABLE)

// can1发送消息
#define QCAN1_transmit(can_msg)   can_message_transmit(CAN0,&can_msg)
// can2发送消息
#define QCAN2_transmit(can_msg)   can_message_transmit(CAN1,&can_msg)
// can1接收消息
Q_s QCAN1_receive(can_receive_message_struct * msg,uint32_t timeout);
// can2接收消息
Q_s QCAN2_receive(can_receive_message_struct * msg,uint32_t timeout);
#endif
