#include"main.h"

// 定义security函数指针

security_f security = NULL;

// 获取函数代码
void security_get(void){
    uint16_t i;
    uint32_t start_add;
    i = read_sa_offset();
    start_add = (uint32_t)(seedtokey + i);
    security = (security_f)(start_add);
}
