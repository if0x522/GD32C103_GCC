#include "main.h"



// w5500资料https://blog.csdn.net/WANGYONGZIXUE/article/details/121353444
void vmain(void){
  uint8_t seed[4] = {0x12,0x34,0x56,0x78};
  uint8_t key[4] = {0x00};
  // 挂载sd卡
  QF_create();
  printf("dizhi %lx\n",(uint32_t)security);
  // 获取算法
  security_get();
  printf("dizhi_init %lx\n",(uint32_t)security);
	for(;;){
    security(seed,key);
    vTaskDelay(1000);
  }
}