/*
    SD卡外设驱动
    20230206
    zhen
*/
#ifndef Q_SD_H
#define Q_SD_H

#include"QBSP_S.h"

#define SDC_MOSI    GPIO_PIN_5
#define SDC_MISO    GPIO_PIN_4
#define SDC_CLK     GPIO_PIN_3
#define SDC_CS      GPIO_PIN_15
#define SDC_SPI     SPI2

// 片选使能
#define SDC_ENABLE  gpio_bit_reset(GPIOA,SDC_CS)
// 片选失能
#define SDC_DISABLE gpio_bit_set(GPIOA,SDC_CS)

// SDCARD 软件相关
// SD 卡类型定义
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06


// SD卡指令表  	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ，SEND_IF_COND
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define CMD23   23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define CMD41   41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00


//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

//SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

// SPI 初始化
void QSD_SPI_Init(void);
// SPI 读写一个字节
uint8_t QSD_SPI_RWbyte(uint8_t txData);
// SPI速度设置
void QSD_SPI_SetSpeed(uint8_t BAUD);

uint8_t SD_SendCmd(uint8_t cmd,uint32_t arg,uint8_t crc);
uint8_t SD_Init(void);
uint8_t SD_WaitReady(void);
uint8_t SD_GetResponse(uint8_t Response);
uint32_t SD_GetSectorCount(void);
uint8_t SD_GetCID(uint8_t *cid_data);
uint8_t SD_RecvData(uint8_t * buf,uint16_t len);
uint8_t SD_GetCSD(uint8_t *csd_data);
uint8_t SD_ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);
uint8_t SD_SendBlock(uint8_t * buf,uint8_t cmd);
uint8_t SD_WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);

#endif

