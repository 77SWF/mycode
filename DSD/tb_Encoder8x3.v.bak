module tb_Encoder8x3;

    reg [7:0] data1;
    wire [2:0] code1;

    integer k;

    Encoder8x3 encoder(.code(code1),.data(data1));

    initial
    begin
        data1 = 8'b00000001;
        for(k=0;k<8;k=k+1)
        begin
            #5;
            data1 = data1 << 1;
        end
    end

    initial
    begin
        $monitor("At time %4t, data=%b, code=%d",
                $time,encoder.data,encoder.code);
    end

endmodule