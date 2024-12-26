module ysyx_24100027_PC(
    input          clk,
    input   reg [31:0] pc,
    output  reg [31:0] npc,
    output  reg [31:0] inst
);
    import "DPI-C" function int pmem_read(input int raddr);
    import "DPI-C" function void npc_trap();

    initial begin
        npc[31:0] = 32'h80000000;
        inst[31:0] = pmem_read(npc);
    end
    
    always @(negedge clk) begin
        npc = pc;
        inst = pmem_read(npc);
        if(inst == 32'h00100073) begin
            npc_trap();
        end
    end


endmodule