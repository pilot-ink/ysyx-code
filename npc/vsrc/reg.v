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
  input [DATA_WIDTH-1:0] busW,
  input [ADDR_WIDTH-1:0] RD,
  input wen,
  input [ADDR_WIDTH-1:0] RS1,RS2,
  output [DATA_WIDTH-1:0] busA,busB
);
  reg [DATA_WIDTH-1:0] rf [2**ADDR_WIDTH-1:0];
  assign busA = rf[RS1];
  assign busB = rf[RS2];
  always @(posedge clk) begin
    if (wen) rf[RD] <= busW;
    rf[0] <= 32'h0;
  end
endmodule