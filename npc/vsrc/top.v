module top(
	input in,
       input clock,     
       output [7:0] result,
       output [3:0] h1, h2
);
	
	
	LFSR a1 (.in(in), .clock(clock), .result(result));
	bcd7se b1(.b(result[3:0]), .h(h1));
	bcd7se b2(.b(result[7:4]), .h(h2));

endmodule
