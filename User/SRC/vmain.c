#include "main.h"

#define DEV_CAN_BAUD_RATE 500000

void can1send(void * arg){
	int i = 0;
	can_msg_t(can1);
	can1.tx_sfid = 0x751;
	can1.tx_dlen = 8;
	can1.tx_ff = CAN_FF_STANDARD;
	can1.fd_flag = 1;
	for( i = 0;i<8;i++){
		can1.tx_data[i] = 0x23+i;
	}
	printf("can1 send msg\n");
	for(;;){
		QCAN1_transmit(can1);
		printf("send \n");
		vTaskDelay(500);
	}
}
void can2send(void * arg){
	int i = 0;
	can_msg_t(can2);
	can2.tx_sfid = 0x754;
	can2.tx_dlen = 8;
	can2.tx_ff = CAN_FF_STANDARD;
	can2.fd_flag = 1;
	for( i = 0;i<8;i++){
		can2.tx_data[i] = 0xa3+i;
	}
	printf("can2 send msg\n");
	for(;;){
		QCAN2_transmit(can2);
		vTaskDelay(500);
	}
}

void vmain(void){
	can_msg_r(can1);
	QCAN1_FD_STANDARD_conf();
	QCAN2_FD_STANDARD_conf();
	xTaskCreate(can1send,"can1send",256,NULL,1,NULL);
	xTaskCreate(can2send,"can2send",256,NULL,1,NULL);
	printf("can listen \n");
    for(;;){
		if(QCAN1_receive(&can1,3) == Q_OK){
			printf("can1 receive msg id%lx\tdlc%d\tdata1%x\n",can1.rx_sfid,can1.rx_dlen,can1.rx_data[0]);
		}
		 if(QCAN2_receive(&can1,3) == Q_OK){
		 	printf("can1 receive msg id%lx\tdlc%d\tdata1%x\n",can1.rx_sfid,can1.rx_dlen,can1.rx_data[0]);
		}
		vTaskDelay(20);
	}
}