module top(
	input [7:0]x,
	input en,
	output [2:0]y,
	output l,
	output [6:0] h
);
	
	
	encode83 i0(.x(x), .en(en), .y(y), .l(l));
	/* verilator lint_off WIDTHEXPAND */
	bcd7seg  i1(.b(y), .h(h));
	/* verilator lint_on WIDTHEXPAND */

endmodule
