module ysyx_24100027_GPR(
  input clk,
  input WE3,
  input [4:0] rs1,rs2,rd,
  input [31:0] WD3,
  output [31:0] RD1, RD2
);
    RegisterFile #(5, 32) gr (
        .clk(clk),
        .waddr(rd),
        .wdata(WD3),
        .wen(WE3),
        .raddr_1(rs1),
        .raddr_2(rs2),
        .rdata_1(RD1),
        .rdata_2(RD2)
    );
endmodule