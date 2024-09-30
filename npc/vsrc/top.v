module top(
       input clock,     
       output [7:0] result,
       output [6:0] h1, h2
);
	wire result1;
	
	assign result1 = result;
	
	LFSR a1 (.clock(clock), .result(result));
	bcd7seg b1(.b(result[3:0]), .h(h1));
	bcd7seg b2(.b(result[7:4]), .h(h2));

endmodule
