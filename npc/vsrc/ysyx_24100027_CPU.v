module ysyx_24100027_CPU(
    input rst,
    input clk,
    input [31:0] pc,
    input [31:0] inst
);
    wire WD3,WE3;
    wire [31:0] RD1,RD2,immtalu;
    wire [2:0] immtype;
    wire [2:0] alucon;
    ysyx_24100027_GPR c1(
        .clk(clk),
        .waddr(inst[11:7]),
        .wdata(WD3),
        .wen(WE3),
        .raddr_1(inst[19:15]),
        .raddr_2(inst[24:20]),
        .rdata_1(RD1),
        .rdata_2(RD2),
        );
    ysyx_24100027_ALU a1(
        .a(RD1),
        .b(immtalu),
        .alucontrol(alucon),
        .result(WD3),
        .of(),
        .zf(),
        .nf(),
        .cf(),
    );
    ysyx_24100027_IMM i1(
        .inst(inst),
        .immtype(immtype),
        .immsext(immtalu),
    );
    ysyx_24100027_IDU i2(
        .opcode(inst[6:0]),
        .funct3(inst[14:12]),
        .alucontrol(alucon),
        .Regwrite(WE3),
        .immtype(immtype),
    );
endmodule