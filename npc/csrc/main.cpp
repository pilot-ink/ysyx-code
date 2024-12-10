#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#//include<nvboard.h>
#include"Vtop.h"  // 从top.v生成
#include"verilated.h"
#include"verilated_vcd_c.h"

int flag = 0;
/*static uint8_t pmem[] = {
   0x00, 0x50, 0x00, 0x93,//addi x1 x0 5
    0x00, 0x10, 0x01, 0x13,//addi x2 x0 1
    0x00, 0x20, 0x01, 0x13,//addi x2 x0 2
    0x00, 0x50, 0x81, 0x13,//addi x2 x1 5
    0x00, 0x10, 0x00, 0x53,//ebreak
};*/
static uint8_t pmem[] = {
    0x93, 0x00, 0x50, 0x00,//addi x1 x0 5
    0x13, 0x01, 0x10, 0x00,//addi x2 x0 1
    0x13, 0x01, 0x20, 0x00,//addi x2 x0 5
    0x13, 0x81, 0x50, 0x00,//addi x2 x1 5
    0x73, 0x00, 0x10, 0x00,//ebreak
};
uint32_t pmem_read(uint32_t pc)
{
    pc -= 0x80000000;
    uint8_t *ptr = pmem;
    ptr += pc;
    uint32_t *value = (uint32_t *)ptr;
    return *value;
}

extern "C" void npc_trap(){
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
    for(int i = 0; flag != 1; i++){
        top->inst = pmem_read(top->pc);
        top->clk = !top->clk; top->eval(); // eval()模型更新 可以理解为执行一次.v文件
        //top->clk = !top->clk; top->eval(); // eval()模型更新 可以理解为执行一次.v文件
        m_trace->dump(contextp->time());
        contextp->timeInc(1);  
        top->pc = top->npc;
        //if(flag == 1) break;
    }
    m_trace->dump(contextp->time());

    m_trace->close();
    delete top;
    delete contextp;
    return 0;
}

