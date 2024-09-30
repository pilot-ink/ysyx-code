module sr(
	input reg in,
	output reg [7:0] q 
);
	initial
		q = 8'b1;
	

	always @(in) begin
		q <= {in,q[7:1]};
	end


endmodule



module LFSR(
	input clock,
	output [7:0] result
);
		
	reg in;
	sr s1(.in(in), .q(result));
	
	if(clock == 1)
		in <= result[4] ^ result[3] ^ result[2] ^ result[0];
	

endmodule
