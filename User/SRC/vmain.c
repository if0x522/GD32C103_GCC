#include "main.h"


void vmain(void){
  QSW_Init();
	for(;;){
    QSW_Updata();
    vTaskDelay(1000);
  }
}