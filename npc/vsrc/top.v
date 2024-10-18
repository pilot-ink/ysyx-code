
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
// module top(
// 	x,en,y,l,hex,clk,rst
// );
// 	input clk,rst;
// 	input [7:0]x;
// 	input en;
// 	output reg [2:0]y;
// 	output reg l;
// 	output [6:0] hex;

// 	wire [3:0] h;
// 	assign h = {1'b0,y[2:0]};

// 	encode83 e1(.x(x), .en(en), .y(y), .l(l));
// 	bcd7seg  b1(.b(h), .h(hex));

// endmodule


/*keyboard*/
module top(clk,rst,ps2_clk,ps2_data,
	data,ready,overflow,hex0,hex1,hex2,hex3,hex4,hex5
);
	input clk,rst,ps2_clk,ps2_data;
	output [7:0] data;
	output ready;
	output overflow;
	output [6:0] hex0;
	output [6:0] hex1;
	output [6:0] hex2;
	output [6:0] hex3;
	output [6:0] hex4;
	output [6:0] hex5;
	
	reg [23:0] data_p;
	reg nextdata_n;
	wire [7:0] asc_out;
	reg [7:0] count;
	
	
	ps2_keyboard inst(	.clk(clk), 
						.clrn(rst), 
						.ps2_clk(ps2_clk), 
						.ps2_data(ps2_data), 
						.data(data),
						.nextdata_n(nextdata_n),
						.ready(ready), 
						.overflow(overflow));
	keycode_to_ascii k1(.keycode(data), .ascii(asc_out));
	
	bcd7seg b1(.b(data_p[3:0]), .h(hex0));
	bcd7seg b2(.b(data_p[7:4]), .h(hex1));
	
	bcd7seg b3(.b(asc_out[3:0]), .h(hex2));
	bcd7seg b4(.b(asc_out[7:4]), .h(hex3));
	//bcd7seg b3(.b(data_p[11:8]), .h(hex2));
	//bcd7seg b4(.b(data_p[15:12]), .h(hex3));
	bcd7seg b5(.b(count[3:0]), .h(hex4));
	bcd7seg b6(.b(count[7:4]), .h(hex5));
	always @(posedge clk) begin
		if(ready) begin
		data_p = {data_p[15:8],data_p[7:0],data[7:0]};
			if(data == 8'hf0) begin
			end else begin
				count = count + 8'd1; 
			end
		nextdata_n = 0;
		end
		else begin
		nextdata_n = 1;
		end
	end 

endmodule


