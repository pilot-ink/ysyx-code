`timescale 1ns / 1ps
module sr(
	input reg in,
	input clock,
	output reg [7:0] q 
);
	initial
		q = 8'b1;
	

	always @(clock) begin
		q <= {in,q[7:1]};
	end


endmodule



module LFSR(
	input clock,
	output [7:0] result
);
		
	wire in;
	assign in = result[4] ^ result[3] ^ result[2] ^ result[0];
	sr s1(.in(in), .clock(clock),.q(result));
	
	

endmodule
