/*---------------------------------------------------------------------------/
/  FatFs模块的配置
/---------------------------------------------------------------------------*/

#include "QBSP_S.h"

#define FFCONF_DEF	80286	/* 修订ID */

/*---------------------------------------------------------------------------/
/ 功能配置
/---------------------------------------------------------------------------*/

#define FF_FS_READONLY	0
/* 此选项切换只读配置。（0：读/写或1：只读）
/   只读配置删除了编写API函数f_write（）、f_sync（）、，
/   f_nunlink（）、f_mkdir（）、f-chmod（）
/   以及可选的书写功能  */
  
  
  


#define FF_FS_MINIMIZE	0
/* 此选项定义最小化级别以删除一些基本API函数。
/
/   0：基本功能已完全启用。
/   1:f_stat（）、f_getfree（）、f-unlink（）、fs_mkdir（）、f=truncate（）和f_reame（）
/  被移除。
/   2: f_opendir(), f_readdir() and f_closedir() are removed in addition to 1.
/   3: f_lseek() function is removed in addition to 2. */


#define FF_USE_FIND		0
/* 此选项切换过滤的目录读取函数f_findfirst（）和
/  f_findnext（）。（0:禁用，1:启用2:也使用匹配的altname[]启用） */


#define FF_USE_MKFS		1
/* 此选项切换f_mkfs（）函数。（0：禁用或1：启用） */


#define FF_USE_FASTSEEK	1
/*此选项切换快速寻道功能。（0：禁用或1：启用）*/


#define FF_USE_EXPAND	0
/* 此选项切换f_expand函数。（0：禁用或1：启用） */


#define FF_USE_CHMOD	0
/* This option switches attribute manipulation functions, f_chmod() and f_utime().
/  (0:Disable or 1:Enable) Also FF_FS_READONLY needs to be 0 to enable this option. */


#define FF_USE_LABEL	0
/*此选项切换卷标函数f_getlabel（）和f_setlabel（）.
/  (0:Disable or 1:Enable) */


#define FF_USE_FORWARD	0
/* This option switches f_forward() function. (0:Disable or 1:Enable) */


#define FF_USE_STRFUNC	1
#define FF_PRINT_LLI	1
#define FF_PRINT_FLOAT	1
#define FF_STRF_ENCODE	3
/* FF_USE_STRFUNC switches string functions, f_gets(), f_putc(), f_puts() and
/  f_printf().
/
/   0: Disable. FF_PRINT_LLI, FF_PRINT_FLOAT and FF_STRF_ENCODE have no effect.
/   1: Enable without LF-CRLF conversion.
/   2: Enable with LF-CRLF conversion.
/
/  FF_PRINT_LLI = 1 makes f_printf() support long long argument and FF_PRINT_FLOAT = 1/2
/  makes f_printf() support floating point argument. These features want C99 or later.
/  When FF_LFN_UNICODE >= 1 with LFN enabled, string functions convert the character
/  encoding in it. FF_STRF_ENCODE selects assumption of character encoding ON THE FILE
/  to be read/written via those functions.
/
/   0: ANSI/OEM in current CP
/   1: Unicode in UTF-16LE
/   2: Unicode in UTF-16BE
/   3: Unicode in UTF-8
*/


/*---------------------------------------------------------------------------/
/ Locale and Namespace Configurations
/---------------------------------------------------------------------------*/

#define FF_CODE_PAGE	936
/* This option specifies the OEM code page to be used on the target system.
/  Incorrect code page setting can cause a file open failure.
/
/   437 - U.S.
/   720 - Arabic
/   737 - Greek
/   771 - KBL
/   775 - Baltic
/   850 - Latin 1
/   852 - Latin 2
/   855 - Cyrillic
/   857 - Turkish
/   860 - Portuguese
/   861 - Icelandic
/   862 - Hebrew
/   863 - Canadian French
/   864 - Arabic
/   865 - Nordic
/   866 - Russian
/   869 - Greek 2
/   932 - Japanese (DBCS)
/   936 - Simplified Chinese (DBCS)
/   949 - Korean (DBCS)
/   950 - Traditional Chinese (DBCS)
/     0 - Include all code pages above and configured by f_setcp()
*/


#define FF_USE_LFN		0
#define FF_MAX_LFN		255
/* The FF_USE_LFN switches the support for LFN (long file name).
/
/   0: Disable LFN. FF_MAX_LFN has no effect.
/   1: Enable LFN with static  working buffer on the BSS. Always NOT thread-safe.
/   2: Enable LFN with dynamic working buffer on the STACK.
/   3: Enable LFN with dynamic working buffer on the HEAP.
/
/  To enable the LFN, ffunicode.c needs to be added to the project. The LFN function
/  requiers certain internal working buffer occupies (FF_MAX_LFN + 1) * 2 bytes and
/  additional (FF_MAX_LFN + 44) / 15 * 32 bytes when exFAT is enabled.
/  The FF_MAX_LFN defines size of the working buffer in UTF-16 code unit and it can
/  be in range of 12 to 255. It is recommended to be set it 255 to fully support LFN
/  specification.
/  When use stack for the working buffer, take care on stack overflow. When use heap
/  memory for the working buffer, memory management functions, ff_memalloc() and
/  ff_memfree() exemplified in ffsystem.c, need to be added to the project. */


#define FF_LFN_UNICODE	2
/* This option switches the character encoding on the API when LFN is enabled.
/
/   0: ANSI/OEM in current CP (TCHAR = char)
/   1: Unicode in UTF-16 (TCHAR = WCHAR)
/   2: Unicode in UTF-8 (TCHAR = char)
/   3: Unicode in UTF-32 (TCHAR = DWORD)
/
/  Also behavior of string I/O functions will be affected by this option.
/  When LFN is not enabled, this option has no effect. */


#define FF_LFN_BUF		255
#define FF_SFN_BUF		12
/* 这组选项定义FILINFO结构中文件名成员的大小
/其用于读出目录项。这些值应足以
/要读取的文件名。读取文件名的最大可能长度取决于
/关于字符编码。当未启用LFN时，这些选项无效。*/


#define FF_FS_RPATH		0
/* This option configures support for relative path.
/
/   0: Disable relative path and remove related functions.
/   1: Enable relative path. f_chdir() and f_chdrive() are available.
/   2: f_getcwd() function is available in addition to 1.
*/


/*---------------------------------------------------------------------------/
/ Drive/Volume Configurations
/---------------------------------------------------------------------------*/

#define FF_VOLUMES		1
/* Number of volumes (logical drives) to be used. (1-10) */


#define FF_STR_VOLUME_ID	0
#define FF_VOLUME_STRS		"RAM","NAND","CF","SD","SD2","USB","USB2","USB3"
/* FF_STR_VOLUME_ID switches support for volume ID in arbitrary strings.
/  When FF_STR_VOLUME_ID is set to 1 or 2, arbitrary strings can be used as drive
/  number in the path name. FF_VOLUME_STRS defines the volume ID strings for each
/  logical drives. Number of items must not be less than FF_VOLUMES. Valid
/  characters for the volume ID strings are A-Z, a-z and 0-9, however, they are
/  compared in case-insensitive. If FF_STR_VOLUME_ID >= 1 and FF_VOLUME_STRS is
/  not defined, a user defined volume string table is needed as:
/
/  const char* VolumeStr[FF_VOLUMES] = {"ram","flash","sd","usb",...
*/


#define FF_MULTI_PARTITION	0
/* This option switches support for multiple volumes on the physical drive.
/  By default (0), each logical drive number is bound to the same physical drive
/  number and only an FAT volume found on the physical drive will be mounted.
/  When this function is enabled (1), each logical drive number can be bound to
/  arbitrary physical drive and partition listed in the VolToPart[]. Also f_fdisk()
/  function will be available. */


#define FF_MIN_SS		512
#define FF_MAX_SS		512
/* This set of options configures the range of sector size to be supported. (512,
/  1024, 2048 or 4096) Always set both 512 for most systems, generic memory card and
/  harddisk, but a larger value may be required for on-board flash memory and some
/  type of optical media. When FF_MAX_SS is larger than FF_MIN_SS, FatFs is configured
/  for variable sector size mode and disk_ioctl() function needs to implement
/  GET_SECTOR_SIZE command. */


#define FF_LBA64		0
/* 此选项切换对64位LBA的支持。（0：禁用或1：启用）
/要启用64位LBA，还需要启用exFAT。（FF_FS_EXFAT==1） */


#define FF_MIN_GPT		0x10000000
/* 将GPT切换为分区格式的最小扇区数（f_mkfs和
/f_fdisk函数。0x100000000 max.当FF_LBA64时，此选项无效==0. */


#define FF_USE_TRIM		0
/* 此选项切换对ATA-TRIM的支持。（0：禁用或1：启用）
/要启用微调功能，还应向
/disk_ioctl（）函数. */



/*---------------------------------------------------------------------------/
/ System Configurations
/---------------------------------------------------------------------------*/

#define FF_FS_TINY		0
/* 此选项切换微型缓冲区配置。（0：正常或1：微小）
/在微小的配置中，文件对象（FIL）的大小被压缩为FF_MAX_SS字节。
/不是从文件对象中删除私有扇区缓冲区，而是公共扇区
/文件系统对象（FATFS）中的缓冲区用于文件数据传输。 */


#define FF_FS_EXFAT		0
/*此选项切换对exFAT文件系统的支持。（0：禁用或1：启用）
/要启用exFAT，还需要启用LFN。（FF_USE_LFN>=1）
/注意，启用exFAT会丢弃ANSI C（C89）兼容性。*/


#define FF_FS_NORTC		1
#define FF_NORTC_MON	1
#define FF_NORTC_MDAY	1
#define FF_NORTC_YEAR	2022
/*选项FF_FS_NORTC切换时间戳功能。如果系统没有
/不需要RTC或有效时间戳，请设置FF_FS_NORTC=1以禁用
/时间戳功能。FatFs修改的每个对象都有固定的时间戳
/由当地时间中的FF_NORTC_MON、FF_NORTC_MDAY和FF_NORTC_YEAR定义。
/要启用时间戳函数（FF_FS_NORTC=0），get_fattime（）函数需要
/添加到项目以从实时时钟读取当前时间。FF_ NORTC_，
/FF_NORTC_MDAY和FF_NORTC_YEAR没有影响。
/这些选项在只读配置中无效（FF_FS_READONLY=1）. */


#define FF_FS_NOFSINFO	0
/* 如果您需要知道FAT32卷上正确的可用空间，请将其设置为0位
/选项，并且在卷装载后的第一次执行f_getfree（）函数将强制
/完整的FAT扫描。位1控制最后分配的簇号的使用。
/
/bit0=0：如果可用，在FSINFO中使用空闲集群计数。
/bit0=1:不信任FSINFO中的空闲群集计数。
/bit1=0：使用FSINFO中最后分配的集群号（如果可用）。
/bit1=1：不信任FSINFO中最后分配的群集号。
*/


#define FF_FS_LOCK		0
/* 选项FF_FS_LOCK切换文件锁定功能以控制复制文件打开
/以及非法操作打开对象。当FF_FS_READONLY时，此选项必须为0
/为1。
/
/0：禁用文件锁定功能。为了避免卷损坏，应用程序
/应避免非法打开、删除和重命名打开的对象。
/>0：启用文件锁定功能。该值定义了多少文件/子目录
/可以在文件锁定控制下同时打开。请注意，文件
/锁定控制与再入无关*/


#define FF_FS_REENTRANT	0
#define FF_FS_TIMEOUT	1000
/* 选项FF_FS_REENTRANT切换FatF的重入（线程安全）
/模块本身。请注意，无论此选项如何
/音量始终是可重入的，音量控制函数f_mount（）、f_mkfs（）
/和f_fdisk（）函数始终不可重入。仅文件/目录访问
/到相同的体积是在这个特征的控制下。
/
/0：禁用重入。FF_FS_TIMEOUT无效。
/1：启用重新进入。此外，用户提供的同步处理器，
/ff_mutex_create（）、ff_mutex-delete（）、ff_mutex_take（）和ff_mutex.give（）
/函数，必须添加到项目中。示例可在ffsystem.c中获得。
/
/FF_FS_TIMEOUT以O/S时间刻度为单位定义超时时间。
*/



/*--- End of configuration options ---*/
