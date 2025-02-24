#ifndef __REG__
#define __REG__

#include "common.h"

typedef struct 
{
    word_t gpr[32];
    vaddr_t pc;
}CPU_state;

bool isa_difftest_checkregs(CPU_state *ref, vaddr_t pc);
void isa_regs_display(CPU_state *ref);
void isa_regs_sync();

#endif