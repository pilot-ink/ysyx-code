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
        .WE3(WE3),
        .rs1(inst[19:15]),
        .rs2(inst[24:20]),
        .rd(inst[11:7]),
        .WD3(WD3),
        .RD1(RD1),
        .RD2(RD2),
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