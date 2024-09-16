module top(
	input [3:0] a, b;
       input [2:0] c;      //功能选择
       input reg cin;      //进位
       output reg [3:0] result;
       output reg zero, overflow, carry, size; //比较大小时，size=1,a>b;size=0,a<b;判断相等时,size=1,a=b;size=0,a不等于b;

);
	
	
	ALU4(.a(a), .b(b), .c(c), .cin(cin), .result(result), ,zero(zero),.overflow(overflow), .carry(carry), .size(size));

endmodule
