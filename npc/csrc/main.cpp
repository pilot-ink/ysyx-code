#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Vtop.h"
#include "verilated.h"
//用来生成.vcd文件
#include <verilated_vcd_c.h>
#include <nvboard.h>

nvboard_bind_all_pins(&dut);
nvboard.init();

int main(int argc, char** argv)
{
	VerilatedContext* contextp = new VerilatedContext;
	contextp->commandArgs(argc, argv);
	Vtop* top = new Vtop{contextp};
	
	VerilatedVcdC *tfp = new VerilatedVcdC; //初始化VCD对象指针
	contextp->traceEverOn(true);			//打开追踪
	top->trace(tfp, 0);
	tfp->open("wave.vcd");

	while(!contextp->gotFinish()){
		int a = rand() & 1;
		int b = rand() & 1;
		top->a = a;
		top->b = b;
		top->eval();
		printf("a = %d, b = %d, c = %d\n", a, b, top->f);

		tfp->dump(contextp->time());		//dump wave
		contextp->timeInc(1);			//dump wave

		assert(top->f == (a ^ b));
		nvboard_update();
	}
	delete top;
	tfp->close();
	delete contextp;
	nvboard_quit();
	return 0;
}
