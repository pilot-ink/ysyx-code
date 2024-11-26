#include<utils.h>
#include <cpu/decode.h>
#include <elf.h>

iringbuf *rbuf;
fringbuf *fbuf;
struct linked_list *head_st;
static char *r_elf_file = NULL;
long get_file_size(FILE *stream);
void outputsyminfo(const Elf32_Sym *psym, const char *pbuffstr, int ncount);
void init_iringbuf(){
    rbuf = malloc(sizeof(iringbuf));
    for(int i = 0; i < Ringbuffer_max;i++)
        rbuf->buffer[i] = malloc(sizeof(char) * 128);
}

void destory_iringbuf(){
    for(int i = 0; i < Ringbuffer_max;i++)
        free(rbuf->buffer[i]);
    free(rbuf);
}
//workable
void push_iringbuf(char *str){
    if(RingBuffer_full(rbuf)){
        rbuf->start = 0;
        rbuf->end = 0;
        strcpy(rbuf->buffer[rbuf->start],str);
        rbuf->start++;
        return;
    }
    else{
        strcpy(rbuf->buffer[rbuf->start],str);
        rbuf->start++;
    }
}
void print_iringbuf()
{
    for(int i = rbuf->end;(i%Ringbuffer_max)+1 != rbuf->start;i=(i+1)%Ringbuffer_max)
     printf("inst:%s\n",rbuf->buffer[i]);
}

void read_elf(char *file){
    int value;
    r_elf_file = file;
    FILE *fp = fopen(r_elf_file, "r");
    if(!fp) panic("can not open %s file",r_elf_file);
    Elf32_Ehdr *read_elf_file = malloc(sizeof(Elf32_Ehdr));
    char *pbuff = malloc(get_file_size(fp));
    char *buffer = pbuff;
    fread(pbuff, sizeof(char), get_file_size(fp), fp);
    printf("ELF Header:\r\n");
    //Magic
    printf("  Magic:   ");
    for(int i = 0;i<EI_NIDENT;++i)   //e_ident[EI_NIDENT]
    {
        printf("%02X", pbuff[i]);
        putchar(' ');
    }
    printf("\r\n");
    printf("  %-33s:", "Class");
    switch(pbuff[EI_CLASS])
    {
        case 0:
            printf(" Invalid class\r\n");
            break;
        case 1:
            printf(" ELF32\r\n");
            break;
        case 2:
            printf(" ELF64\r\n");
            break;
        default:
            printf(" ERROR\r\n");
            break;
    }
    printf("  %-33s:", "Data");
    switch(pbuff[EI_DATA])
    {
        case 0:
            printf(" Invalid data encoding\r\n");
            break;
        case 1:
            printf(" 2's complement, little endian\r\n");
            break;
        case 2:
            printf(" 2's complement, big endian\r\n");
            break;
        default:
            printf(" ERROR\r\n");
            break;
    }
    //Version
    printf("  %-33s: %s\r\n", "Version", "1(current)");
    //OS/ABI
    printf("  %-33s: %s\r\n", "OS/ABI", "UNIX - System V");
    //ABI Version
    printf("  %-33s: %s\r\n", "ABI Version", "0");
    pbuff += EI_NIDENT;
    //Type
    printf("  %-33s:", "Type");
    switch(*(uint16_t*)pbuff)
    {
        case 0:
            printf(" No file type\r\n");
            break;
        case 1:
            printf(" Relocatable file\r\n");
            break;
        case 2:
            printf(" Executable file\r\n");
            break;
        case 3:
            printf(" Shared object file\r\n");
            break;
        case 4:
            printf(" Core file\r\n");
            break;
        default:
            printf(" ERROR\r\n");
            break;
    }
    pbuff += sizeof(uint16_t);
    //Machine
    printf("  %-33s:", "Machine");
    switch(*(uint16_t*)pbuff)
    {
        case EM_386:
            printf(" Intel 80386\r\n");
            break;
        case EM_ARM:
            printf(" ARM\r\n");
            break;
        case EM_X86_64:
            printf(" AMD X86-64 arrchitecture\r\n");
            break;
        case EM_RISCV:
            printf(" RISCV\r\n");
            break;
        default:
            printf(" ERROR\r\n");
            break;
    }
    pbuff += sizeof(uint16_t);
    //Version
    printf("  %-33s: %s\r\n", "version", "0X1");
    pbuff += sizeof(uint32_t);
    //入口点位置
    printf("  %-33s: 0X%x\r\n", "Entry point address", *(uint32_t*)pbuff);
    pbuff += sizeof(uint32_t);
    //程序头大小
    printf("  %-33s: %u (bytes into file)\r\n", "Start of program headers", *(uint32_t*)pbuff);
    pbuff += sizeof(uint32_t);
    //区段大小
    printf("  %-33s: %u (bytes into file)\r\n", "Start of section headers", *(uint32_t*)pbuff);
    read_elf_file->e_shoff = *(uint32_t*)pbuff;
    pbuff += sizeof(uint32_t);
    //Flags
    printf("  %-33s: 0X0\r\n", "Flags");
    pbuff += sizeof(Elf32_Word);
    //本节大小
    printf("  %-33s: %d (bytes)\r\n", "Size of this header", *(Elf32_Half*)pbuff);
    pbuff += sizeof(Elf32_Half);
    //程序头大小
    printf("  %-33s: %d (bytes)\r\n", "Size of program headers", *(Elf32_Half*)pbuff);
    pbuff += sizeof(Elf32_Half);
    //程序头大小
    printf("  %-33s: %d\r\n", "Number of program headers", *(Elf32_Half*)pbuff);
    pbuff += sizeof(Elf32_Half);
    //section大小
    printf("  %-33s: %d (bytes)\r\n", "Size of section headers", *(Elf32_Half*)pbuff);
    pbuff += sizeof(Elf32_Half);
    //section大小
    printf("  %-33s: %d\r\n", "Number of section headers", *(Elf32_Half*)pbuff);
    read_elf_file->e_shnum = *(Elf32_Half *)pbuff;
    pbuff += sizeof(Elf32_Half);
    //下标值
    printf("  %-33s: %d\r\n", "Section header string table index", *(Elf32_Half*)pbuff);
    read_elf_file->e_shstrndx = *(Elf32_Half*)pbuff;
    Elf32_Shdr *psecheader = (Elf32_Shdr *)(buffer + read_elf_file->e_shoff);
    Elf32_Shdr* pshstr = (Elf32_Shdr*)(psecheader + read_elf_file->e_shstrndx);
    char* pshstrbuff = (char *)(buffer + pshstr->sh_offset);
    
    printf("共有 %d 节区表, 偏移位置开始于 0x%x:\r\n\r\n",read_elf_file->e_shnum, read_elf_file->e_shoff);
      printf("节头:\r\n");  //打印标志位信息
    printf("  [Nr] %-16s  %-16s  %-16s  %-16s\t", "Name", "Type", "Address", "Offset");
    printf("       %-16s  %-16s  %-2s  %-5s  %-5s  %-5s\t\n", "Size", "EntSize", "Flags", "Link", "Info", "Align");
    //遍历每一个节表数量
    for(int i = 0;i<read_elf_file->e_shnum;++i)
    {
        printf("  [%2d] %-16s  ", i, (char *)(psecheader[i].sh_name + pshstrbuff));
        //Type
        switch(psecheader[i].sh_type)
        {
            case SHT_NULL:
                printf("%-16s  ", "NULL");break;
            case SHT_PROGBITS:
                printf("%-16s  ", "PROGBITS");break;
            case SHT_SYMTAB:
                printf("%-16s  ", "SYMTAB");break;
            case SHT_STRTAB:
                printf("%-16s  ", "STRTAB");break;
            case SHT_RELA:
                printf("%-16s  ", "RELA");break;
            case SHT_HASH:
                printf("%-16s  ", "GNU_HASH");break;
            case SHT_DYNAMIC:
                printf("%-16s  ", "DYNAMIC");break;
            case SHT_NOTE:
                printf("%-16s  ", "NOTE");break;
            case SHT_NOBITS:
                printf("%-16s  ", "NOBITS");break;
            case SHT_REL:
                printf("%-16s  ", "REL");break;
            case SHT_SHLIB:
                printf("%-16s  ", "SHLIB");break;
            case SHT_DYNSYM:
                printf("%-16s  ", "DYNSYM");break;
            case SHT_INIT_ARRAY:
                printf("%-16s  ", "INIT_ARRY");break;
            case SHT_FINI_ARRAY:
                printf("%-16s  ", "FINI_ARRY");break;
            case SHT_PREINIT_ARRAY:
                printf("%-16s  ", "PREINIT_ARRAY");break;
            case SHT_GNU_HASH:
                printf("%-16s  ", "GNU_HASH");break;
            case SHT_GNU_ATTRIBUTES:
                printf("%-16s  ", "GNU_ATTRIBUTES");break;
            case SHT_GNU_LIBLIST:
                printf("%-16s  ", "GNU_LIBLIST");break;
            case SHT_GNU_verdef:
                printf("%-16s  ", "GNU_verdef");break;
            case SHT_GNU_verneed:
                printf("%-16s  ", "GNU_verneed");break;
            case SHT_GNU_versym:
                printf("%-16s  ", "GNU_versym");break;
            default:
                printf("%-16s  ", "NONE");break;
        }
        printf("%016X  %08X\t", psecheader[i].sh_addr, psecheader[i].sh_offset);
        printf("       %016X  %016x  ", psecheader[i].sh_size, psecheader[i].sh_entsize);
            switch (psecheader[i].sh_flags) {
                case 0:
                    printf("%3s    %4u  %4u  %4u\t",
                           "", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 1:
                    printf("%3s    %4u  %4u  %4u\t",
                           "W", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 2:
                    printf("%3s    %4u  %4u  %4u\t",
                           "A", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 4:
                    printf("%3s    %4u  %4u  %4u\t",
                           "X", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 3:
                    printf("%3s    %4u  %4u  %4u\t",
                           "WA", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 5://WX
                    printf("%3s    %4u  %4u  %4u\t",
                           "WX", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 6://AX
                    printf("%3s    %4u  %4u  %4u\t",
                           "AX", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case 7://WAX
                    printf("%3s    %4u  %4u  %4u\t",
                           "WAX", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                case SHF_MASKPROC://MS
                    printf("%3s    %4u  %4u  %4u\t",
                           "MS", psecheader[i].sh_link, psecheader[i].sh_info, psecheader[i].sh_addralign);
                    break;
                default:
                    printf("NONE\t");
                    break;
            }
    printf("\n");
    }
     //从节区里面定位到偏移
    Elf32_Ehdr* pfilehead = (Elf32_Ehdr*)buffer;
    Elf32_Half eshstrndx = pfilehead->e_shstrndx;
    psecheader = (Elf32_Shdr*)(buffer + pfilehead->e_shoff);
    pshstr = (Elf32_Shdr*)(psecheader + eshstrndx);
    pshstrbuff = (char *)(buffer + pshstr->sh_offset);
     
    for(int i = 0;i<pfilehead->e_shnum;++i)
    {
        if(!strcmp(psecheader[i].sh_name + pshstrbuff, ".dynsym") || !strcmp(psecheader[i].sh_name + pshstrbuff, ".symtab"))
        {
            Elf32_Sym* psym = (Elf32_Sym*)(buffer + psecheader[i].sh_offset);
            int ncount = psecheader[i].sh_size / psecheader[i].sh_entsize;
            char* pbuffstr = (char*)((psecheader + psecheader[i].sh_link)->sh_offset + buffer);
            printf("Symbol table '%s' contains %d entries:\r\n", psecheader[i].sh_name + pshstrbuff, ncount);
            outputsyminfo(psym, pbuffstr, ncount);
            continue;
        }
    }
}
void init_fringbuf(){
    fbuf = malloc(sizeof(fringbuf));
    for(int i = 0; i < fRingbuffer_max;i++)
        fbuf->buffer[i] = malloc(sizeof(char) * 128);
}
void destory_fringbuf(){
    for(int i = 0; i < fRingbuffer_max;i++)
        free(fbuf->buffer[i]);
    free(fbuf);
}
/*
void push_iringbuf(char *str){
    if(RingBuffer_full(rbuf)){
        rbuf->start = 0;
        rbuf->end = 0;
        strcpy(rbuf->buffer[rbuf->start],str);
        rbuf->start++;
        return;
    }
    else{
        strcpy(rbuf->buffer[rbuf->start],str);
        rbuf->start++;
    }
}
*/
void print_fringbuf()
{
    for(int i = fbuf->end;(i%fRingbuffer_max)+1 != fbuf->start;i=(i+1)%fRingbuffer_max)
     printf("%s\n",fbuf->buffer[i]);
}
long get_file_size(FILE *stream)
{
	long file_size = -1;
	long cur_offset = ftell(stream);	// 获取当前偏移位置
	fseek(stream, 0, SEEK_END);
	file_size = ftell(stream);	// 获取此时偏移值，即文件大小
	fseek(stream, cur_offset, SEEK_SET);
	return file_size;
}
void outputsyminfo(const Elf32_Sym *psym, const char *pbuffstr, int ncount)
{
    printf("%7s  %-8s          %s  %s    %s   %s      %s  %s\r\n",
           "Num:", "Value", "Size", "Type", "Bind", "Vis", "Ndx", "Name");
    for(int i = 0;i<ncount;++i)
    {
        printf("%6d:  %016x  %-6u", i, psym[i].st_value, psym[i].st_size);
        char typelow = ELF32_ST_TYPE(psym[i].st_info);
        char bindhig = ELF32_ST_BIND(psym[i].st_info);
        switch(typelow)
        {
            case STT_NOTYPE:
                printf("%-8s", "NOTYPE");break;
            case STT_OBJECT:
                printf("%-8s", "OBJECT");break;
            case STT_FUNC:
                printf("%-8s", "FUNC");break;
            case STT_SECTION:
                printf("%-8s", "SECTION");break;
            case STT_FILE:
                printf("%-8s", "FILE");break;
            default:
                break;
        }
        switch(bindhig)
        {
            case STB_LOCAL:
                printf("%-8s", "LOCAL"); break;
            case STB_GLOBAL:
                printf("%-8s", "GLOBAL"); break;
            case STB_WEAK:
                printf("%-8s", "WEAK"); break;
            default:
                break;
        }
        printf("%-8d", psym[i].st_other);
        switch(psym[i].st_shndx)
        {
            case SHN_UNDEF:
                printf("%s  %s\r\n", "UND", psym[i].st_name + pbuffstr);break;
            case SHN_ABS:
                printf("%s  %s\r\n", "ABS", psym[i].st_name + pbuffstr);break;
            case SHN_COMMON:
                printf("%s  %s\r\n", "COM", psym[i].st_name + pbuffstr);break;
            default:
                printf("%3d  %s\r\n", psym[i].st_shndx, psym[i].st_name + pbuffstr);break;
        }
    }
}
#ifdef CONFIG_mtrace
void init_mringbuf(){
    mbuf = malloc(sizeof(mringbuf)); 
}
void destory_mringbuf(){
    free(mbuf);
}
void push_mringbuf(char wr,paddr_t addr, word_t data){
    if(RingBuffer_full(mbuf)){
        mbuf->start = 0;
        mbuf->end   = 0;
        mbuf->wrbuffer[mbuf->start] = wr;
        mbuf->pbuffer[mbuf->start] = addr;
        mbuf->dbuffer[mbuf->start] = data;
        mbuf->start++;
    }
    else{
        mbuf->wrbuffer[mbuf->start] = wr;
        mbuf->pbuffer[mbuf->start] = addr;
        mbuf->dbuffer[mbuf->start] = data;
        mbuf->start++;
    }
}
void print_mringbuf(){
    for(int i = mbuf->end;(i%mRingbuffer_max)+1 != mbuf->start;i=(i+1)%Rmingbuffer_max)
     printf("%c\taddr:%08X\tdata:%X\n",mbuf->wrbuffer[i],mbuf->pbuffer[i],mbuf->dbuffer[i]);
}
#endif