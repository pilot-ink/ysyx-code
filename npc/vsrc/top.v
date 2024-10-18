
/*线性反馈移位寄存器*/
module top(
	input clk,rst,
  input [3:0] data[1:0],
  input [1:0] choose,
	output [7:0] result
);
	MuxKey #(4, 2, 2) i0 (data , choose, {
    2'b00, data[0],
    2'b01, data[1],
    2'b10, data[2],
    2'b11, data[3]
  });

endmodule







