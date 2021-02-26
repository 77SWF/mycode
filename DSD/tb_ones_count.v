module tb_ones_count;
    wire [3:0] count;
    reg [7:0] dat_in;

    ones_count a(count,dat_in);

    integer k;

    initial
    begin
        for(k=0;k<256;k=k+1)
        begin
            dat_in = k;
            #5;
        end
    end

    initial 
        $monitor("At time %4t, dat_in=%b, count=%b",
                    $time, dat_in, count);
endmodule

