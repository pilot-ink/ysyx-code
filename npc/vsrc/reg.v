module Reg #(WIDTH = 1, RESET_VAL = 0) (
  input clk,
  input rst,
  input [WIDTH-1:0] din,
  output reg [WIDTH-1:0] dout,
  input wen
);
  always @(posedge clk) begin
    if (rst) dout <= RESET_VAL;
    else if (wen) dout <= din;
  end
endmodule



module RegisterFile #(ADDR_WIDTH = 1, DATA_WIDTH = 1) (
  input clk,
  input [DATA_WIDTH-1:0] wdata,
  input [ADDR_WIDTH-1:0] waddr,
  input wen,
  input [ADDR_WIDTH-1:0] raddr_1,raddr_2,
  output [DATA_WIDTH-1:0] rdata_1,rdata_2
);
  reg [DATA_WIDTH-1:0] rf [2**ADDR_WIDTH-1:0];
  assign rdata_1 = rf[raddr_1];
  assign rdata_2 = rf[raddr_2];
  always @(posedge clk) begin
    if (wen) rf[waddr] <= wdata;
    rf[0] = rf[0] & 0;
  end
endmodule