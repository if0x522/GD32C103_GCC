#include "main.h"


void vmain(void){

    for(;;){
		printime();
		printf("task a\n");
		vTaskDelay(500);
		printf("task b\n");
		Qdelay_ms(500);
	}
}