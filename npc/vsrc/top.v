`timescale 1ns / 1ps
module top(
     input clrn,ps2_clk,ps2_data,
     input nextdata_n,  
     output [7:0] data,
     output ready,
     output overflow,
     output [6:0] h1,h2,
     output sampling
);
	/* parameter */
	parameter [31:0] clock_period = 10;
	reg clk;
	wire [7:0] data1;
	wire nextdata,ready1;
	reg ps2clk;
	
	assign ps2clk = ps2_clk;
	
	assign nextdata = nextdata_n;
	assign ready1 = ready;
	assign data1 = data;
	
	ps2_keyboard inst(.clk(clk), .clrn(clrn), .ps2_clk(ps2_clk), .ps2_data(ps2_data), .nextdata_n(nextdata_n), .data(data), .ready(ready), .overflow(overflow), .sampling_indictor(sampling));
	
	read r1(.ready(ready1), .nextdata(nextdata));

	bcd7seg b1(.b(data1[3:0]), .h(h1));
	bcd7seg b2(.b(data1[7:4]), .h(h2));
	initial begin /* clock driver */
    		clk = 0;
    		forever
        		#(clock_period/2) clk = ~clk;
	end

endmodule
