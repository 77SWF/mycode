module tb_mux4x1;
    wire dout1;
    reg [1:0] sel1;
    reg [3:0] din1;

    integer k;

    mux4x1 mux(dout1,sel1,din1);

    initial
    begin
        din1 = 4'b0;
        sel1 =2'b0;
        for(k=0;k<4;k=k+1)
        begin
            #5;
            sel1 = k;
            din1 = {$random} %16;
        end
    end

    initial 
        $monitor("At time %4t, din=%b, sel=%b, dout=%b",
                    $time, din1, sel1, dout1);
endmodule
