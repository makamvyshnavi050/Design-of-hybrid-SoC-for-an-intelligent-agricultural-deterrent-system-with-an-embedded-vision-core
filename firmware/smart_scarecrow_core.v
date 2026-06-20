// Verilog design module
// File Name: smart_scarecrow.v
module smart_scarecrow (
    input wire clk,
    input wire rst_btn,
    input wire esp32_cam_1,
    input wire esp32_cam_2,
    output reg trigger_to_esp32
);

// Synchronous block: Only changes on the rising edge of the clock
always @(posedge clk) begin
    if (rst_btn) begin
        trigger_to_esp32 <= 1'b0;
    end else begin
        // We use an IF statement for cleaner priority logic
        if (esp32_cam_1 == 1'b1 || esp32_cam_2 == 1'b1) begin
            trigger_to_esp32 <= 1'b1;
        end else begin
            trigger_to_esp32 <= 1'b0;
        end
    end
end
endmodule
