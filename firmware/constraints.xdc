// verilog constraint file

# Clock Signal
set_property -dict { PACKAGE_PIN E3  IOSTANDARD LVCMOS33 } [get_ports { clk }];
create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5} [get_ports { clk }];

# Reset Button
set_property -dict { PACKAGE_PIN N17 IOSTANDARD LVCMOS33 } [get_ports { rst_btn }];

# Inputs from ESP32-CAMs
set_property -dict { PACKAGE_PIN D18 IOSTANDARD LVCMOS33 } [get_ports { esp32_cam_1 }];
set_property -dict { PACKAGE_PIN E18 IOSTANDARD LVCMOS33 } [get_ports { esp32_cam_2 }];

# Output to ESP32 Dev Module
set_property -dict { PACKAGE_PIN C17 IOSTANDARD LVCMOS33 } [get_ports { trigger_to_esp32 }];

# Board Configuration Settings
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]
