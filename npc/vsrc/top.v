module top(clk,rst,ps2_clk,ps2_data,
	data,ready,overflow,hex0
);
	input clk,rst,ps2_clk,ps2_data;
	output [7:0] data;
	output ready;
	output overflow;
	output [6:0] hex0;
	
	reg [7:0] data_p;
	reg nextdata_n;
	
	
	ps2_keyboard inst(.clk(clk), .clrn(rst), .ps2_clk(ps2_clk), .ps2_data(ps2_data), .data(data),.nextdata_n(nextdata_n),.ready(ready), .overflow(overflow));
	
	bcd7seg b1(.b(data_p[3:0]), .h(hex0));
	always @(*) begin
		if(ready) begin
		data_p = data;
		nextdata_n = 0;
		end
		else begin
		nextdata_n = 1;
		end
	end 

endmodule


