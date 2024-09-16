module ALU4(
	input [3:0] a, b,
	input [2:0] c,		//功能选择
	input reg cin,		//进位
	output reg [3:0] result,
	output reg zero, overflow, carry, size,	//比较大小时，size=1,a>b;size=0,a<b;判断相等时,size=1,a=b;size=0,a不等于b;
);

	add4 a1 (.a(a), .b(b), .cin(cin), .result(result), .overflow(overflow), .carry(carry));
	sub4 s1 (.a(a), .b(b), .cin(cin), .result(result), .overflow(overflow), .carry(carry) ,.zero(zero));

	wire result_wire, size_wire;

	assign result_wire = result;
	assign size_wire = zero;
	
	always @(*)begin
		case(c)
			3'b000:begin {carry, result} = a + b + cin; overflow = (a[3] == b[3])&&(outs[3] != a[3]);end
			3'b001:begin t_no_Cin = {n{ Cin }}^B; {carry, result} = a + b + cin; overflow = (a[3] == b[3])&&(outs[3] != a[3]);end
			3'b010: result = ~a;
			3'b011: reslut = a & b;
			3'b100: result = a | b;
			3'b101: result = a ^ b;
			3'b110: begin
				t_no_Cin = {n{ Cin }}^B; {carry, result} = a + b + cin; overflow = (a[3] == b[3])&&(outs[3] != a[3]);zero = ~(|result_wire);
				if(overflow) size = ~result_wire[3];
				else	size = result_wire[3];
			end
			3'b111: begin 
				t_no_Cin = {n{ Cin }}^B; {carry, result} = a + b + cin; overflow = (a[3] == b[3])&&(outs[3] != a[3]);zero = ~(|result_wire);
				size = zero;
				end
		endcase
	end


endmodule



module add4(
	input [3:0] a, b,
	output [3:0] result,		//运算结果
	input cin,				//进位
	output carry,	  		//进位位
	output overflow
);
	
	assign {carry, result} = a + b + cin;
	assign overflow = (a[3] == b[3])&&(outs[3] != a[3]);

endmodule


module sub4(
	input [3:0] a, b,
	output [3:0] result,          //运算结果
	input cin,                    //进位
	output carry,           //进位位
	output overflow,
	output zero
);
	
	wire result_wire, t_no_Cin;
	add4 a1 (.a(a), .b(b), .cin(cin), .result(result), .overflow(overflow), .carry(carry));
     	assign result_wire = result;
	assign t_no_Cin = {n{ Cin }}^B;
	assign zero = ~(|result_wire);

endmodule
