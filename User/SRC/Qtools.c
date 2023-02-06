#include "main.h"

// 打印某段内存 addr 内存首地址，len打印长度，s 间隔符
void printmem(uint8_t * addr,uint8_t len,char s){
    for(int i = 0;i<len;i++){
        printf("%x%c",addr[i],s);
    }
}
