#include "main.h"

FIL File_user;          // 文件对象

void vmain(void){
    uint8_t bus[16];
    UINT s;
    QF_create();
    f_open(&File_user,"/app.bin",FA_READ);
	for(;;){
        f_read(&File_user,bus,16,&s);
        printmem(bus,16,'-');
        printf("\n");
        vTaskDelay(500);
  }
}