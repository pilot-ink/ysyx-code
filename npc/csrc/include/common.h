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

#define CONFIG_ITRACE 1
#define CONFIG_FTRACE 1
#define CONFIG_MTRACE 1

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

// macro concatenation
#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)
#define concat3(x, y, z) concat(concat(x, y), z)
#define concat4(x, y, z, w) concat3(concat(x, y), z, w)
#define concat5(x, y, z, v, w) concat4(concat(x, y), z, v, w)

// macro testing
// See https://stackoverflow.com/questions/26099745/test-if-preprocessor-symbol-is-defined-inside-macro
#define CHOOSE2nd(a, b, ...) b
#define MUX_WITH_COMMA(contain_comma, a, b) CHOOSE2nd(contain_comma a, b)
#define MUX_MACRO_PROPERTY(p, macro, a, b) MUX_WITH_COMMA(concat(p, macro), a, b)
// define placeholders for some property
#define __P_DEF_0  X,
#define __P_DEF_1  X,
#define __P_ONE_1  X,
#define __P_ZERO_0 X,
// define some selection functions based on the properties of BOOLEAN macro
#define MUXDEF(macro, X, Y)  MUX_MACRO_PROPERTY(__P_DEF_, macro, X, Y)
#define MUXNDEF(macro, X, Y) MUX_MACRO_PROPERTY(__P_DEF_, macro, Y, X)
#define MUXONE(macro, X, Y)  MUX_MACRO_PROPERTY(__P_ONE_, macro, X, Y)
#define MUXZERO(macro, X, Y) MUX_MACRO_PROPERTY(__P_ZERO_,macro, X, Y)









#endif