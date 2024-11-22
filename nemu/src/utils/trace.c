#include<utils.h>
#include <cpu/decode.h>
#include <elf.h>

iringbuf *rbuf;
fringbuf *fbuf;
static char *r_elf_file = NULL;
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
    Elf32_Ehdr *ef = malloc(sizeof(Elf32_Ehdr));
    
    value = fscanf("Magic:%s\n",ef->e_ident);

    free(ef);
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