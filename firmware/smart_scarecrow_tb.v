// verilog testbench
// File Name: tb_smart_scarecrow.v
`timescale 1ns / 1ps

module tb_smart_scarecrow;
    reg clk, rst_btn, esp32_cam_1, esp32_cam_2;
    wire trigger_to_esp32;

    // Instantiate Device Under Test (DUT)
    smart_scarecrow dut (
        .clk(clk),
        .rst_btn(rst_btn),
        .esp32_cam_1(esp32_cam_1),
        .esp32_cam_2(esp32_cam_2),
        .trigger_to_esp32(trigger_to_esp32)
    );

    // Clock generation: 10ns period
    initial clk = 0;
    always #5 clk = ~clk;

    // Stimulus block
    initial begin
        // Initialize ALL inputs at 0ns to prevent Red Bars
        rst_btn = 1;
        esp32_cam_1 = 0;
        esp32_cam_2 = 0;
        
        #20 rst_btn = 0; // Release Reset
        
        // Test the cases
        #20 {esp32_cam_1, esp32_cam_2} = 2'b01;
        #20 {esp32_cam_1, esp32_cam_2} = 2'b10;
        #20 {esp32_cam_1, esp32_cam_2} = 2'b11;
        #20 {esp32_cam_1, esp32_cam_2} = 2'b00;
        
        #40 $finish;
    end
endmodule
