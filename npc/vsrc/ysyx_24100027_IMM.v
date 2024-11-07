module ysyx_24100027_IMM(
    input [31:0] inst,
    input [2:0] immtype,
    output [31:0] immsext
);
    wire [31:0] immI;
    assign immI = {{20{inst[31]}},inst[31:20]};
    assign immsext = immI;
endmodule