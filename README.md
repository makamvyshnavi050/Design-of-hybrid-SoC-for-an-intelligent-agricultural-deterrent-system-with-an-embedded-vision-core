# Design-of-hybrid-SoC-for-an-intelligent-agricultural-deterrent-system-with-an-embedded-vision-core

# Project Description
Crop destruction caused by birds and animals is a significant challenge in agriculture, leading to substantial financial losses for farmers in rural regions where continuous monitoring is not feasible. Traditional deterrent methods such as physical scarecrows, PIR motion sensors, and cloud-based smart camera systems consistently fail due to animal habituation, lack of species-specific detection, and high response latency. 

This project implements a fully edge-computed, low-latency system that combines real-time AI computer vision with robust FPGA hardware logic. The system captures live video feeds via IP cameras, processes them locally using an optimized object detection model, filters out false alerts (such as human workers), and uses a synchronized FPGA core to trigger localized, randomized audio-visual deterrents. Operating entirely offline over a local WiFi network, it achieves an end-to-end response latency of just 1 to 2 seconds, offering a field-ready solution for remote agricultural protection.

# Key Features
AI-Powered Object Detection: Real-time identification of threat animals (birds, dogs, horses, bears) using YOLOv3 with a 0.7 confidence threshold.
Intelligent Priority Logic: Suppresses false alerts automatically when a human farm worker is detected in the frame.
Low Latency & Deterministic Control: Utilizes a Xilinx Artix-7 FPGA running a synchronous Verilog HDL module at 100 MHz to process camera triggers simultaneously.
Anti-Habituation Deterrent System: Unpredictable deterrence mechanism utilizing randomly selected predator sounds paired with irregular LED strobe patterns.

# System Architecture & Tech Stack
Software & AI: Python, YOLOv3 Object Detection, OpenCV, IP Camera Streaming.
Hardware Logic Core: Nexys Board (Xilinx Artix-7 FPGA), Verilog HDL.
Edge Communication: ESP32-CAM (WiFi-to-GPIO conversion), ESP32-Dev Module.
Peripherals: DFPlayer Mini (Audio Player), High-Intensity LED Strobe Circuit, 8-ohm Speaker.
