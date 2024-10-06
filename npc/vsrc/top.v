module top(
     input clk,clrn,ps2_clk,ps2_data,
     input nextdata_n,  
     output [7:0] data,
     output reg ready,
     output reg overflow,
     output [6:0] h1,h2
);
	
	wire [7:0] data1;
	assign data1 = data;
	
	ps2_keyboard inst(.clk(clk), .clrn(clrn), .ps2_clk(ps2_clk), .ps2_data(ps2_data), .nextdata_n(nextdata_n), .data(data), .ready(ready), .overflow(overflow));
		

	bcd7seg b1(.b(data1[3:0]), .h(h1));
	bcd7seg b2(.b(data1[7:4]), .h(h2));

endmodule
