module mux4x1(dout,sel,din);
    output dout;
    input [1:0] sel;
    input [3:0] din;

    wire out1,out2;
    mux2x1 mux1(.dout(out1),.sel(sel[0]),.din(din[1:0])),
           mux2(.dout(out2),.sel(sel[0]),.din(din[3:2])),
           mux3(.dout(dout),.sel(sel[1]),.din({out1,out2}));
endmodule
