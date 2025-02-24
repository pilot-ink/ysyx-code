module ysyx_24100027_MEM(
    input clk,
    input reg [31:0] Addr,
    input [31:0] DataIn,
    input [2:0] MemOP,
    input WrEn,
    input wen,
    output [31:0] DataOut
);
    import "DPI-C" function int pmem_read(input int raddr);
    import "DPI-C" function void pmem_write(
        input int waddr, input int wdata, input byte wmask);

    reg [31:0] data_r;
    reg [7:0] wmask;
    MuxKey #(3, 3, 8) m1(wmask, MemOP,{
        3'b000, 8'h01,
        3'b001, 8'h02,
        3'b010, 8'h04
    });
    MuxKey #(5, 3, 32) m2(DataOut, MemOP,{
        3'b000, {{24{data_r[31]}}, data_r[7:0]},
        3'b001, {{16{data_r[15]}},data_r[15:0]},
        3'b010, data_r[31:0],
        3'b100, {24'h0,data_r[7:0]},
        3'b101, {16'h0,data_r[15:0]}
    });
    
    always @(posedge clk) begin
        if(wen) begin
            if(WrEn) begin
                pmem_write(Addr, DataIn, wmask);
                data_r = 32'h0;
            end
            else begin
                data_r = pmem_read(Addr);
            end
        end
        else begin
            data_r = 32'h0; 
        end
    end


endmodule