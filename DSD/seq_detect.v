module seq_detect(flag, din, clk, rst_n);
    output reg flag;
    input din,clk,rst_n;

    parameter IDLE=9'b0__0000_0001,
              A=9'b0_0000_0010,
              B=9'b0_0000_0100,
              C=9'b0_0000_1000,
              D=9'b0_0001_0000,
              E=9'b0_0010_0000,
              F=9'b0_0100_0000,
              G=9'b0_1000_0000,
              H=9'b1_0000_0000;

    reg [8:0] p_state,n_state;

    always @(negedge clk)
    begin
        if(!rst_n)
        begin
            //flag <= 1'b0;
            p_state <= IDLE;
        end
        else
        begin
            p_state <= n_state;
        end
    end

    always @(*)
    begin
        flag = ((p_state==D)||(p_state==H))? 1'b1:1'b0;
        case (p_state)
            IDLE: n_state = (din)? A : E;
            A: n_state = (din)? B : E;
            B: n_state = (din)? B : C;
            C: n_state = (din)? D : E;
            D: n_state = (din)? B : E;
            E: n_state = (din)? F : E;
            F: n_state = (din)? G : IDLE;
            G: n_state = (din)? A : H;
            H: n_state = (din)? A : E;
            default: n_state = IDLE;
        endcase
    end

endmodule