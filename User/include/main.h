/*!
    Qpyue
    2022_12_14
*/

#ifndef MAIN_H
#define MAIN_H

#define bootsize    0x5000
#define printime()  printf("[%ld:%d]",sys_s,sys_ms)
#define Q_printmem(addr,len)   printmem(addr,len,'\t')

#include "gd32c10x.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
// #include "ff.h"
#include "QBSP.h"
#include "userconf.h"

void printmem(uint8_t * addr,uint8_t len,char s);

#endif /* MAIN_H */
