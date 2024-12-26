#include "common.h"
#include "reg.h"
#include "difftest.h"

Decode s = {};
CPU_state cpu = {};

void pc_npc_inst_sync(){
    s.pc = top->rootp->top__DOT__c1__DOT__pc;
    //uint32_t A=top__DOT__c1__DOT__PCA_out,B;
    s.npc = (top->rootp->top__DOT__c1__DOT__PCA_out + top->rootp->top__DOT__c1__DOT__PCB_out);
    s.inst = top->rootp->top__DOT__c1__DOT__inst;
    //printf("pc:%08x,npc%08x,inst:%08x\n",s.pc, s.npc, s.inst);
}

void cpu_exec(uint32_t n){
    for(int i = 0;i < n; i++){
        
        
        top->clk = !top->clk; top->eval(); // eval()模型更新 可以理解为执行一次.v文件
        m_trace->dump(contextp->time());
        contextp->timeInc(1);
        top->clk = !top->clk; top->eval(); // eval()模型更新 可以理解为执行一次.v文件
        m_trace->dump(contextp->time());
        contextp->timeInc(1); 
        isa_regs_sync();
        pc_npc_inst_sync();
        difftest_step(s.pc, s.npc); 
        
        
        if(flag == 1) break;
    }
}