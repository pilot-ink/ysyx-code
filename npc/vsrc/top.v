module top(  // 这里名字要与Makefile中的TOPNAME和cpp文件中的#include "Vtop.h"相同
  input clk,
  input rst,
  output reg [15:0] led
);
  reg [31:0] count;
  always @(posedge clk) begin
    if (rst) begin led <= 1; count <= 0; end
    else begin
      if (count == 0) led <= {led[14:0], led[15]}; // 后四位：0001->0010->0100->1000 为1的位亮灯
      count <= (count >= 5000000 ? 32'b0 : count + 1); // 控制亮灯间隔
    end
  end
endmodule


