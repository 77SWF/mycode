module counter8b_updown(count,clk,reset,dir);
    output reg [7:0] count;
    input clk,reset,dir;

    always @(posedge clk ,posedge reset)
    begin
        if(reset)
            count <= 8'b0;
        else
            case(dir)
                1'b1: count <= count + 8'b1;
                1'b0: count <= count - 8'b1;
                default: count <= 8'b0;
            endcase
    end
endmodule
