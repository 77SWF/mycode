module ALU(c_out,sum,oper,a,b,c_in);
    output reg c_out;
    output reg [7:0] sum;
    input [2:0] oper;
    input [7:0] a;
    input  [7:0] b;
    input c_in;

    always @*
    begin
        case(oper)
            3'd0: {c_out,sum} = a + b + c_in;
            3'd1: {c_out,sum} = a + ~b +c_in; 
            3'd2: {c_out,sum} = b + ~a + ~c_in;
            3'd3: {c_out,sum} = { 1'b0, a|b };
            3'd4: {c_out,sum} = { 1'b0, a&b };
            3'd5: {c_out,sum} = { 1'b0, (~a) & b };
            3'd6: {c_out,sum} = { 1'b0, a ^ b };
            3'd7: {c_out,sum} = { 1'b0, a ~^ b};
            default: {c_out,sum} = 9'bx;
	endcase
    end
endmodule
