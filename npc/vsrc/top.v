module top(
	x,en,y,l,hex
);
	input [7:0]x;
	input en;
	output reg [2:0]y;
	output reg l;
	output [6:0] hex;

	wire [3:0] hex;
	assign h = {0,y};

	encode83 e1(.x(x), .en(en), .y(y), .l(l));
	bcd7seg  b1(.b(h), .h(hex))

endmodule


/*keyboard*/
// module top(clk,rst,ps2_clk,ps2_data,
// 	data,ready,overflow,hex0,hex1,hex2,hex3,hex4,hex5
// );
// 	input clk,rst,ps2_clk,ps2_data;
// 	output [7:0] data;
// 	output ready;
// 	output overflow;
// 	output [6:0] hex0;
// 	output [6:0] hex1;
// 	output [6:0] hex2;
// 	output [6:0] hex3;
// 	output [6:0] hex4;
// 	output [6:0] hex5;
	
// 	reg [23:0] data_p;
// 	reg nextdata_n;
	
	
// 	ps2_keyboard inst(.clk(clk), .clrn(rst), .ps2_clk(ps2_clk), .ps2_data(ps2_data), .data(data),.nextdata_n(nextdata_n),.ready(ready), .overflow(overflow));
	
// 	bcd7seg b1(.b(data_p[3:0]), .h(hex0));
// 	bcd7seg b2(.b(data_p[7:4]), .h(hex1));
// 	bcd7seg b3(.b(data_p[11:8]), .h(hex2));
// 	bcd7seg b4(.b(data_p[15:12]), .h(hex3));
// 	bcd7seg b5(.b(data_p[19:16]), .h(hex4));
// 	bcd7seg b6(.b(data_p[23:20]), .h(hex5));
// 	always @(posedge clk) begin
// 		if(ready) begin
// 		data_p <= {data_p[15:8],data_p[7:0],data};
// 		nextdata_n <= 0;
// 		end
// 		else begin
// 		nextdata_n <= 1;
// 		end
// 	end 

// endmodule


