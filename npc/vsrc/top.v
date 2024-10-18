
/*线性反馈移位寄存器*/
// module top(
// 	input clk,rst,
// 	input clock,
// 	output [7:0] result,
// 	output [6:0] hex,hex1
// );
// 	wire [3:0] h,h1;
// 	assign h = result[3:0];
// 	assign h1 = result[7:4];

// 	LFSR l1(.clock(clock), .result(result));
// 	bcd7seg b1(.b(h), .h(hex));
// 	bcd7seg b2(.b(h1), .h(hex1));

// endmodule


/*ALU*/
// module top(
//   input clk,rst,
//   input  wire [3:0] A,
//   input  wire [3:0] b,
//   input  wire [2:0] option,
//   output reg        carry,
//   output reg        overflow,
//   output reg        zero,
//   output reg  [3:0] result,
//   output 	  [6:0] hex
// );

// 	wire [3:0] h;
// 	assign h = result;

// 	ALU4 a1(.A(A), .b(b), .option(option), .carry(carry), .overflow(overflow), .zero(zero), .result(result));
// 	bcd7seg b1(.b(h), .h(hex));

// endmodule



/*encode83*/
module top(
	x,en,y,l,hex,clk,rst
);
	input clk,rst;
	input [7:0]x;
	input en;
	output reg [2:0]y;
	output reg l;
	output [6:0] hex;

	wire [3:0] h;
	assign h = {1'b0,y[2:0]};

	encode83 e1(.x(x), .en(en), .y(y), .l(l));
	bcd7seg  b1(.b(h), .h(hex));

endmodule



