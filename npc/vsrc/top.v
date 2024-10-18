
/*线性反馈移位寄存器*/
module top(
	input clk,rst,
	output [15:0] led
);
	water_lamp w1(.clk(clk), .rst(rst), .led(led));

endmodule








