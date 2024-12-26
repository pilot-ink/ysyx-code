#include "common.h"
#include "reg.h"

extern CPU_state cpu;

bool isa_difftest_checkregs(CPU_state *ref, vaddr_t pc){
    if(ref->pc != s.npc) return false;
    for(int i = 0;i < 32;i++){
        if(ref->gpr[i] != cpu.gpr[i])
        {
            if(ref->gpr[i] == top->rootp->top__DOT__c1__DOT__busW) ;
            else return false;
        }    
    }
    return true;
}

void isa_regs_display(CPU_state *ref){
    for(int i = 0; i < 32; i++)
        printf("%s:\t0x%08x\treal:0x%08x\n:",regs[i],top->rootp->top__DOT__c1__DOT__gpr1__DOT__gr__DOT__rf[i],ref->gpr[i]);
    printf("pc:\t0x%08x\treal:0x%08x\n",s.npc,ref->pc);
    printf("diff:%08x\n",top->rootp->top__DOT__c1__DOT__busW);
}


void isa_regs_sync()
{
    for(int i = 0; i < 32; i++)
        cpu.gpr[i] = top->rootp->top__DOT__c1__DOT__gpr1__DOT__gr__DOT__rf[i];
    cpu.pc = top->rootp->top__DOT__c1__DOT__pc;
}