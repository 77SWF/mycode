module shift_counter(count,clk,reset);
    output reg [7:0] count;
    input clk,reset;

    reg [1:0] cnt;
    reg dir;

    initial
    begin
        count <= 8'b1;
        cnt <= 2'b0;
        dir <= 1'b1;
    end

    always @(posedge clk)
    begin
        if(reset)
        begin
            cnt <= 2'b0;
            count <= 8'b1;
	    dir <= 1; 
        end 
        else if(cnt<2'b11)
            cnt <= cnt + 1;
        else if(dir == 0)
            begin
                if(count>8'b1)
                    count <= count >> 1;
                else
                    cnt <= 0;
            end
        else if(dir==1 && count<8'b10000000)
            count <= count << 1;
        else
        begin
            count <= count >> 1;
            dir = 1'b0;
        end
    end
endmodule
