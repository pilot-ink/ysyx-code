module ysyx_24100027_CPU(
    input rst,
    input clk
);

    wire [31:0] inst;
    wire [31:0] pc;
    wire [31:0] npc;

    wire regwr;
    wire [31:0] busA,busB,imm,busW,rs1,rs2,PCA_out,PCB_out,result,DataOut;
    wire [2:0] extop;
    wire [3:0] ALUCTR;
    wire ALUActr;
    wire [1:0] ALUBctr;
    wire less,zero,PCActr,PCBctr;
    wire [2:0] branch;
    wire Mem_wen;

    wire MemtoReg,MemWr;
    wire [2:0] MemOP;

    wire [31:0] ALUA,ALUB;

    assign rs1=busA;
    assign rs2=busB;
    
    assign npc=PCA_out+PCB_out;


    ysyx_24100027_MEM mem1(
        .clk(clk),
        .Addr(result),
        .MemOP(MemOP),
        .DataIn(busB),
        .WrEn(MemWr),
        .wen(Mem_wen),
        .DataOut(DataOut)
    );

    ysyx_24100027_PC p1(
        .clk(clk),
        .pc(npc),
        .npc(pc),
        .inst(inst)
    );

    //good to go
    ysyx_24100027_GPR gpr1(
        .clk(clk),
        .regwr(regwr),
        .rs1(inst[19:15]),
        .rs2(inst[24:20]),
        .rd(inst[11:7]),
        .busW(busW),
        .busA(busA),
        .busB(busB)
        );
    
    ysyx_24100027_ALU alu1(
        .a(ALUA),
        .b(ALUB),
        .Aluctr(ALUCTR),
        .ALUout(result),
        .less(less),
        .zero(zero)
    );
    //good to go
    ysyx_24100027_IMM imm1(
        .inst(inst),
        .extop(extop),
        .imm(imm)
    );
    ysyx_24100027_IDU idu1(
        .clk(clk),
        .opcode(inst[6:0]),
        .funct3(inst[14:12]),
        .funct7(inst[31:25]),
        .Regwr(regwr),
        .Aluctr(ALUCTR),
        .Extop(extop),
        .MemtoReg(MemtoReg),
        .MemWr(MemWr),
        .MemOP(MemOP),
        .Mem_wen(Mem_wen),
        .AluActr(ALUActr),
        .AluBctr(ALUBctr),
        .Branch(branch)
    );
    ysyx_24100027_BRANCHCTR branch1(
        .less(less),
        .zero(zero),
        .branch(branch),
        .PCActr(PCActr),
        .PCBctr(PCBctr)
    );
    MuxKeyWithDefault #(4, 2, 32) ALUBmux(ALUB, ALUBctr, 32'h0,{
        2'b00, rs2,
        2'b01, imm,
        2'b10, 32'h4,
        2'b11, 32'h0
    });
    MuxKeyWithDefault #(2, 1, 32) ALUAmux(ALUA, ALUActr, 32'h0,{
        1'b0, rs1,
        1'b1, pc
    });
    MuxKeyWithDefault #(2, 1, 32) PCAmux(PCA_out, PCActr, 32'h0,{
        1'b0, 32'h4,
        1'b1, imm
    });
    MuxKeyWithDefault #(2, 1, 32) PCBmux(PCB_out, PCBctr, 32'h0,{
        1'b0, pc,
        1'b1, rs1
    });
    MuxKeyWithDefault #(2, 1, 32) endmux(busW, MemtoReg, 32'h0,{
        1'b0, result,
        1'b1, DataOut
    });
endmodule