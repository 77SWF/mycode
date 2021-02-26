module tb_mux2x1;
    reg sel1;
    reg [1:0] din1;
    wire dout1;

    mux2x1 mux(.dout(dout1),.sel(sel1),.din(din1));

    integer k;

    initial
    begin
        din1 = 2'b10;
        sel1 = 1'b0;
        for(k=0;k<2;k=k+1)
        begin
            #5;
            sel1 = ~sel1;
        end
    end

    initial
        $monitor("At time %4t, din=%b, sel=%b, dout=%b",
                    $time, din1, sel1, dout1);
endmodule
