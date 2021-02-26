module tb_counter8b_updown;
    wire [7:0] count;
    reg clk,reset,dir;

    counter8b_updown a(count,clk,reset,dir);

    integer k;

    initial
    begin
        clk = 1'b0;
        forever #5 clk = ~clk;
    end

    initial
    begin
        dir = 1'b1;
        #50 dir = 1'b0;
    end

    initial
    begin
        reset = 1'b1;
        #5 reset = 1'b0;
	#120 reset = 1'b1;
    end
endmodule
