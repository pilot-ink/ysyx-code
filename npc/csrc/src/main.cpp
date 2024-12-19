#include <stdio.h>
#include <assert.h>
#include "common.h"
#include "sdb.h"
#include "parse.h"
#include "difftest.h"
#include "reg.h"

//include<nvboard.h>


/*func */
long get_file_size(FILE *stream);
extern "C" void npc_trap();


/*global value*/
VerilatedContext* contextp ;
VerilatedVcdC* m_trace ; // trace_object
Vtop* top ;
int flag = 0;
uint8_t * pmem;
char *bin_file;
char *diff_so_file;
extern CPU_state cpu;


/*static uint8_t pmem[] = {
   0x00, 0x50, 0x00, 0x93,//addi x1 x0 5
    0x00, 0x10, 0x01, 0x13,//addi x2 x0 1
    0x00, 0x20, 0x01, 0x13,//addi x2 x0 2
    0x00, 0x50, 0x81, 0x13,//addi x2 x1 5
    0x00, 0x10, 0x00, 0x53,//ebreak
};*/

//void nvboard_bind_all_pins(Vtop* top);s
int main(int argc, char** argv) {
    parse_args(argc, argv);
    printf("binfile:%s\tdiff:%s\n",bin_file, diff_so_file);
    FILE *fp = fopen(bin_file,"r");
    if(fp == NULL) printf("wrong!\n");
    pmem = (uint8_t *)malloc(get_file_size(fp)*sizeof(char));
    if(fread(pmem, sizeof(char), get_file_size(fp), fp) != get_file_size(fp))
        printf("wrong!\n");
    printf("--------------\n");
    contextp = new VerilatedContext;
    m_trace = new VerilatedVcdC; // trace_object
    top = new Vtop{contextp};
    contextp->traceEverOn(true); // 开启波形 
    contextp->commandArgs(argc, argv);


    // nvboard_bind_all_pins(top);  // 引脚绑定
    // nvboard_init();
    
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
    init_difftest(diff_so_file, get_file_size(fp), 123);
    top->pc = 0x80000000;
    cpu.pc = 0x80000000;
    sdb_mainloop();
    m_trace->dump(contextp->time());

    m_trace->close();
    fclose(fp);
    free(pmem);
    delete top;
    delete contextp;
    return 0;
}


long get_file_size(FILE *stream)
{
	long file_size = -1;
	long cur_offset = ftell(stream);	// 获取当前偏移位置
	fseek(stream, 0, SEEK_END);
	file_size = ftell(stream);	// 获取此时偏移值，即文件大小
	fseek(stream, cur_offset, SEEK_SET);
	return file_size;
}

extern "C" void npc_trap(){
    flag = 1;
    printf("HIT GOOD\n");
}


