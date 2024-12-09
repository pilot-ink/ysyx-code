module ysyx_24100027_IMM(
    input [31:7] inst,
    input [2:0] extop,
    output [31:0] imm
);
    MuxKey #(5, 3 ,32) IMMMUx(imm,extop,{
        3'b000,{{20{inst[31]}},inst[31:20]},
        3'b001,{inst[31:12], 12'h0},
        3'b010,{{20{instr[31]}}, instr[31:25], instr[11:7]},
        3'b011, {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0},
        3'b100,{{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0}
    });
endmodule