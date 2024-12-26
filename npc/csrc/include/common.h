#ifndef __COMMON__
#define __COMMON__


#include <readline/readline.h>
#include <readline/history.h>



#include"Vtop.h"  // 从top.v生成
#include"Vtop___024root.h"
#include"verilated.h"
#include"verilated_vcd_c.h"

#define CONFIG_ISA_riscv 1
#define CONFIG_ISA "riscv32"
#define CONFIG_MBASE 0x80000000
#define CONFIG_MSIZE 0x8000000

typedef uint32_t word_t;
typedef uint32_t vaddr_t;
typedef uint32_t paddr_t;

typedef struct Decode {
  vaddr_t pc;
  vaddr_t npc; // next pc
  uint32_t inst;
  char logbuf[128];
} Decode;



extern VerilatedContext* contextp ;
extern VerilatedVcdC* m_trace ; // trace_object
extern Vtop* top ;
extern int flag ;
extern uint8_t * pmem;

extern const char *regs[];
extern Decode s;











#endif