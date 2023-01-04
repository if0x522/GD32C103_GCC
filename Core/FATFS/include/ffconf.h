/*---------------------------------------------------------------------------/
/  FatFs Functional Configurations
/---------------------------------------------------------------------------*/

#define FFCONF_DEF	86631	/* 版本识别 */

/*---------------------------------------------------------------------------/
/ 功能配置
/---------------------------------------------------------------------------*/

#define FF_FS_READONLY	0
/* 这个选项开关只读配置。(0:读/写或1:只读) 　　

/只读配置删除编写API函数,f_write(),f_sync(), 　　

/ f_unlink(),f_mkdir(),f_chmod(),f_rename(),f_truncate(),f_getfree() 　　

/写和可选的功能. */


#define FF_FS_MINIMIZE	0
/*此选项定义删除一些基本的API函数极小化水平。　　

/ 　　

/ 0:所有基本功能都是激活的。　　

/ 1:f_stat(),f_getfree(),f_unlink(),f_mkdir(),f_chmod(),f_utime(), 　　

/ f_truncate()和f_rename()函数删除。　　

/ 2:f_opendir(),f_readdir()和f_closedir()中除了1。　　

/ 3:f_lseek()函数删除除了2。*/

#define FF_USE_FIND		0
/*此选项切换过滤的目录读取函数f_findfirst（）和
/f_findnext（）。（0:禁用，1:启用2:也使用匹配的altname[]启用）*/

#define FF_USE_MKFS		0
/*此选项切换f_mkfs（）函数。（0：禁用或1：启用）*/

#define FF_USE_FASTSEEK	0
/*此选项切换快速寻道功能。（0：禁用或1：启用）*/

#define FF_USE_EXPAND	0
/*此选项切换f_expand函数。（0：禁用或1：启用）*/

#define FF_USE_CHMOD	0
/*此选项切换属性操纵函数f_chmod（）和f_utile（）。
/（0：禁用或1：启用）此外，FF_FS_READONLY需要为0才能启用此选项*/

#define FF_USE_LABEL	0
/*此选项切换卷标函数f_getlabel（）和f_setlabel（）。
/（0：禁用或1：启用）*/

#define FF_USE_FORWARD	0
/*此选项切换f_forward（）函数。（0：禁用或1：启用）*/

#define FF_USE_STRFUNC	0
#define FF_PRINT_LLI	0
#define FF_PRINT_FLOAT	0
#define FF_STRF_ENCODE	0
/*FF_USE_STRFUNC切换字符串函数、f_gets（）、f_putc（）和f_puts（）
/f_printf（）。
/
/0：禁用。FF_PRINT_LLI、FF_PRINT_FLOAT和FF_STRF_ENCODE没有影响。
/1：在不进行LF-CRLF转换的情况下启用。
/2：通过LF-CRLF转换启用。
/
/FF_PRINT_LLI=1使f_printf（）支持长参数，FF_PRINT_FLOAT=1/2
使f_printf（）支持浮点参数。这些功能需要C99或更高版本。
/当FF_LFN_UNICODE>=1且启用LFN时，字符串函数转换字符
/在其中编码。FF_STRF_ENCODE选择文件上的字符编码假设
/通过这些功能进行读/写。
/
/0:当前CP中的ANSI/OEM
/1:UTF-16LE中的Unicode
/2:UTF-16BE中的Unicode
/3:UTF-8中的Unicode
*/


/*---------------------------------------------------------------------------/
/ 区域设置和命名空间配置
/---------------------------------------------------------------------------*/

#define FF_CODE_PAGE	936
/*此选项指定要在目标系统上使用的OEM代码页。
/错误的代码页设置可能导致文件打开失败。
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


#define FF_USE_LFN		3
#define FF_MAX_LFN		255
/*FF_USE_LFN切换对LFN（长文件名）的支持。
/
/0：禁用LFN。FF_MAX_LFN没有影响。
/1：在BSS上启用具有静态工作缓冲区的LFN。始终不是线程安全的。
/2：在STACK上启用具有动态工作缓冲区的LFN。
/3：在HEAP上启用具有动态工作缓冲区的LFN。
/
/要启用LFN，请使用ffunicode。c需要添加到项目中。LFN函数
/要求某些内部工作缓冲区占用（FF_MAX_LFN+1）*2字节
/启用exFAT时，额外（FF_MAX_LFN+44）/15*32字节。
/FF_MAX_LFN以UTF-16代码单位定义工作缓冲区的大小
/在12到255之间。建议设置为255以完全支持LFN
/规范。
/当使用堆栈作为工作缓冲区时，请注意堆栈溢出。使用堆时
/工作缓冲区的内存、内存管理函数、ff_memalloc（）和
/ff_memfree（）是ffsystem中的示例。c、 需要添加到项目中*/

#define FF_LFN_UNICODE	0
/*当启用LFN时，此选项切换API上的字符编码。
/
/0:当前CP中的ANSI/OEM（TCHAR=字符）
/1:UTF-16中的Unicode（TCHAR=WCHAR）
/2:UTF-8中的Unicode（TCHAR=char）
/3:UTF-32中的Unicode（TCHAR=DWORD）
/
/字符串I/O函数的行为也会受到此选项的影响。
/未启用LFN时，此选项无效*/

#define FF_LFN_BUF		255
#define FF_SFN_BUF		12
/*这组选项定义FILINFO结构中文件名成员的大小
/其用于读出目录项。这些值应足以
/要读取的文件名。读取文件名的最大可能长度取决于
/关于字符编码。当未启用LFN时，这些选项无效*/

#define FF_FS_RPATH		0
/*此选项配置对相对路径的支持。
/
/0：禁用相对路径并删除相关函数。
/1：启用相对路径。f_chdir（）和f_chdrive（）可用。
/2:f_getcwd（）函数除了1之外还可用。
*/

/*---------------------------------------------------------------------------/
/ 驱动器/卷配置
/---------------------------------------------------------------------------*/

#define FF_VOLUMES		1
/*要使用的卷（逻辑驱动器）数。(1-10) */

#define FF_STR_VOLUME_ID	0
#define FF_VOLUME_STRS		"RAM","NAND","CF","SD","SD2","USB","USB2","USB3"
/*FF_STR_VOLUME_ID开关支持任意字符串中的卷ID。
/当FF_STR_VOLUME_ID设置为1或2时，可以使用任意字符串作为驱动器
/路径名中的数字。FF_VOLUME_STRS定义每个卷的卷ID字符串
/逻辑驱动器。项目数量不得小于FF_VOLUMES。有效的
/卷ID字符串的字符是A-Z、A-Z和0-9，但它们是
/比较不区分大小写。如果FF_STR_VOLUME_ID>=1且FF_VOLUME_STRS为
/未定义，用户定义的卷字符串表需要定义为：
/
/constchar*VolumeStr[FF_VOLUMES]={“ram”，“flash”，“sd”，“usb”，。。。
*/

#define FF_MULTI_PARTITION	0
/*此选项切换对物理驱动器上多个卷的支持。
/默认情况下（0），每个逻辑驱动器号都绑定到同一物理驱动器
/编号，并且只有在物理驱动器上找到的FAT卷将被装载。
/启用此功能时（1），每个逻辑驱动器号都可以绑定到
/VolToPart[]中列出的任意物理驱动器和分区。还有f_fdisk（）
/功能将可用*/

#define FF_MIN_SS		512
#define FF_MAX_SS		512
/*这组选项配置要支持的扇区大小范围。(512,
/1024、2048或4096）对于大多数系统、通用存储卡和
/硬盘，但板载闪存和一些
/光学介质类型。当FF_MAX_SS大于FF_MIN_SS时，配置FatFs
/对于可变扇区大小模式，需要实现disk_ioctl（）函数
/GET_SECTOR_SIZE命令*/

#define FF_LBA64		0
/*此选项切换对64位LBA的支持。（0：禁用或1：启用）
/要启用64位LBA，还需要启用exFAT。（FF_FS_EXFAT==1）*/

#define FF_MIN_GPT		0x10000000
/* Minimum number of sectors to switch GPT as partitioning format in f_mkfs and
/  f_fdisk function. 0x100000000 max. This option has no effect when FF_LBA64 == 0. */


#define FF_USE_TRIM		0
/* This option switches support for ATA-TRIM. (0:Disable or 1:Enable)
/  To enable Trim function, also CTRL_TRIM command should be implemented to the
/  disk_ioctl() function. */



/*---------------------------------------------------------------------------/
/系统配置
/---------------------------------------------------------------------------*/

#define FF_FS_TINY		0
/* This option switches tiny buffer configuration. (0:Normal or 1:Tiny)
/  At the tiny configuration, size of file object (FIL) is shrinked FF_MAX_SS bytes.
/  Instead of private sector buffer eliminated from the file object, common sector
/  buffer in the filesystem object (FATFS) is used for the file data transfer. */


#define FF_FS_EXFAT		0
/* This option switches support for exFAT filesystem. (0:Disable or 1:Enable)
/  To enable exFAT, also LFN needs to be enabled. (FF_USE_LFN >= 1)
/  Note that enabling exFAT discards ANSI C (C89) compatibility. */


#define FF_FS_NORTC		0
#define FF_NORTC_MON	1
#define FF_NORTC_MDAY	1
#define FF_NORTC_YEAR	2020
/* The option FF_FS_NORTC switches timestamp functiton. If the system does not have
/  any RTC function or valid timestamp is not needed, set FF_FS_NORTC = 1 to disable
/  the timestamp function. Every object modified by FatFs will have a fixed timestamp
/  defined by FF_NORTC_MON, FF_NORTC_MDAY and FF_NORTC_YEAR in local time.
/  To enable timestamp function (FF_FS_NORTC = 0), get_fattime() function need to be
/  added to the project to read current time form real-time clock. FF_NORTC_MON,
/  FF_NORTC_MDAY and FF_NORTC_YEAR have no effect.
/  These options have no effect in read-only configuration (FF_FS_READONLY = 1). */


#define FF_FS_NOFSINFO	0
/* If you need to know correct free space on the FAT32 volume, set bit 0 of this
/  option, and f_getfree() function at first time after volume mount will force
/  a full FAT scan. Bit 1 controls the use of last allocated cluster number.
/
/  bit0=0: Use free cluster count in the FSINFO if available.
/  bit0=1: Do not trust free cluster count in the FSINFO.
/  bit1=0: Use last allocated cluster number in the FSINFO if available.
/  bit1=1: Do not trust last allocated cluster number in the FSINFO.
*/


#define FF_FS_LOCK		0
/* The option FF_FS_LOCK switches file lock function to control duplicated file open
/  and illegal operation to open objects. This option must be 0 when FF_FS_READONLY
/  is 1.
/
/  0:  Disable file lock function. To avoid volume corruption, application program
/      should avoid illegal open, remove and rename to the open objects.
/  >0: Enable file lock function. The value defines how many files/sub-directories
/      can be opened simultaneously under file lock control. Note that the file
/      lock control is independent of re-entrancy. */


/* #include <somertos.h>	// O/S definitions */
#define FF_FS_REENTRANT	0
#define FF_FS_TIMEOUT	1000
#define FF_SYNC_t		HANDLE
/* The option FF_FS_REENTRANT switches the re-entrancy (thread safe) of the FatFs
/  module itself. Note that regardless of this option, file access to different
/  volume is always re-entrant and volume control functions, f_mount(), f_mkfs()
/  and f_fdisk() function, are always not re-entrant. Only file/directory access
/  to the same volume is under control of this function.
/
/   0: Disable re-entrancy. FF_FS_TIMEOUT and FF_SYNC_t have no effect.
/   1: Enable re-entrancy. Also user provided synchronization handlers,
/      ff_req_grant(), ff_rel_grant(), ff_del_syncobj() and ff_cre_syncobj()
/      function, must be added to the project. Samples are available in
/      option/syscall.c.
/
/  The FF_FS_TIMEOUT defines timeout period in unit of time tick.
/  The FF_SYNC_t defines O/S dependent sync object type. e.g. HANDLE, ID, OS_EVENT*,
/  SemaphoreHandle_t and etc. A header file for O/S definitions needs to be
/  included somewhere in the scope of ff.h. */



/*--- End of configuration options ---*/
