/*!
    Qpyue
    2022_12_14
*/

#ifndef MAIN_H
#define MAIN_H

#define bootsize    0x5000
#define printime()  printf("[%ld:%d]",sys_s,sys_ms)
#define QF_create() {FATFS QFATFS;f_mount(&QFATFS,"",1);}       // 挂载SD卡文件系统

#include "gd32c10x.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "ff.h"
#include "QBSP.h"
#include "userconf.h"


#endif /* MAIN_H */
