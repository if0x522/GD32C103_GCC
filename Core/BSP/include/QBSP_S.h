#ifndef QBSP_S_H
#define QBSP_S_H
#include<stdio.h>

#define Q_logi(tga,log) printf("%s:\t%s\n",tga,log)
// 通用状态
typedef enum{
    Q_ERR = 0,    
    Q_OK = 1,     
}Q_s;


#endif
