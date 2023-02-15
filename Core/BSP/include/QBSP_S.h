#ifndef QBSP_S_H
#define QBSP_S_H

#include<stdio.h>
#include<stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "gd32c10x.h"
#include "userconf.h"

extern uint16_t sys_ms;
extern uint32_t sys_s;

void Qdelay_ms(uint32_t t);

#define Q_logi(tga,log) printf("[%ld:%d]%s:\t%s\n",sys_s,sys_ms,tga,log)
// 通用状态
typedef enum{
    Q_ERR = 0,    
    Q_OK = 1,   
    Q_ERR_TimeOut = 2,
    Q_ERR_CheckSum = 3,  
}Q_s;

// 打印一段地址
void printmem(uint8_t * addr,uint8_t len,char s);
// 打印小数
void echof(double ff,uint8_t n);
// 比较两个字符串是否相同
uint8_t check_str(uint8_t * s1,uint8_t * s2,uint8_t len);

#define Q_printmem(addr,len)   printmem(addr,len,'\t')



#endif
