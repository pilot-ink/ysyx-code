module ysyx_24100027_ALU(
    input [31:0] a,
    input [31:0] b,
    input [2:0] alucontrol,
    output [31:0] result,
    output of,
    output zf,
    output nf,
    output cf
);
    wire [31:0] raddsub;
    wire [31:0] frand;
    wire [31:0] ror;
    wire [31:0] b2;
    assign b2 = b^{32{alucontrol[0]}};
    assign {cf,raddsub} = a + b2 + alucontrol[0];
    assign of = (~(a[31]^b2[31]))&(a[31]^raddsub[31]);
    assign rand = a^b;
    assign ror = a|b;
    MuxKey #(4,3,32) alumux(result,alucontrol,{
        3'b000,raddsub,
        3'b001,radsub,
        3'b010,frand,
        3'b011,ror
    });
    assign zf = ~(|result);
    assign nf = result[31];
endmodule