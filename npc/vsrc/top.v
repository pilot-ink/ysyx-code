module top(
       input clock, 
       input [7:0] a;    
       output [7:0] result,
       output [6:0] h1, h2
);
	wire [7:0] result1;
	
	assign result1 = result;
	
	LFSR a1 (.clock(clock), .result(result));
	bcd7seg b1(.b(a[3:0]), .h(h1));
	bcd7seg b2(.b(a[7:4]), .h(h2));

endmodule
