#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#//include<nvboard.h>
#include"Vtop.h"  // 从top.v生成
#include"verilated.h"
#include"verilated_vcd_c.h"

int flag = 0;
static const uint32_t pmem[] = {
    0b00000000010100000000000010010011, //addi x1 x0 5
    0b00000000000100000000000100010011, //addi x2 x0 1
    0b00000000001000000000000100010011, //addi x2 x0 5
    0b00000000010100001000000100010011  //addi x2 x1 5
};
uint32_t pmem_read(uint32_t pc)
{
    pc -= 0x80000000;
    return pmem[pc];
}

void npc_trap(){
    flag = 1;
}


//void nvboard_bind_all_pins(Vtop* top);
int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->traceEverOn(true); // 开启波形 
    contextp->commandArgs(argc, argv);

    Vtop* top = new Vtop{contextp};

    // nvboard_bind_all_pins(top);  // 引脚绑定
    // nvboard_init();
    VerilatedVcdC* m_trace = new VerilatedVcdC; // trace_object
    top->trace(m_trace, 99);  // 99表示记录最详细的信号信息
    m_trace->open("wave.vcd");  // 波形文件
    
    flag = 0;
    top->clk = 0;  // clk初始化
    // ----原reset函数----start
    int n = 10;
    top->rst = 1;
    while (n-- > 0){top->clk = !top->clk;top->eval();top->clk = !top->clk;top->eval();}
    top->rst = 0;
    //m_trace->dump(contextp->time()); // 记录波形
   // ----原reset函数----end
    // while (!contextp->gotFinish()) {
	// 	//nvboard_update();
    //     contextp->timeInc(1);  // 推进仿真时间
    //     top->clk = !top->clk; top->eval(); // eval()模型更新 可以理解为执行一次.v文件
    //     m_trace->dump(contextp->time()); // 记录波形
    //     top->inst = pmem_read(top->pc);
    //     top->eval();
    //     top->pc += 4;

    // }
    top->pc = 0x80000000;
    for(int i = 0; ; i++){
        if(flag == 1) break;
        top->inst = pmem_read(top->pc);
        top->pc = top->npc;
        top->clk = !top->clk; top->eval(); // eval()模型更新 可以理解为执行一次.v文件
        top->clk = !top->clk; top->eval(); // eval()模型更新 可以理解为执行一次.v文件
        m_trace->dump(contextp->time());
        contextp->timeInc(1);  
    }

    m_trace->close();
    delete top;
    delete contextp;
    return 0;
}

