#include "common.h"
#include "reg.h"
#include "difftest.h"
#include "trace.h"

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
        pc_npc_inst_sync();
        #ifdef CONFIG_DIFFTEST 
        isa_regs_sync();
        
        difftest_step(s.pc, s.npc); 
        #endif
        #ifdef CONFIG_ITRACE
        
        //用来保存这样的反汇编(0x80000000: 00 00 04 13 mv	s0, zero)
        char *p = s.logbuf;
        //输出"0x80000000:" "pc:"
        p += snprintf(p, sizeof(s.logbuf), "0x%08x", s.pc);
        //输出指令的机器码
        int ilen = 4;
        int j;
        uint8_t *inst = (uint8_t *)&s.inst;
        for (j = ilen - 1; j >= 0; j --) {
            p += snprintf(p, 4, " %02x", inst[j]);
        }
        //00 00 04 13 mv，用来输出在机器码与汇编指令的空格
        int ilen_max = 4;
        int space_len = ilen_max - ilen;
         if (space_len < 0) space_len = 0;
        space_len = space_len * 3 + 1;
        memset(p, ' ', space_len);
         p += space_len;
        
        //输出反汇编到s->logbuf
        void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
        disassemble(p, s.logbuf + sizeof(s.logbuf) - p, s.pc, (uint8_t *)&s.inst, ilen);
        push_iringbuf(s.logbuf);
        //printf("inst:%s\n",s.logbuf);
        #endif
        #ifdef CONFIG_FTRACE
            char inst_name[4];
            strncpy(inst_name, p, sizeof(char)*4);
            //printf("%s\tvalue:%d\n",inst_name,strncmp(inst_name, "jal",sizeof(char)*3));
            if(!strncmp(inst_name, "jal",sizeof(char)*3) || \
                !strncmp(inst_name, "jalr",sizeof(char)*4) || \
                !strncmp(inst_name, "ret" ,sizeof(char)*3)){
                if(s.inst ==0x00008067){
                    push_fringbuf(s.pc,s.npc,1);
                }
                else if(BITS(s.inst, 11, 7) == 1){
                    push_fringbuf(s.pc,s.npc,0);
                }
               // print_fringbuf();
            }
        #endif
        
        if(flag == 1) break;
    }
}