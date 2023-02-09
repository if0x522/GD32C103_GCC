#include "QSD.h"

uint8_t SD_TYPE = 0;

uint8_t SDTGA[] = "Q_SD";

// SPI 初始化
void QSD_SPI_Init(void){
    spi_parameter_struct sdc_init;
    // 相关模块时钟初始化
    rcu_periph_clock_enable(RCU_SPI2);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    // GPIO设定
    rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
    // CLK 和 MOSI （mcu输出模式的引脚）设定为复用推挽模式
    gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,SDC_CLK|SDC_MOSI);
    Q_logi(SDTGA,"GPIOB3 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
    Q_logi(SDTGA,"GPIOB5 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
    // MISO （mcu输入） 设定为浮动输入模式
    gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,SDC_MISO);
    Q_logi(SDTGA,"GPIOB4 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
    // SD spi片选引脚设定推挽输出模式
    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,SDC_CS);
    Q_logi(SDTGA,"GPIOA15 GPIO_MODE_IPU GPIO_OSPEED_50MHZ");
    // 失能sdcard片选
    SDC_DISABLE;
    // 设定SPI
    sdc_init.trans_mode     = SPI_TRANSMODE_FULLDUPLEX; // 设置为全双工模式
    sdc_init.device_mode    = SPI_MASTER;               // 设置为主机模式
    sdc_init.frame_size     = SPI_FRAMESIZE_8BIT;       // 设置为8bit通信模式
    sdc_init.clock_polarity_phase   = SPI_CK_PL_HIGH_PH_2EDGE;  // 设定极性和采样
    sdc_init.nss            = SPI_NSS_SOFT;             // 设置软件使能
    sdc_init.prescale       = SPI_PSC_256;              // 预分频器设置
    sdc_init.endian         = SPI_ENDIAN_MSB;           // 设置大小端模式
    spi_init(SDC_SPI,&sdc_init);   
    // 设定spi crc
    // spi_crc_polynomial_set(SDC_SPI,7); 
    // 开启crc
    //spi_crc_on(SDC_SPI);
    // 使能spi
    spi_enable(SDC_SPI);       
}

// SPI 读写一个字节
uint8_t QSD_SPI_RWbyte(uint8_t txData){
    while(RESET == spi_i2s_flag_get(SDC_SPI,SPI_FLAG_TBE));
    spi_i2s_data_transmit(SDC_SPI,txData);
    while(RESET == spi_i2s_flag_get(SDC_SPI,SPI_FLAG_RBNE));
    return  spi_i2s_data_receive(SDC_SPI);
}
/*
    设定spi速率
    参数：
        0   低速模式
        1   高速模式
*/
void QSD_SPI_SetSpeed(uint8_t BAUD){
   
    // 片选失能
    SDC_DISABLE;
    spi_parameter_struct sdc_init;
    spi_disable(SDC_SPI);
    spi_i2s_deinit(SDC_SPI);   // 复位SPI
    // 设定SPI
    sdc_init.trans_mode     = SPI_TRANSMODE_FULLDUPLEX; // 设置为全双工模式
    sdc_init.device_mode    = SPI_MASTER;               // 设置为主机模式
    sdc_init.frame_size     = SPI_FRAMESIZE_8BIT;       // 设置为8bit通信模式
    sdc_init.clock_polarity_phase   = SPI_CK_PL_HIGH_PH_2EDGE;  // 设定极性和采样
    sdc_init.nss            = SPI_NSS_SOFT;             // 设置软件使能
    //sdc_init.prescale       = SPI_PSC_256;              // 预分频器设置
    sdc_init.endian         = SPI_ENDIAN_MSB; 
    if(BAUD == 1){
        sdc_init.prescale       = SPI_PSC_8;
    }else{
        sdc_init.prescale       = SPI_PSC_256;
    }
    spi_init(SDC_SPI,&sdc_init);   // 重新初始化
    // 使能spi
    spi_enable(SDC_SPI);   
    // 使能片选
    SDC_ENABLE;
}

/*
    向SD卡发送一个命令
    参数：  
        cmd 命令
        arg 参数
        crc 校验值
    返回：
        SD卡的响应
*/ 
uint8_t SD_SendCmd(uint8_t cmd,uint32_t arg,uint8_t crc){
    uint8_t r1 = 0;
    uint8_t retry = 0;
    // 进入临界区，
    taskENTER_CRITICAL();
    SDC_DISABLE;
    QSD_SPI_RWbyte(0xFF);
    SDC_ENABLE;
    if(SD_WaitReady() != 0){
        return 0;
    }
    // 发送
    // 分别写入命令
    QSD_SPI_RWbyte(cmd|0x40);
    QSD_SPI_RWbyte(arg >> 24);
    QSD_SPI_RWbyte(arg >> 16);
    QSD_SPI_RWbyte(arg >> 8);
    QSD_SPI_RWbyte(arg);
    QSD_SPI_RWbyte(crc);
    if(cmd == CMD12){
        QSD_SPI_RWbyte(0xff);
    }
    // 等待相应，或超时退出
    retry = 0x1F;
    do{
        r1 = QSD_SPI_RWbyte(0xFF);
    }while((r1&0x80) && (retry--));
     // 退出临界区
    taskEXIT_CRITICAL();
    // 返回状态值
    return r1;
}


// 初始化SD卡
uint8_t SD_Init(void){
    uint8_t r1 = 0;     // 存放返回值
    uint8_t buf[4];
    uint16_t i;
    // 初始化SPI
    QSD_SPI_Init();
    Q_logi(SDTGA,"SPI init");
    // 配置为低速模式
    QSD_SPI_SetSpeed(0);
    // 发送至少74个脉冲
    for(i = 0;i<10;i++){
        QSD_SPI_RWbyte(0xff);
    }
    // 片选
    SDC_ENABLE;
    // 卡复位
    i = 20;
    do{
        // 进入IDLE状态
        r1 = SD_SendCmd(CMD0,0,0x95);
    }while((r1!=0x01)&&(i--));
    SD_TYPE = 0;
    // 识别卡类型
    if(r1 == 0x01){         // SD V2.0
        if(SD_SendCmd(CMD8,0x1AA,0x87)==1){
            //获取R7 resp的尾随返回值
            for(i = 0;i<4;i++){
                buf[i] = QSD_SPI_RWbyte(0xFF);
            }
            // 卡是否支持2.7~3.6V
            if((buf[2] == 0x01) && (buf[3] == 0xAA)){
                i = 0xFFFE;
                do{
                    // 发送cmd55
                    SD_SendCmd(CMD55,0,0x01);
                    // 发送CMD41
                    r1 = SD_SendCmd(CMD41,0x40000000,0x01);
                }while((r1)&&(i--));
                // 鉴别SD2.0版本
                if((i)&&(SD_SendCmd(CMD58,0,0x01) == 0)){
                    // 得到OCR值
                    for(i = 0;i<4;i++){
                        buf[i] = QSD_SPI_RWbyte(0xFF);
                    }
                    // 检查CCS
                    if(buf[0]&0x40){
                        SD_TYPE = SD_TYPE_V2HC;
                    }else{
                        SD_TYPE = SD_TYPE_V2;
                    }
                }
            }
        }
    }else{      // SD V1 /MMC V3
        // 发送CMD55
        SD_SendCmd(CMD55,0,0x01);
        // 发送CMD41
        r1 = SD_SendCmd(CMD41,0,0x01);
        if(r1<=1){
            SD_TYPE = SD_TYPE_V1;
            i = 0xFFFE;
            // 等待退出IDLE模式
            do{
                // 发送CMD55
                SD_SendCmd(CMD55,0,0x01);
                // 发送CMD41
                r1 = SD_SendCmd(CMD41,0,0x01);
            }while((r1)&&(i--));
        }else{      // MMC 卡不支持CMD55+CMD41识别
            // MMC V3
            SD_TYPE = SD_TYPE_MMC;
            i = 0xFFFE;
            // 等待推出IDLE模式
            do{
                // 发送CMD1
                r1 = SD_SendCmd(CMD1,0,0x01);
            }while((r1)&&(i--));
        }
        // 错误的卡
        if((i == 0)||(SD_SendCmd(CMD16,512,0x01)!=0)){
            SD_TYPE = SD_TYPE_ERR;
        }
    }
    // 配置为高速模式
    QSD_SPI_SetSpeed(1);
    if(SD_TYPE){
        uint32_t size = 0;
        double size_g;
        size = SD_GetSectorCount();
        size_g = ((((double)size)/2)/1024)/1024;
        Q_logi(SDTGA,"SD card init");
        printf("SD card SD card capacity is ");
        echof(size_g,2);
        printf(" G\n");
        return 0;
    }else{
        return 1;
    }
    // 其他错误
    return 0xaa;
}

/*
    等待卡准备好
    返回值：
        0   准备好了
        其他    错误
*/
uint8_t SD_WaitReady(void){
    uint32_t t = 0;
    uint8_t reg;
    for(t = 0;t<0xFFFF;t++){
        reg = QSD_SPI_RWbyte(0xFF);   // 获取返回值
        if(reg == 0xFF){
            break;
        }
    }
    if(t<0xFFFF){
        return 0;
    }else{
        return 1;
    }
}

/*
    等待SD卡回应
    参数：
        response    要得到的回应值
    返回值：
        0   成功
        其他    失败
*/
uint8_t SD_GetResponse(uint8_t Response){
    // 等待次数
    uint16_t count=0xFFFF;
    // 等待得到回应
    while((QSD_SPI_RWbyte(0xFF) != Response)&&count){
        count--;
    }
    if(count == 0){
        return MSD_RESPONSE_FAILURE;    // 失败
    }else{
        return MSD_RESPONSE_NO_ERROR;   // 成功
    }
}

/*
    获取SD卡的总扇区数
    返回值：
        0   取容量出错
        其他    SD卡的容量
    note：
        没扇区的字节数必为512，否则初始化不能通过
*/
uint32_t SD_GetSectorCount(void){
    uint8_t csd[16];
    uint32_t capacity;
    uint8_t n;
    uint16_t csize;
    // 取CSD信息，如果出错返回0
    if(SD_GetCSD(csd)!=0){
        return 0;
    }
    // 如果为SDHC卡，按照下边方式计算
    // V2.00 的卡
    if((csd[0]&0xC0) == 0x40){
        csize = csd[9] + ((uint16_t)csd[8]<<8) + 1;
        // 得到扇区数
        capacity = (uint32_t)csize << 10;
    }else{      // V1.X
        n = (csd[5]&15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
			//得到扇区数 
		capacity= (uint32_t)csize << (n - 9);  
    }
    return capacity;
}

/*
    获取SD卡的CID信息，包括制造商信息
    参数：
        cid_data    缓存区，至少16byte
    返回值：
        0       no_err
        1       err
*/
uint8_t SD_GetCID(uint8_t * cid_data){
    uint8_t r1;
    // 发送CMD10读CID
    r1 = SD_SendCmd(CMD10,0,0x01);
    if(r1==0x00){
        // 接收16个字节的数据
        r1 = SD_RecvData(cid_data,16);
    }
    return r1;
}

/*
    从sd卡读取一个数据包
    参数：
        buf     缓存区
        len     长度
    返回值：
        0       成功
        其他    失败
*/
uint8_t SD_RecvData(uint8_t * buf,uint16_t len){
    // 等待sd卡发回数据起始令牌0xFE
    if(SD_GetResponse(0xFE)){
        return 1;
    }
    // 开始接收数据
    while(len--){
        *buf = QSD_SPI_RWbyte(0xFF);
        buf++;
    }
    // 接收CRC并丢弃
    QSD_SPI_RWbyte(0xFF);
    QSD_SPI_RWbyte(0xFF);
    // 读取成功
    return 0;
}

/*
    获取SD卡的CSD信息，包含容量和速度信息
    参数：
        cid_data（用来存放CID的内存，至少16byte
    返回值：
        0：NO_ERR
        1：错误
*/
uint8_t SD_GetCSD(uint8_t *csd_data){
    uint8_t r1;
    // 发送CMD9，读CSD
    r1 = SD_SendCmd(CMD9,0,0x01);
    if(r1 == 0){
        // 接收16字节数据
        r1 = SD_RecvData(csd_data,16);
    }
    return r1;
}

/*
    从SD卡读数据
    参数：
        buf     缓存区
        sector  目标扇区
        cnt     扇区数量
    返回值：
        0       成功
        其他    失败
*/
uint8_t SD_ReadDisk(uint8_t * buf,uint32_t sector,uint8_t cnt){
    uint8_t r1;
    // 转换为字节地址
    if(SD_TYPE != SD_TYPE_V2HC){
        sector <<= 9;
    }
    if(cnt == 1){
        // 读命令
        r1 = SD_SendCmd(CMD17,sector,0x01);
        // 指令发送成功
        if(r1 == 0){
            // 接收512字节,一个扇区
            r1 = SD_RecvData(buf,512);
        }
    }else{      // 连续读命令
        r1 = SD_SendCmd(CMD18,sector,0x01);
        do{
            // 接收512字节,一个扇区
            r1 = SD_RecvData(buf,512);
            buf += 512;
            cnt--;
        }while((cnt)&&(r1 == 0));
        // 发送停止命令
        SD_SendCmd(CMD12,0,0x01);
    }
    return r1;
}

/*
    向sd卡写入一个数据包的内容 512字节
    参数：
        buf 缓存区
        cmd 指令
    返回值：
        0   成功
        其他    失败
*/
uint8_t SD_SendBlock(uint8_t * buf,uint8_t cmd){
    uint16_t t;
    // 等待准备就绪
    if(SD_WaitReady()){
        return 1;
    }
    QSD_SPI_RWbyte(cmd);
    // 不是结束指令
    if(cmd != 0xFD){
        for(t = 0;t<512;t++){
            QSD_SPI_RWbyte(buf[t]);
        }
        // 忽略crc
        QSD_SPI_RWbyte(0xFF);
        QSD_SPI_RWbyte(0xFF);
        // 接收响应
        t = QSD_SPI_RWbyte(0xFF);
        if((t&0x1F)!=0x05){
            // 响应错误
            return 2;
        }
    }
    // 写入成功
    return 0;
}

/*
    写SD卡
    参数：
        buf     数据缓存区
        sector  起始扇区
        cnt     扇区数
    返回值：
        0   OK
        其他    失败
*/
uint8_t SD_WriteDisk(uint8_t * buf,uint32_t sector,uint8_t cnt){
    uint8_t r1;
    // 转换成字节地址
    if(SD_TYPE!=SD_TYPE_V2HC){
        sector <<= 9;
    }
    if(cnt == 1){
        // 读命令
        r1 = SD_SendCmd(CMD24,sector,0x01);
        // 指令发送成功
        if(r1 == 0){
            // 写512个字节
            r1 = SD_SendBlock(buf,0xFE);
        }
    }else{
        if(SD_TYPE !=SD_TYPE_MMC){
            SD_SendCmd(CMD55,0,0x01);
            // 发送指令
            SD_SendCmd(CMD23,cnt,0x01);
        }
        // 连续写命令
        r1 = SD_SendCmd(CMD25,sector,0x01);
        if(r1 == 0){
            do{
                // 接收512字节
                r1 = SD_SendBlock(buf,0xFC);
                buf+=512;
                cnt--;
            }while((cnt)&&(r1 == 0));
            // 停止接收
            r1 = SD_SendBlock(0,0xFD);
        }
    }
    return r1;
}
