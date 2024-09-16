module encode42(x,en,y);
  input  [3:0] x;
  input  en;
  output reg [1:0] y;
  integer i;
  always @(x or en) begin
    if (en) begin
      y = 0;
      for( i = 0; i <= 3; i = i+1)
          if(x[i] == 1)  y = i[1:0];
    end
    else  y = 0;
  end
endmodule

module encode83(
	input [7:0]x,
	input en,
	output reg [2:0]y,
	output reg l);
/* verilator lint_off CASEOVERLAP */
	always @(x or en) begin
		if(en) begin
		casez(x)
			8'bzzzz_zzz1:begin  y = 3'b000; l = 1;end
			8'bzzzz_zz1z:begin  y = 3'b001; l = 1;end
			8'bzzzz_z1zz:begin  y = 3'b010; l = 1;end
			8'bzzzz_1zzz:begin  y = 3'b011; l = 1;end
			8'bzzz1_zzzz:begin  y = 3'b100; l = 1;end
			8'bzz1z_zzzz:begin  y = 3'b101; l = 1;end
			8'bz1zz_zzzz:begin  y = 3'b110; l = 1;end
			8'b1zzz_zzzz:begin  y = 3'b111; l = 1;end
			default:begin y = 3'b000; l = 0; end
		endcase
	end
	end
/* verilator lint_on CASEOVERLAP */
endmodule
