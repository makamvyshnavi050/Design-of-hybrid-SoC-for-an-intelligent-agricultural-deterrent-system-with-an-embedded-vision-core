//cam1 Arduino code 
#include <WebServer.h> 
#include <WiFi.h> 
#include <esp32cam.h>
const char* WIFI_SSID = "OPPO A78 5G"; 
const char* WIFI_PASS = "13192305";

// --- UPDATED STATIC IP SETTINGS FOR CAM 1 ---
IPAddress local_IP(172, 18, 170, 100); // Fixed IP for Board 1
IPAddress gateway(172, 18, 170, 191); // Your Phone's Gateway from ipconfig 
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);
WebServer server(80);
const int FPGA_SIGNAL_PIN = 14;
static auto loRes = esp32cam::Resolution::find(320, 240);

void handleFpgaSignal() 
{
  if (server.hasArg("state")) 
  { 
    String state = server.arg("state"); 
    if (state == "1") 
    {
      digitalWrite(FPGA_SIGNAL_PIN, HIGH); 
      server.send(200, "text/plain", "ALERT_SENT");
      Serial.println(">>> FPGA SIGNAL: HIGH (1) - ANIMAL");
    } 
    else 
    {
      digitalWrite(FPGA_SIGNAL_PIN, LOW); 
      server.send(200, "text/plain", "SAFE_SENT"); 
      Serial.println(">>> FPGA SIGNAL: LOW (0) - SAFE");
    }
  }
}

void serveJpg() 
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) { server.send(503, "", ""); return; } 
  server.setContentLength(frame->size()); 
  server.send(200, "image/jpeg");
  WiFiClient client = server.client(); 
  frame->writeTo(client);
}

void handleJpgLo() 
{
  esp32cam::Camera.changeResolution(loRes); 
  serveJpg();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("\n--- CAM 1 Booting ---");
  pinMode(FPGA_SIGNAL_PIN, OUTPUT);
  digitalWrite(FPGA_SIGNAL_PIN, LOW);
  if (!psramInit()) 
  { 
    Serial.println("PSRAM Init Failed!");
  }
// Apply static IP before connecting
  if (!WiFi.config(local_IP, gateway, subnet, dns)) 
  { 
    Serial.println("Static IP Failed to configure!");
  }
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nCONNECTED!"); 
  Serial.print("CAM 1 Fixed IP: "); 
  Serial.println(WiFi.localIP());
  using namespace esp32cam; 
  Config cfg; 
  cfg.setPins(pins::AiThinker); 
  cfg.setResolution(loRes); 
  cfg.setBufferCount(2); 
  cfg.setJpeg(15);
  if (!Camera.begin(cfg)) 
  { 
    Serial.println("Camera Init Failed!");
  } 
  else 
  {
    Serial.println("Camera Init OK");
  }
  server.on("/cam-lo.jpg", handleJpgLo); 
  server.on("/signal", handleFpgaSignal); 
  server.begin();
  Serial.println("CAM 1 Ready at 172.18.170.100");
}

void loop() 
{ 
  server.handleClient();
}
