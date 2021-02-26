
module moore(flag, din, clk, rst);

    output reg flag;
    input din, clk, rst;

    parameter IDLE = 9'b0_0000_0001;
    parameter A = 9'b0_0000_0010; 
    parameter B = 9'b0_0000_0100;
    parameter C = 9'b0_0000_1000;
    parameter D = 9'b0_0001_0000;
    parameter E = 9'b0_0010_0000;
    parameter F = 9'b0_0100_0000; 
    parameter G = 9'b0_1000_0000; 
    parameter H = 9'b1_0000_0000; 

    reg [8:0] state;

    always @ ( posedge rst ) begin
        flag <= 1'b0;
        state <= IDLE;
    end

    always @ ( posedge clk ) begin
        flag <= (state == H) ? 1'b1 : 1'b0;
        case (state)
            IDLE: state <= (din) ? IDLE : A;
            A: state <= (din) ? B : A;
            B: state <= (din) ? IDLE : C;
            C: state <= (din) ? D : A;
            D: state <= (din) ? IDLE : E;
            E: state <= (din) ? F : A;
            F: state <= (din) ? IDLE : G;
            G: state <= (din) ? H : A;
            H: state <= (din) ? IDLE : G;
            default: begin state <= IDLE; flag <= 1'b0; end
        endcase
    end

endmodule
