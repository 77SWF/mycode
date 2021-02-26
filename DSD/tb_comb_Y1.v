module tb_comb_Y1;
    wire Y;
    reg A,B,C;

    comb_Y1 a(Y,A,B,C);

    integer k;

    initial
    begin
        for(k=0;k<8;k=k+1)
        begin
            {A,B,C} = k;
            #5;
        end
    end

    initial
        $monitor("At time %4t, A=%b, B=%b, C=%b, Y=%b",
                    $time, A, B, C, Y);
endmodule
