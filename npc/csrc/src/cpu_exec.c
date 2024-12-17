#include"common.h"


uint32_t pmem_read(uint32_t pc)
{
    pc -= 0x80000000;
    uint8_t *ptr = pmem;
    ptr += pc;
    uint32_t *value = (uint32_t *)ptr;
    //printf("value:%x\n",*value);
    return *value;
}

void cpu_exec(int n){
    if(flag == 1) return ;
        for(int i = 0;i < n; i++){
            top->inst = pmem_read(top->pc);
            top->clk = !top->clk; top->eval(); // eval()模型更新 可以理解为执行一次.v文件
            top->clk = !top->clk; top->eval(); // eval()模型更新 可以理解为执行一次.v文件
            m_trace->dump(contextp->time());
            contextp->timeInc(1);  
            top->pc = top->npc;
            //if(flag == 1) break;
        }
}