/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "sdcard.h"
/* Definitions of physical drive number for each drive */
// #define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
// #define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
// #define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define SDC	0

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
){
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
){
	DSTATUS stat;
	switch (pdrv) {
	case SDC :
		stat = SD_Init();	// 初始化SD卡
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* 驱动编号 */
	BYTE *buff,		/* 数据缓存区 */
	LBA_t sector,	/* 扇区地址 */
	UINT count		/* 需要读取的扇区数 */
){
	DRESULT res;
	switch (pdrv) {
	case SDC :
		res = SD_ReadDisk((uint8_t *)buff,sector,count);
		return res;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* 驱动编号 */ 
	const BYTE *buff,	/* 数据缓存区 */
	LBA_t sector,		/* 扇区地址 */
	UINT count			/* 需要读取的扇区数*/
){
	DRESULT res;
	switch (pdrv) {
	case SDC :
		res = SD_WriteDisk((uint8_t *)buff,sector,count);
		return res;
	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/*  物理驱动号 */
	BYTE cmd,		/* 控制码 */
	void *buff		/* 发送/接收数据缓冲区地址 */
){
	DRESULT res = 0;

	switch (pdrv) {
	case SDC :
		switch (cmd)
		{
		case CTRL_SYNC:		// 等待写过程
			res = SD_WaitReady();
			break;
		default:
			break;
		}
		return res;
	}
	return RES_PARERR;
}

