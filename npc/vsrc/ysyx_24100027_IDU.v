module ysyx_24100027_IDU(
    input [6:0] opcode,
    input [2:0] funct3,
    input [6:0] funct7,
    output Regwr,
    output [3:0] Aluctr,
    output [2:0] Extop,
    output AluActr,
    output [1:0] AluBctr,
    output [2:0] Branch
);
    import "DPI-C" function void npc_trap();

    parameter INSTR_NUM = 6-1;
    wire [8:0] instr_type;
    assign instr_type[8:4] = opcode[6:2];

    always @(*) begin
        if(opcode == 7'b1110011) begin
            npc_trap();
        end
    end
    MuxKey #(INSTR_NUM, 5, 4) TypeMux(instr_type[3:0],opcode[6:2],{
        5'b01101,{4'b0000},          //lui
        5'b00101,{4'b0000},           //auipc
        5'b11011,{4'b0000},           //jal
        5'b11001,{func3[2:0],0},      //jalr
        5'b00100,{func3[2:0],0}        //addi
    });
    //000-I 001-U 010-S 011-B 100-J
    MuxKey #(INSTR_NUM, 9, 3) ExtopMux(Extop,instr_type[8:0],{
        9'b011010000, 3'b001,
        9'b001010000, 3'b001,
        9'b110110000, 3'b100,
        9'b110010000, 3'b000,
        9'b001000000, 3'b000
    });
    //1-w 0-non
    MuxKey #(INSTR_NUM, 9, 1) RegwrMux(Regwr,instr_type[8:0],{
        9'b011010000, 1'b1,
        9'b001010000, 1'b1,
        9'b110110000, 1'b1,
        9'b110010000, 1'b1,
        9'b001000000, 1'b1
    });
    //
    MuxKey #(INSTR_NUM, 9, 4) AluctrMux(Aluctr,instr_type[8:0],{
        9'b011010000, 4'b0011,
        9'b001010000, 4'b0000,
        9'b110110000, 4'b0000,
        9'b110010000, 4'b0000,
        9'b001000000, 4'b0000
    });
    //1-pc 0-busA
    MuxKey #(INSTR_NUM, 9, 1) AluActrMux(AluActr,instr_type[8:0],{
        9'b011010000, 1'b0,
        9'b001010000, 1'b1,
        9'b110110000, 1'b1,
        9'b110010000, 1'b1,
        9'b001000000, 1'b0
    });
    //00-busB 01-imm 10-4
    MuxKey #(INSTR_NUM, 9, 2) AluBctrMux(AluBctr,instr_type[8:0],{
        9'b011010000, 2'b01,
        9'b001010000, 2'b01,
        9'b110110000, 2'b10,
        9'b110010000, 2'b10,
        9'b001000000, 2'b01
    });

    MuxKey #(INSTR_NUM, 9, 3) BranchMux(Branch,instr_type[8:0],{
        9'b011010000, 3'b000,
        9'b001010000, 3'b000,
        9'b110110000, 3'b001,
        9'b110010000, 3'b010,
        9'b001000000, 3'b000
    });
endmodule