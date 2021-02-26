module tb_dec_counter;
    wire [3:0] count;
    reg clk,reset;
    
    dec_counter a(count,clk,reset);

    initial
    begin
        clk = 1'b0;
        forever #1 clk = ~clk;
    end

    initial 
    begin
        reset = 1'b0;
        #25 reset = 1'b1;
    end

    initial 
        $monitor("At time %4t, reset=%b, count=%d",
                    $time, reset, count);
endmodule
