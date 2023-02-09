#include "QBSP_S.h"

// 打印某段内存 addr 内存首地址，len打印长度，s 间隔符
void printmem(uint8_t * addr,uint8_t len,char s){
    for(int i = 0;i<len;i++){
        printf("%x%c",addr[i],s);
    }
}

// 打印一个小数，
// ff 需要打印的数  n 保留小数位数
void echof(double ff,uint8_t n){
    char ffs[16] = {0x00};
    uint8_t m;  // 记录最大位数
    int tmp;
    uint8_t num;
    tmp = (int)ff;
    m = 0;
    // 提取整数部分，反向储存在字符串后半段
    while(tmp>0){
        ffs[8+m] = (tmp%10)|0x30;
        tmp = (int)tmp/10;
        m++;
    }
    num = 0;
    // 将整数部分规整
    if(m == 0){     // 若没有整数部分，写0
        ffs[0] = '0';
        num = 1;
    }else{
        for(;m>0;){
            m--;
            ffs[num++] = ffs[8+m];
        }
    }
    // 加入小数点
    ffs[num++] = 0x2E;
    // 计算小数部分
    for(m = 0;m<n;m++){
        ff *= 10;
        ffs[num++] = (((int)ff)%10)|0x30;
    }
    // 将转换结果打印

    printf("%s",ffs);
}
