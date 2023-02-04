/*!
    Qpyue
    2022_12_14
*/

#ifndef MAIN_H
#define MAIN_H

#define bootsize    0x5000
#define printime()  printf("[%ld:%d]",sys_s,sys_ms)

#include "gd32c10x.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
// #include "ff.h"
#include "QBSP.h"

extern uint16_t sys_ms;
extern uint32_t sys_s;

void Qdelay_ms(uint32_t t);

#endif /* MAIN_H */
