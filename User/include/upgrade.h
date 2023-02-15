#ifndef UPGRADE_H
#define UPGRADE_H

#include<stdio.h>
#include<stdint.h>

#define ELFFILE "/flash/security.dll"

extern uint8_t seedtokey[2048];

typedef void (*security_f)(uint8_t * seed,uint8_t * kay);

extern security_f security;

// ELF  结构体
// ELF 文件头解析
typedef struct {
    uint8_t e_ident[16];    //ELF文件的描述，是一个16字节的标识，表明当前文件的数据格式，位数等：
    uint16_t e_type;        /*文件的标识字段标识文件的类型；
                                ET_NONE(0)：未知的文件格式；
                                ET_REL(1)：可重定位文件，比如目标文件；
                                ET_EXEC(2)：可执行文件；
                                ET_DYN(3)：共享目标文件；
                                ET_CORE(4)：Core转储文件，比如程序crash之后的转储文件；
                                ET_LOPROC(0xff00)：特定处理器的文件标识；
                                ET_HIPROC(0xffff)：特定处理器的文件标识;
                                [ET_LOPROC,ET_HIPROC]之间的值用来表示特定处理器的文件格式；*/
    uint16_t e_machine;     // 目标文件的体系结构
    uint32_t e_version;     // 当前文件的版本
    uint32_t e_entry;       // 程序的虚拟入口地址
    uint32_t e_phoff;       // 文件中程序头表的偏移
    uint32_t e_shoff;       // 段表在文件中的偏移
    uint32_t e_flags;       // 处理器相关的标志位
    uint16_t e_ehsize;      // ELF文件头的大小
    uint16_t e_phentsize;   // 程序头表中单项的大小，表中每一项的大小相同
    uint16_t e_phnum;       // 程序头表中的项数
    uint16_t e_shentsize;   //  // 段描述符大小
    uint16_t e_shnum;       //    // 表明有多少段
    uint16_t e_shstrndx;    // 节表中节名的索引
} ELF32_Ehdr;

// 段头表
typedef struct {
    uint32_t sh_name;   // 值是节名称在字符串表中的索引；
    uint32_t sh_type;   // 描述节的类型和语义
    uint32_t sh_flags;  // 1bit位的标志位
    uint32_t sh_addr;   // 如果当前节需要被装载到内存，则当前项存储当前节映射到内存的首地址
    uint32_t sh_offset; // 当前节的首地址相对于文件的偏移
    uint32_t sh_size;   // 节的大小。
    uint32_t sh_link;   // 存储节投标中的索引，表示当前节依赖于对应的节
    uint32_t sh_info;   // 节的附加信息。对于特定的节有特定的含义，其他为0；
    uint32_t sh_addralign;  // 地址约束对齐，
    uint32_t sh_entssize;   // 某些节是一个数组，对于这类节当前字段给出数组中每个项的字节数
} ELF_32_shdr;

// 符号表
typedef struct{
  uint32_t	st_name;    // 存储一个指向字符串表的索引来表示对应符号的名称
  uint32_t	st_value;   // 存储对应符号的取值
  uint32_t	st_size;    // 符号的大小
  uint8_t	st_info;    // 指定符号的类型和绑定属性
  uint8_t	st_other;   // 该成员当前持有 0 并且没有定义的含义
  uint16_t	st_shndx;   // 每个符号都有属于的节
} Elf32_Sym;

// 获取SA算法及起始地址
uint32_t read_sa_offset(void);
// 读取代码段
void read_text(uint32_t addr,uint32_t siz);
// 读取字符表
void read_str(uint32_t addr,uint32_t siz,uint8_t * read_strbuf);
// 读取特定段的内容
void reads(uint32_t addr,uint8_t i,uint32_t siz,uint8_t * read_buf);
// 获取函数代码
void security_get(void);
#endif
