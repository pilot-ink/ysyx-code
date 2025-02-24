module ysyx_24100027_IDU(
    input clk,
    input [6:0] opcode,
    input [2:0] funct3,
    input [6:0] funct7,
    output Regwr,
    output [3:0] Aluctr,
    output [2:0] Extop,
    output AluActr,
    output MemtoReg,
    output MemWr,
    output [2:0] MemOP,
    output Mem_wen,
    output [1:0] AluBctr,
    output [2:0] Branch
);
    initial begin
        $readmemh("/home/dhb/ysyx-workbench/npc/vsrc/data/instr_ctr_data.txt", rom);
    end

    parameter INSTR_NUM = 37; //40 - ebreak -fence - csr
    wire [8:0] instr_type;
    assign instr_type[8:4] = opcode[6:2];
    wire [6:0] inst_decode;
    reg [7:0] rom[110:0];

    wire [23:0] inst_decode_data;
    assign inst_decode_data[23:0] = {rom[inst_decode[6:0]],rom[inst_decode[6:0]+7'h1],rom[inst_decode[6:0]+7'h2]};

    assign Extop    = inst_decode_data[23:21];
    assign Regwr    = inst_decode_data[20];
    assign Branch   = inst_decode_data[19:17];
    assign MemtoReg = inst_decode_data[16];
    assign MemWr    = inst_decode_data[15];
    assign MemOP    = inst_decode_data[14:12];
    assign AluActr  = inst_decode_data[11];
    assign AluBctr  = inst_decode_data[10:9];
    assign Aluctr   = inst_decode_data[8:5];


    MuxKeyWithDefault #(2, 5, 1) IDU_WEN(Mem_wen, opcode[6:2], 1'b0,{
        5'b00000, 1'b1,
        5'b01000, 1'b1
    });

    MuxKeyWithDefault #(3, 5, 3) preprocess(instr_type[3:1],opcode[6:2], funct3[2:0],{
        5'b01101, 3'b000,            //lui
        5'b00101, 3'b000,            //auipc
        5'b11011, 3'b000           //jal
    });
    MuxKeyWithDefault #(24, 8, 1) TypeMux1(instr_type[0],instr_type[8:1],funct7[5],{
        {5'b01101,{3'b000}},    1'b0,              //lui
        {5'b00101,{3'b000}},    1'b0,             //auipc
        {5'b00100,{3'b000}},    1'b0,              //addi
        {5'b00100,{3'b010}},    1'b0,             //slti
        {5'b00100,{3'b011}},    1'b0,             //sltiu
        {5'b00100,{3'b100}},    1'b0,             //xori
        {5'b00100,{3'b110}},    1'b0,              //ori
        {5'b00100,{3'b111}},    1'b0,            //andi
        {5'b11011,{3'b000}},    1'b0,            //jal
        {5'b11001,{3'b000}},    1'b0,            //jalr
        {5'b11000,{3'b000}},    1'b0,            //beq
        {5'b11000,{3'b001}},    1'b0,            //bne
        {5'b11000,{3'b100}},    1'b0,           //blt
        {5'b11000,{3'b101}},    1'b0,            //bge
        {5'b11000,{3'b110}},    1'b0,            //bltu
        {5'b11000,{3'b111}},    1'b0,           //bgeu
        {5'b00000,{3'b000}},    1'b0,           //lb
        {5'b00000,{3'b001}},    1'b0,            //lh
        {5'b00000,{3'b010}},    1'b0,            //lw
        {5'b00000,{3'b100}},    1'b0,            //lbu
        {5'b00000,{3'b101}},    1'b0,            //lhu
        {5'b01000,{3'b000}},    1'b0,            //sb
        {5'b01000,{3'b001}},    1'b0,            //sh
        {5'b01000,{3'b010}},    1'b0             //sw
    });
    MuxKey #(INSTR_NUM, 9, 7) TypeMux2(inst_decode[6:0],instr_type[8:0],{
        {5'b01101,{4'b0000}},        7'd0,              //lui
        {5'b00101,{4'b0000}},        7'd3,             //auipc
        {5'b00100,{3'b000,1'b0}},    7'd6,              //addi
        {5'b00100,{3'b010,1'b0}},    7'd9,             //slti
        {5'b00100,{3'b011,1'b0}},    7'd12,             //sltiu
        {5'b00100,{3'b100,1'b0}},    7'd15,             //xori
        {5'b00100,{3'b110,1'b0}},    7'd18,              //ori
        {5'b00100,{3'b111,1'b0}},    7'd21,            //andi
        {5'b00100,{3'b001,1'b0}},    7'd24,             //slli
        {5'b00100,{3'b101,1'b0}},    7'd27,            //srli
        {5'b00100,{3'b101,1'b1}},    7'd30,             //srai
        {5'b01100,{3'b000,1'b0}},    7'd33,             //add
        {5'b01100,{3'b000,1'b1}},    7'd36,             //sub
        {5'b01100,{3'b001,1'b0}},    7'd39,            //sll
        {5'b01100,{3'b010,1'b0}},    7'd42,            //slt
        {5'b01100,{3'b011,1'b0}},    7'd45,            //sltu
        {5'b01100,{3'b100,1'b0}},    7'd48,            //xor
        {5'b01100,{3'b101,1'b0}},    7'd51,             //srl
        {5'b01100,{3'b101,1'b1}},    7'd54,             //sra
        {5'b01100,{3'b110,1'b0}},    7'd57,             //or
        {5'b01100,{3'b111,1'b0}},    7'd60,            //and
        {5'b11011,{4'b0000}},        7'd63,            //jal
        {5'b11001,{3'b000,1'b0}},    7'd66,            //jalr
        {5'b11000,{3'b000,1'b0}},    7'd69,            //beq
        {5'b11000,{3'b001,1'b0}},    7'd72,            //bne
        {5'b11000,{3'b100,1'b0}},    7'd75,           //blt
        {5'b11000,{3'b101,1'b0}},    7'd78,            //bge
        {5'b11000,{3'b110,1'b0}},    7'd81,            //bltu
        {5'b11000,{3'b111,1'b0}},    7'd84,           //bgeu
        {5'b00000,{3'b000,1'b0}},    7'd87,           //lb
        {5'b00000,{3'b001,1'b0}},    7'd90,            //lh
        {5'b00000,{3'b010,1'b0}},    7'd93,            //lw
        {5'b00000,{3'b100,1'b0}},    7'd96,            //lbu
        {5'b00000,{3'b101,1'b0}},    7'd99,            //lhu
        {5'b01000,{3'b000,1'b0}},    7'd102,            //sb
        {5'b01000,{3'b001,1'b0}},    7'd105,            //sh
        {5'b01000,{3'b010,1'b0}},    7'd108             //sw
    });
endmodule