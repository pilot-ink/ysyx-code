module top(clk,rst,ps2_clk,ps2_data,
	data,ready,overflow,hex0,hex1,hex2,hex3,hex4,hex5
);
	input clk,rst,ps2_clk,ps2_data;
	output [7:0] data;
	output ready;
	output overflow;
	output [6:0] hex0;
	output [6:0] hex1;
	output [6:0] hex2;
	output [6:0] hex3;
	output [6:0] hex4;
	output [6:0] hex5;
	
	reg [23:0] data_p;
	reg nextdata_n;
	wire [7:0] asc_out;
	reg [7:0] count;

	
	
	ps2_keyboard inst(	.clk(clk), 
						.clrn(rst), 
						.ps2_clk(ps2_clk), 
						.ps2_data(ps2_data), 
						.data(data),
						.nextdata_n(nextdata_n),
						.ready(ready), 
						.overflow(overflow));
	wire [7:0] data_1;
	assign data_1 = data_p[7:0];
	keycode_to_ascii k1(.keycode(data_1), .ascii(asc_out));
	
	bcd7seg b1(.b(data_p[3:0]), .h(hex0));
	bcd7seg b2(.b(data_p[7:4]), .h(hex1));
	
	bcd7seg b3(.b(asc_out[3:0]), .h(hex2));
	bcd7seg b4(.b(asc_out[7:4]), .h(hex3));
	//bcd7seg b3(.b(data_p[11:8]), .h(hex2));
	//bcd7seg b4(.b(data_p[15:12]), .h(hex3));
	bcd7seg b5(.b(count[3:0]), .h(hex4));
	bcd7seg b6(.b(count[7:4]), .h(hex5));
	always @(posedge clk) begin
		if(ready) begin
		data_p = {data_p[15:8],data_p[7:0],data[7:0]};
		if(data[7:0] == 8'hf0) begin
			end else begin
				if(data_p[15:8] == 8'hf0) begin
					count = count + 8'd1; 
				end else begin
					if(data_p[15:8] != data[7:0]) begin
						count = count + 8'd1; 
					end else begin
						if(data_p[23:16] == 8'hf0) begin
							count = count + 8'd1; 
						end
					end
				end
			end
		nextdata_n = 0;
		end
		else begin
		nextdata_n = 1;
		end
	end 

endmodule

