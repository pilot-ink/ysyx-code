module ALU4(
	input [3:0] a, b,
	input [2:0] c,		//功能选择
	input reg cin,		//进位
	output reg [3:0] result,
	output reg zero, overflow, carry
);
	initial
		cin = 0;

	wire [3:0] 	a_s;
	wire		cin_next;
	reg [3:0]	B;
	
	chose_xor a1(.b(b), .c(c), .B(B));
	
	assign {cin_next, a_s} = a + B + cin; 
	 //carry
  	always @(*) begin
   		case (option)
      			3'b000: carry = a_cin;  //加法
      			3'b001: carry = a_cin;  //减法
      			3'b010: carry = 1'b0;   //取反
      			3'b011: carry = 1'b0;   //与
      			3'b100: carry = 1'b0;   //或
      			3'b101: carry = 1'b0;   //异或
      			3'b110: carry = a_cin;  //比较大小
      			3'b111: carry = a_cin;  //判断相等
     			 default: ;
    		endcase
    		
    	 //overflow
  	reg overflow_temp;
  	assign overflow_temp = (a[3]==B[3]) && (a[3]!=a_s[3]);
    		
    	always @(&) begin
    		case (option)
     		 	3'b000: overflow = overflow_temp;  //带符号加法
      			3'b001: overflow = overflow_temp;  //带符号减法
      			3'b010: overflow = 1'b0;           //取反
      			3'b011: overflow = 1'b0;           //与
      			3'b100: overflow = 1'b0;           //或
      			3'b101: overflow = 1'b0;           //异或
      			3'b110: overflow = overflow_temp;  //带符号比较大小
      			3'b111: overflow = overflow_temp;  //判断相等
      			default: ;
    		endcase  
    	end 
    	assign zero = ~(|a_s);

  	//result
  	always @(*) begin
    		case (option)
      			3'b000: result = a_s;                              //加法
      			3'b001: result = a_s;                              //减法
      			3'b010: result = a ^ 4'hf;                         //取反
      			3'b011: result = a & B;                            //与
      			3'b100: result = a | B;                            //或
      			3'b101: result = a ^ B;                            //异或
      			3'b110: result = {3'd0, a_s[3] ^ overflow_temp};   //比较大小
      			3'b111: result = {3'd0, {zero}};                   //判断相等
      			default: ;
    			endcase    
  	end
    	
    	   
  end


endmodule







module chose_xor(
	input [3:0] b;
	input [2:0] c;
	output reg [3:0] B;
);

	reg [3:0] temp = b ^ 4'b1111;
	
	always @(*) begin
		case(c)
			3'b000: B = b;      //加法
            		3'b001: B = temp;   //减法
            		3'b010: B = b;      //取反
            		3'b011: B = b;      //与
            		3'b100: B = b;      //或
            		3'b101: B = b;      //异或
            		3'b110: B = temp;   //比较大小
            		3'b111: B = temp;   //判断相等
		endcase
	end





endmodule
