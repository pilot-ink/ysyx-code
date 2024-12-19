#include "common.h"
#include "reg.h"

extern CPU_state cpu;

bool isa_difftest_checkregs(CPU_state *ref, vaddr_t pc){
    if(ref->pc != top->pc) return false;
    for(int i = 0;i < 32;i++){
        if(ref->gpr[i] != top->rootp->top__DOT__c1__DOT__gpr1__DOT__gr__DOT__rf[i])
            return false;
    }
    return true;
}

void isa_regs_display(){
    for(int i = 0; i < 32; i++)
        printf("%d\t%s:\t0x%08x\n",i,regs[i],top->rootp->top__DOT__c1__DOT__gpr1__DOT__gr__DOT__rf[i]);
}


void isa_regs_sync()
{
    for(int i = 0; i < 32; i++)
        cpu.gpr[i] = top->rootp->top__DOT__c1__DOT__gpr1__DOT__gr__DOT__rf[i];
    cpu.pc = top->rootp->top__DOT__c1__DOT__gpr1__DOT__gr__DOT__rf[32];
}