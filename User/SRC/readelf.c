#include "main.h"

uint8_t seedtokey[2048];    // 申请2K内存用以保存sa计算代码

FIL ff;                     // 文件操作句柄
UINT s;

// 读取特定段的内容
void reads(uint32_t addr,uint8_t i,uint32_t siz,uint8_t * read_buf){
    uint32_t add;
    add = addr + (i *siz);
    f_open(&ff,ELFFILE,FA_READ);
    f_lseek(&ff,add);
    f_read(&ff,read_buf,siz,&s);
    f_close(&ff);
}

// 读取字符表
void read_str(uint32_t addr,uint32_t siz,uint8_t * read_strbuf){
    f_open(&ff,ELFFILE,FA_READ);
    f_lseek(&ff,addr);
    f_read(&ff,read_strbuf,siz,&s);
    f_close(&ff);
}

// 读取代码段
void read_text(uint32_t addr,uint32_t siz){
    f_open(&ff,ELFFILE,FA_READ);
    f_lseek(&ff,addr);
    f_read(&ff,seedtokey,siz,&s);
    f_close(&ff);
}

// 获取SA算法及起始地址
uint32_t read_sa_offset(void){
    uint8_t read_buf[64];         // 数据读取缓存
    uint8_t read_strbuf[256];            // 符号表
    uint8_t read_sym[64];
    uint32_t addse;     // 段表地址
    uint32_t lense; // 段长度
    uint32_t numse;     // 段数量
    uint16_t inde;      // 字符表索引
    uint32_t adddyn;    // 符号表地址
    uint32_t lendyn;    // 符号表大小
    uint32_t numdyn;    // 符号表数量
    uint32_t addtext;   // 代码段地址
    ELF32_Ehdr * Ehdr;
    ELF_32_shdr * shdr;
    Elf32_Sym * Sym;
    int i;
    if(f_open(&ff,ELFFILE,FA_READ) != 0){
        printf("%s file not found\n",ELFFILE);
        return 0xFFFF;
    }
    // 读取elf头
    f_read(&ff,read_buf,sizeof(ELF32_Ehdr),&s);
    f_close(&ff);
    Ehdr = (ELF32_Ehdr *)read_buf;
    addse = Ehdr->e_shoff;   // 段地址
    lense = Ehdr->e_shentsize;  // 段长度
    numse = Ehdr->e_shnum;   // 段数量
    inde = Ehdr->e_shstrndx;    // 字符
    // 读段名字符表
    reads(addse,inde,lense,read_buf);
    shdr = (ELF_32_shdr *)read_buf;
    read_str(shdr->sh_offset,shdr->sh_size,read_strbuf);
    // 循环读数据段，对不同段做不同处理
    for(i = 0;i<numse;i++){
        reads(addse,i,lense,read_buf);
        shdr = (ELF_32_shdr *)read_buf;
        if(check_str(read_strbuf+shdr->sh_name,(uint8_t *)".text",5)){
            // 代码段读入内存
            addtext = shdr->sh_offset;
            read_text(addtext,shdr->sh_size);
        }else if(check_str(read_strbuf+shdr->sh_name,(uint8_t *)".dynstr",7)){
            // 符号字符表,读入缓冲区
            read_str(shdr->sh_offset,shdr->sh_size,read_sym);
        }else if(check_str(read_strbuf+shdr->sh_name,(uint8_t *)".dynsym",7)){
            // 符号表记录表信息
            adddyn = shdr->sh_offset;   // 符号表地址
            lendyn = shdr->sh_entssize; // 符号表长度
            numdyn = shdr->sh_size/lendyn;  // 符号表数量
        }
    }
    // 开始读取符号表，获取入口地址
    for(i = 0;i<numdyn;i++){
        reads(adddyn,i,lendyn,read_buf);
        Sym = (Elf32_Sym *)read_buf;
        if(((Sym->st_info&0x0f )== 2)&&(check_str(read_sym + Sym->st_name,(uint8_t *)"SeedToKey",9))){
            adddyn = Sym->st_value;
            break;
        }
    }
    // 返回索引值
    return adddyn-addtext;
}
