#ifndef __TRACE__
#define __TRACE__
#include "common.h"
// ----------- trace -----------


#ifdef CONFIG_ITRACE
void init_iringbuf();
void destory_iringbuf();
void push_iringbuf(char *str);
void print_iringbuf();
#define Ringbuffer_max 10
typedef struct{
  char *buffer[Ringbuffer_max];
  int start;
  int end;
}iringbuf;
#endif

#ifdef CONFIG_FTRACE
void init_fringbuf();
void push_fringbuf(vaddr_t pc, vaddr_t snpc,int a);
char *find_linked_list(vaddr_t pc);
void print_fringbuf();
void read_elf(char *file);

#define BITMASK(bits) ((1ull << (bits)) - 1)
#define BITS(x, hi, lo) (((x) >> (lo)) & BITMASK((hi) - (lo) + 1)) // similar to x[hi:lo] in verilog

#define fRingbuffer_max 1000
typedef struct{
  vaddr_t pc;
  vaddr_t snpc;
  int type;
  char src_func[30];
  char dst_func[30];
}func_info;

typedef struct{
  func_info *buffer[fRingbuffer_max];
  int start;
  int end;
  //int n;        //reminder of numbers of calling func
}fringbuf;

struct linked_list{
  char  func_name[40];
  uint32_t   addr;
  int   size;
  struct linked_list * next;
};
#endif


#ifdef CONFIG_MTRACE
void init_mringbuf();
void print_mringbuf();
void push_mringbuf(char wr,paddr_t addr, word_t data);
#define mRingbuffer_max 20
typedef struct{
  char  wrbuffer[mRingbuffer_max];
  paddr_t pbuffer[mRingbuffer_max];
  word_t  dbuffer[mRingbuffer_max];
  int start;
  int end;
}mringbuf;
#endif




#endif