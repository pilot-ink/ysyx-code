module bcd7seg(
	input [3:0] b,
	output reg [6:0] h
);

	always @(b) begin
		case(b)
			4'b000 : h = 7'b1000000;
			4'b001 : h = 7'b1111001;
			4'b010 : h = 7'b0100100;
			4'b011 : h = 7'b0110000;
			4'b100 : h = 7'b0011001;
			4'b101 : h = 7'b0010010;
			4'b110 : h = 7'b0000010;
			4'b111 : h = 7'b1111000;
			default : h = 0;
		endcase
	end

endmodule
