/*ALU*/
module top(
  input clk,rst,
  input  wire [3:0] A,
  input  wire [3:0] b,
  input  wire [2:0] option,
  output reg        carry,
  output reg        overflow,
  output reg        zero,
  output reg  [3:0] result,
);



	ALU4 a1(.A(A), .b(b), .option(option), .carry(carry), .overflow(overflow), .zero(zero), .result(result));

endmodule


