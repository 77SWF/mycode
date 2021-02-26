// (5)
module testbench_comb;
    wire Y1,Y2,Y3,Y4;
    reg A,B,C,D;

    comb_str a(Y1, A, B, C, D);
    comb_dataflow b(Y2, A, B, C, D);
    comb_behaviour c(Y3, A, B, C, D);
    comb_prim d(Y4, A, B, C, D);

    integer k;

    initial
    begin
        {A,B,C,D} = 4'b0;
        for(k=0;k<15;k=k+1)
        begin
            #5;
            {A,B,C,D} = k+1;
        end
    end

    initial 
        $monitor("At time %4t, A=%b, B=%b, C=%b, D=%b, Y1=%b, Y2=%b, Y3=%b, Y4=%b",
                    $time, A, B, C, D, Y1, Y2, Y3, Y4);
endmodule
