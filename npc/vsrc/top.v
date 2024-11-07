module top(
    input rst,
    input clk,
    input [31:0] pc,
    input [31:0] inst
);
    ysyx_24100027_CPU c1(
        .rst(rst),
        .clk(clk),
        .pc(pc),
        .inst(inst)
    );

endmodule

