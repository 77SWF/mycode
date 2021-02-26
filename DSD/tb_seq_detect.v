module tb_seq_detect;
    wire flag;
    reg din,clk,rst_n;

    reg [31:0] data = 32'b1100_0110_0100_0110_1010_0100_1010_0010;
    seq_detect a(flag, din, clk, rst_n);
    initial
    begin
        clk = 1'b0;
        forever #5 clk = ~clk;
    end

    integer k;
    
    initial
    begin
        rst_n = 1'b0;
        #20 rst_n = 1'b1;
    end

    initial
        for(k=0;k<32;k=k+1)
        begin
            din = data[31];
            data = data << 1;
            #10;
        end
    
endmodule
