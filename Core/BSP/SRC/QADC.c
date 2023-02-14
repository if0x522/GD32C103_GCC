#include"QADC.h"

// 板卡ADC通道与芯片通道对应关系
uint8_t QADC_channel[16] = {
    10,     // GPIOC0   ADC_IN10,
    11,     // GPIOC1   ADC_IN11,
    12,     // GPIOC2   ADC_IN12,
    13,     // GPIOC3   ADC_IN13,
    0,      // GPIOA0   ADC_IN0,
    1,      // GPIOA1   ADC_IN1,
    2,      // GPIOA2   ADC_IN2, 
    3,      // GPIOA3   ADC_IN3, 
    4,      // GPIOA4   ADC_IN4, 
    5,      // GPIOA5   ADC_IN5, 
    6,      // GPIOA6   ADC_IN6, 
    7,      // GPIOA7   ADC_IN7, 
    14,     // GPIOC4   ADC_IN14,
    15,     // GPIOC5   ADC_IN15,
    8,      // GPIOB0   ADC_IN8, 
    9,      // GPIOB1   ADC_IN9, 
};

// 初始化ADC
void QADC_Init(void){
    // 配置时钟
  rcu_periph_clock_enable(RCU_ADC0);
  rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV16);
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
  rcu_periph_clock_enable(RCU_GPIOC);
  // 配置gpio
  gpio_init(GPIOA,GPIO_MODE_AIN,GPIO_OSPEED_2MHZ,QADC4|QADC5|QADC6|QADC7|QADC8|QADC9|QADC10|QADC11);
  gpio_init(GPIOB,GPIO_MODE_AIN,GPIO_OSPEED_2MHZ,QADC14|QADC15);
  gpio_init(GPIOC,GPIO_MODE_AIN,GPIO_OSPEED_2MHZ,QADC0|QADC1|QADC2|QADC3|QADC12|QADC13);
  // 配置ADC同步模式，所有ADC运行于独立模式
  adc_mode_config(ADC_MODE_FREE);
  // 配置采样率
  adc_resolution_config(ADC0,ADC_RESOLUTION_12B);
  // 配置ADC数据对齐方式，右对齐
  adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
  // 配置规则组深度为1
  adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1U);
  // 设置软件触发模式
  adc_external_trigger_source_config(ADC0,ADC_REGULAR_CHANNEL,ADC0_1_EXTTRIG_REGULAR_NONE);
  // 使能触发模式
  adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
  // 使能ADC0
  adc_enable(ADC0);
  vTaskDelay(1);
  // 校准ADC
  adc_calibration_enable(ADC0);
}

// 获取特定通道测量数值
uint16_t QADC_GET(uint8_t channel){
    // 设置规则组通道
  adc_regular_channel_config(ADC0,0U,QADC_channel[channel],ADC_SAMPLETIME_71POINT5);
  // 启动软件触发
  adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);
  // 等待测量完成
  while(!adc_flag_get(ADC0,ADC_FLAG_EOC));
  // 清除标志位
  adc_flag_clear(ADC0,ADC_FLAG_EOC);
  // 返回测量值
  return adc_regular_data_read(ADC0);
}

// 获取特定通道电压值
uint16_t QADC_GET_Vol(uint8_t channel){
    return actual(vref(QADC_GET(channel)));
}

