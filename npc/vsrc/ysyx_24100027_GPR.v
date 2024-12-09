module ysyx_24100027_GPR(
  input clk,
  input regwr,
  input [4:0] rs1,rs2,rd,
  input [31:0] busW,
  output [31:0] busA, busB
);
    RegisterFile #(5, 32) gr (
        .clk(clk),
        .RD(rd),
        .busW(busW),
        .wen(regwr),
        .RS1(rs1),
        .RS2(rs2),
        .busA(busA),
        .busB(busB)
    );
endmodule