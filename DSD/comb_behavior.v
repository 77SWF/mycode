// (3)
module comb_behaviour(Y,A,B,C,D);
    output reg Y;
    input A,B,C,D;

    always @*
        Y = ( ~( A|D ) ) & ( B & C & (~D) );
endmodule
