`timescale 1ns / 1ns
module tb_comb_str;
    wire y;
    reg sel,A,B,C,D;

    comb_str a(y,sel,A,B,C,D);

    integer k;

    initial 
    begin
        for(k=0;k<32;k=k+1)
        begin
            {A,B,C,D} = k;
            #5;
        end
    end

    initial
    begin
        sel = 1'b0;
        forever #80 sel = ~sel;
    end

    initial
        $monitor("At time %4t, sel=%b, A=%b, B=%b, C=%b, D=%b, y=%b",
                    $time, sel, A, B, C, D, y);
endmodule
