module ALU4(
	input [3:0] a, b,
	input [2:0] c,		//功能选择
	input reg cin,		//进位
	output reg [3:0] result,
	output reg zero, overflow, carry, size,	//比较大小时，size=1,a>b;size=0,a<b;判断相等时,size=1,a=b;size=0,a不等于b;
);



	wire result_wire, size_wire;
	wire [3:0] t_no_cin;
	assign t_no_cin = {4{cin}}^b;

	assign result_wire = result;
	assign size_wire = zero;
	
	always @(*)begin
		case(c)
			3'b000:begin {carry, result} = a + b + cin; overflow = (a[3] == b[3])&&(result[3] != a[3]);end
			3'b001:begin {carry, result} = a + t_no_Cin + {3'b000, cin}; overflow = (a[3] == b[3])&&(result[3] != a[3]);end
			3'b010: result = ~a;
			3'b011: result = a & b;
			3'b100: result = a | b;
			3'b101: result = a ^ b;
			3'b110: begin
				{carry, result} = a + t_no_Cin + {3'b000, cin}; overflow = (a[3] == b[3])&&(result[3] != a[3]);zero = ~(|result);
				if(overflow) size = ~result_wire[3];
				else	size = result_wire[3];
			end
			3'b111: begin 
				{carry, result} = a + t_no_Cin + {3'b000, cin}; overflow = (a[3] == b[3])&&(result[3] != a[3]);zero = ~(|result);
				size = zero;
				end
		endcase
	end


endmodule
