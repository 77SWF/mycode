module wavegen;
reg [3:0] a = 4'b0;
reg [3:0] b = 4'b0;
reg [4:0] c = 5'b0;
reg [5:0] d = 6'b0;
 reg [5:0] e = 6'b0;
 initial begin
 a = 4'b10x0;
 b = 4'b0001;
 c = 5'bZ;
 #5 c = !a == !b;
 #5 c = c & (~b);
 #5 c = c + b;
 #5 c = {c[2:0], d[3:2]};
 #5 c = c + a;
end
initial fork
 begin
 #0 d = 6'bz10;
 @(c) d = 6'b1;
 @(c) d = 6'b10;
 @(c) d = 6'b11;
 @(c) d = 6'b101;
 @(c) d = 6'b111;
 end
 begin
 #0 wait(d) e = 6'bx;
 #5 wait(d) e = 6'b11_0000;
 #5 wait(d) e = 6'b01_1000;
 #5 wait(d) e = 6'b00_1100;
 #5 wait(d) e = 6'b00_0011;
 #5 wait(d) e = 6'b10_0001;
 end
 join
endmodule