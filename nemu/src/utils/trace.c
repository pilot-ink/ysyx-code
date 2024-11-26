#include<utils.h>
#include <cpu/decode.h>
#include <elf.h>

iringbuf *rbuf;
fringbuf *fbuf;
struct linked_list *head_st;
struct linked_list *ptr_list_tail;
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
    //Magic
    strncpy(read_elf_file->e_ident,pbuff,EI_NIDENT);
    pbuff += EI_NIDENT;
    //Type
    read_elf_file->e_type = *(uint16_t*)pbuff;
    pbuff += sizeof(uint16_t);
    //Machine
    read_elf_file->e_machine = *(uint16_t*)pbuff;
    pbuff += sizeof(uint16_t);
    //Version
    read_elf_file->e_version = *(uint32_t*)pbuff;
    pbuff += sizeof(uint32_t);
    //入口点位置
    read_elf_file->e_entry = *(uint32_t*)pbuff;
    pbuff += sizeof(uint32_t);
    //程序头大小
    read_elf_file->e_phoff = *(uint32_t*)pbuff;
    pbuff += sizeof(uint32_t);
    //区段大小
    read_elf_file->e_shoff = *(uint32_t*)pbuff;
    pbuff += sizeof(uint32_t);
    //Flags
    read_elf_file->e_flags = *(uint32_t*)pbuff;
    pbuff += sizeof(Elf32_Word);
    //本节大小
    read_elf_file->e_ehsize = *(uint32_t*)pbuff;
    pbuff += sizeof(Elf32_Half);
    //程序头大小
    read_elf_file->e_phentsize = *(uint32_t*)pbuff;
    pbuff += sizeof(Elf32_Half);
    //程序头大小
    read_elf_file->e_phnum = *(uint32_t*)pbuff;
    pbuff += sizeof(Elf32_Half);
    //section大小
    read_elf_file->e_shentsize = *(uint32_t*)pbuff;
    pbuff += sizeof(Elf32_Half);
    //section大小
    read_elf_file->e_shnum = *(Elf32_Half *)pbuff;
    pbuff += sizeof(Elf32_Half);
    //下标值
    read_elf_file->e_shstrndx = *(Elf32_Half*)pbuff;
    
    //从节区里面定位到偏移
    Elf32_Ehdr* pfilehead = (Elf32_Ehdr*)buffer;
    Elf32_Half eshstrndx = pfilehead->e_shstrndx;
    Elf32_Shdr *psecheader = (Elf32_Shdr*)(buffer + pfilehead->e_shoff);
    Elf32_Shdr *pshstr = (Elf32_Shdr*)(psecheader + eshstrndx);
    char *pshstrbuff = (char *)(buffer + pshstr->sh_offset);
     
    for(int i = 0;i<pfilehead->e_shnum;++i)
    {
        if(!strcmp(psecheader[i].sh_name + pshstrbuff, ".dynsym") || !strcmp(psecheader[i].sh_name + pshstrbuff, ".symtab"))
        {
            Elf32_Sym* psym = (Elf32_Sym*)(buffer + psecheader[i].sh_offset);
            int ncount = psecheader[i].sh_size / psecheader[i].sh_entsize;
            char* pbuffstr = (char*)((psecheader + psecheader[i].sh_link)->sh_offset + buffer);
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
    for(int i = 0;i<ncount;++i)
    {
        char typelow = ELF32_ST_TYPE(psym[i].st_info);
        char bindhig = ELF32_ST_BIND(psym[i].st_info);
        switch(typelow)
        {
            case STT_NOTYPE:
                break;
            case STT_OBJECT:
                break;
            case STT_FUNC:
                ptr_list_tail->next->next = malloc(sizeof(struct linked_list));
                ptr_list_tail->next = ptr_list_tail->next->next;
                ptr_list_tail->next->addr = psym[i].st_value;
                ptr_list_tail->next->size = psym[i].st_size;
                ptr_list_tail->next->next = NULL;
                strcpy(ptr_list_tail->next->func_name, psym[i].st_name + pbuffstr);
                break;
            case STT_SECTION:
                break;
            case STT_FILE:
                break;
            default:
                break;
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