#include"QIO.h"


// 板卡使用2byte，外部有扩展则在加
uint8_t Qrelay[QRELAY_L] = {0x00,0x00};
uint8_t Qswitch[QSW_L] = {0x00};

// RELAY 初始化
void QRELAY_Init(void){
  // 初始化时钟
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_GPIOC);
  // 初始化GPIO
  gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_2MHZ,RELAYP_DA|RELAYP_SCK);
  gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_2MHZ,RELAYP_LOC);
  // 所有引脚拉低
  RELAY_LOC(0);
  RELAY_SCK(0);
  RELAY_DA(0);
  // 将relay设置为全零
  QRELAY_setall(0,0);
}

// 设置输出状态
void QRELAY_setall(uint8_t flg,uint8_t sta){
  int i = 0;
  int j = 0;
  // 判断受否需要重置所有位
  if(flg){
    for(i=0;i<QRELAY_L;i++){
      Qrelay[i] = sta;
    }
  }
  // 开始移位输出
  for(i = 0;i<QRELAY_L;i++){
    for(j = 0;j<8;j++){
      if((1<<j)&Qrelay[i]){
        RELAY_DA(1);
      }else{
        RELAY_DA(0);
      }
      RELAY_SCK(1);
      RELAY_SCK(0);
    }
  }
  // 移位数据使能
  RELAY_LOC(1);
  RELAY_LOC(0);
}

// 开关输入初始化
void QSW_Init(void){
    // 初始化时钟
  rcu_periph_clock_enable(RCU_GPIOA);
  // 初始化GPIO
  gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_2MHZ,SWP_LOC|SWP_CLK);
  gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_2MHZ,SWP_DA);
  // 所有引脚拉低
  SW_LOC(1);
  SW_CLK(1);
}

// 获取开关状态
void QSW_Updata(void){
    int i,j;
    // 读入状态
    SW_CLK(1);
    SW_LOC(0);
    //vTaskDelay(1);
    // 准备读入
    SW_LOC(1);
    for(i = 0;i<QSW_L;i++){
        for(j = 0;j<8;j++){
            SW_CLK(1);
            SW_CLK(0);
            if(SW_DA()){
                Qswitch[i] = Qswitch[i]|(1<<j);
            }else{
                Qswitch[i] = Qswitch[i]&(uint8_t)(~(1<<j));
            }
        }
    }
}

// 获取某一个开关状态
uint8_t QSW_GET(uint8_t num){
    QSW_Updata();
    return (Qswitch[(uint8_t)num/8])&(1<<(num%8));
}
