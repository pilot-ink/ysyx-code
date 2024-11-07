module ysyx_24100027_IDU(
    input [6:0] opcode,
    input [2:0] funct3,
    output [2:0] alucontrol,
    output Regwrite,
    output [2:0] immtype
);
    wire [6:0] opcode;
    assign Regwrite = 1;
    assign immtype = 3'b000;
    assign alucontrol = 3'b000;
endmodule