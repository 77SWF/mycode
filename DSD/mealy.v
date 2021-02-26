// File: mealy.v

module mealy(flag, din, clk, rst);

    output reg flag;
    input din, clk, rst;

    parameter IDLE = 8'b0000_0001; 
    parameter A = 8'b0000_0010; 
    parameter B = 8'b0000_0100;
    parameter C = 8'b0000_1000;
    parameter D = 8'b0001_0000;  
    parameter E = 8'b0010_0000;  
    parameter F = 8'b0100_0000; 
    parameter G = 8'b1000_0000;  

    reg [8:0] state;

    always @ ( posedge rst ) begin
        flag <= 1'b0;
        state <= IDLE;
    end

    always @ ( posedge clk ) begin
        case (state)
            IDLE: if (din) begin state <= IDLE; flag <= 1'b0; end
                else begin state <= A; flag <= 1'b0; end
            A: if (din) begin state <= B; flag <= 1'b0; end
                else begin state <= A; flag <= 1'b0; end
            B: if (din) begin state <= IDLE; flag <= 1'b0; end
                else begin state <= C; flag <= 1'b0; end
            C: if (din) begin state <= D; flag <= 1'b0; end
                else begin state <= A; flag <= 1'b0; end
            D: if (din) begin state <= IDLE; flag <= 1'b0; end
                else begin state <= E; flag <= 1'b0; end
            E: if (din) begin state <= F; flag <= 1'b0; end
                else begin state <= A; flag <= 1'b0; end
            F: if (din) begin state <= IDLE; flag <= 1'b0; end
                else begin state <= G; flag <= 1'b0; end
            G: if (din) begin state <= F; flag <= 1'b1; end
                else begin state <= A; flag <= 1'b0; end
            default: begin state <= IDLE; flag <= 1'b0; end
        endcase
    end

endmodule

