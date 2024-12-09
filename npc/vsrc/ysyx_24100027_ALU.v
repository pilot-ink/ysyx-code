module ysyx_24100027_ALU(
    input [31:0] a,
    input [31:0] b,
    input [3:0] Aluctr,
    output [31:0] ALUout,
    output less,
    output zero
);
    wire [31:0] ab_xor;
    wire [31:0] ab_or;
    wire [31:0] ab_and;

    wire [31:0] B;
    wire [31:0] adder;
    wire        carry;
    //alu ctr unit
    wire [2:0] Aluctr_out;
    wire AL;
    wire LR;
    wire US;
    wire SubAdd;

    wire [31:0] result;
    wire overflow;

    wire [31:0] shift;

    ALUCTR a1(.Aluctr_in(Aluctr), .Aluctr_out(Aluctr_out), .AL(AL), .LR(LR), .US(US),.SubAdd(SubAdd));
    complement com(.b(b), .option(SubAdd), .B(B));
    barrel_shifter barrel1(.Din(a), .Shanmt(b[4:0]), .LR(LR), .AL(AL), .shift(shift));
    MuxKey #(8,3,32) resultmux(ALUout, Aluctr_out,{
        3'b000, adder,
        3'b001, shift,
        3'b010, {31'h0,less},
        3'b011, B,
        3'b100, ab_xor,
        3'b101, shift,
        3'b110, ab_or,
        3'b111, ab_and
    });
    MuxKey #(2,1,1) lessmux(less, US,{
        1'b0, overflow ^ result[31],
        1'b1, carry ^ SubAdd
    });
    //xor or and
    assign ab_xor   = a ^ b;
    assign ab_or    = a | b;
    assign ab_and   = a & b;
    //add sub
    assign {carry, adder} = a + B + SubAdd;
    assign zero = ~(|adder);
    assign overflow = (a[31]==B[31]) && (a[31]!=adder[31]);
endmodule


module ALUCTR(
    input [3:0] Aluctr_in,
    output [2:0] Aluctr_out,
    output AL,
    output LR,
    output US,
    output SubAdd
);
    assign Aluctr_out = Aluctr_in[2:0];
    //1-sub 0-add 
    MuxKeyWithDefault #(4, 4, 1) SubAddmux(SubAdd, Aluctr_in, 1'b0,{
        4'b0000, 1'b0,
        4'b1000, 1'b1,
        4'b0010, 1'b1,
        4'b1010, 1'b1
    });
    //1-left shift   0-right shift
    //1-        0-LOGIC
    MuxKeyWithDefault #(4, 4, 1) ALmux(AL, Aluctr_in,1'b0,{
        4'b0001, 1'b0,
        4'b1001, 1'b0,
        4'b0101, 1'b1,
        4'b1101, 1'b1
    });
    MuxKeyWithDefault #(4, 4, 1) LRmux(LR, Aluctr_in,1'b0,{
        4'b0001, 1'b1,
        4'b1001, 1'b1,
        4'b0101, 1'b0,
        4'b1101, 1'b0
    });
    //1-usigned 0-signed
    MuxKeyWithDefault #(2, 4, 1) USmux(US, Aluctr_in,1'b0,{
        4'b0010, 1'b0,
        4'b1010, 1'b1
    });
endmodule

module complement(
  input [31:0] b,
  input option,
  output [31:0] B
);
    assign B = {32{option}} ^ b;
endmodule

module barrel_shifter(
    input Din,
    input [4:0] Shanmt,
    input LR,
    input AL,
    output [31:0] shift
);
    assign shift = 32'h0;

endmodule