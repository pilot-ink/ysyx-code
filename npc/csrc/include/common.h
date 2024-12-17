#ifndef __COMMON__
#define __COMMON__


#include <readline/readline.h>
#include <readline/history.h>

#include "sdb.h"
#include "cpu_exec.h"

#include"Vtop.h"  // 从top.v生成
#include"Vtop___024root.h"
#include"verilated.h"
#include"verilated_vcd_c.h"

#define CONFIG_ISA_riscv 1
#define CONFIG_ISA "riscv32"
extern VerilatedContext* contextp ;
extern VerilatedVcdC* m_trace ; // trace_object
extern Vtop* top ;
extern int flag ;
extern uint8_t * pmem;

#endif