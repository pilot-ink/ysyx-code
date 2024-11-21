#include<utils.h>
#include <cpu/decode.h>

iringbuf *rbuf;
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
void push_iringbuf(char *str){
    printf("test:%s\n",str);
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
