module top(clk, clrn, ps2_clk, ps2_data,flag, flag2,hex0);

	input clk,clrn,ps2_clk,ps2_data;
	wire [7:0] data_code;
	wire ready;
	wire overflow;
	reg nextdata_n2;
	
	reg [7:0] data_p;
	reg [7:0] count = 0; // an le duoshao ci
	
	output reg flag = 1;   // judge keyboard-button is valid or not
	output reg flag2 = 1;
	output reg [6:0] hex0;

	reg [6:0] count_clk = 0;
	reg clk_b = 0;
	always @(posedge clk)
	begin
		if(count_clk == 100)
		begin
			count_clk <= 0;
			clk_b <= ~clk_b;
		end
		else
			count_clk <= count_clk + 1;
	end


	ps2_keyboard S(.clk(clk), .clrn(clrn), .ps2_clk(ps2_clk), .ps2_data(ps2_data), .data(data_code), .ready(ready), .nextdata_n(nextdata_n2), .overflow(overflow));

	bcd7seg b1(.b(data_p[3:0]), .h(hex0));

	always @(posedge clk_b)
	begin
		if (ready)
		begin
			if(data_code != 8'hf0 && flag2)
			begin
				data_p <= data_code;
				flag2 <= 1;
				flag <= 1;
			end
			else if (data_code == 8'hf0)
			begin
				data_p <= data_code;
				count <= count + 1;
				flag <= 0;
				flag2 <= 0;
			end
			else if(!flag2)
			begin
				data_p <= data_code;
				flag2 <= 1;
				flag <= 0;
			end

			nextdata_n2 <= 0;
		end
		else
		begin
			nextdata_n2 <= 1;
		end
	end

endmodule
