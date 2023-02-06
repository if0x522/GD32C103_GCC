#include "main.h"

void vmain(void){
  uint32_t size;
  SD_Init();
  printf("SD card Init \n");
  size = SD_GetSectorCount();
  printf("SD size : %ld * 512",size);
	for(;;){
    ;
  }
}