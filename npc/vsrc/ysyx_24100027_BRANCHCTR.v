module ysyx_24100027_BRANCHCTR(
    input less,
    input zero,
    input [2:0] branch,
    output PCActr,
    output PCBctr 
);
    wire [4:0] branchctr;
    MuxKey #(8, 5, 5) branch1(branchctr, branch, {
        3'b000, 5'b00000,
        3'b001, 5'b00100,
        3'b010, 5'b01000,
        3'b011, 5'b01100,
        3'b100, {3'b100,zero,1'b0},
        3'b101, {3'b101,zero,1'b0},
        3'b110, {3'b110,1'b0,less},
        3'b111, {3'b111,1'b0,less}
    });
    MuxKey #(11, 5, 1) branchPCA(PCActr, branchctr, {
        5'b00000,1'b0,
        5'b00100,1'b1,
        5'b01000,1'b1,
        5'b10000,1'b0,
        5'b10010,1'b1,
        5'b10100,1'b1,
        5'b10110,1'b0,
        5'b11000,1'b0,
        5'b11001,1'b1,
        5'b11100,1'b1,
        5'b11101,1'b0
    });
    MuxKey #(11, 5, 1) branchPCB(PCBctr, branchctr, {
        5'b00000,1'b0,
        5'b00100,1'b0,
        5'b01000,1'b1,
        5'b10000,1'b0,
        5'b10010,1'b0,
        5'b10100,1'b0,
        5'b10110,1'b0,
        5'b11000,1'b0,
        5'b11001,1'b0,
        5'b11100,1'b0,
        5'b11101,1'b0
    });
endmodule