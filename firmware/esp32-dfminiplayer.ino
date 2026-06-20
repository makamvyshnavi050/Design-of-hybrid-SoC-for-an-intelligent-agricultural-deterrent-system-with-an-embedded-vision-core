// df mini player and led output program 
#include "HardwareSerial.h"
#include "DFRobotDFPlayerMini.h"

HardwareSerial mySerial(2); 
DFRobotDFPlayerMini player;

const int TRIGGER_PIN = 4; // D4 on your pinout 
int lastSound = 0;
int myVolume = 18;          // Slightly higher than 15 to help it "wake up"

void setup() 
{ 
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // TX2=17, RX2=16
  pinMode(TRIGGER_PIN, INPUT_PULLDOWN);
  Serial.println("System Starting: Root Index Mode");
  
  // Safety: No ACK needed since TX is disconnected 
  if (player.begin(mySerial, true, false)) 
  { 
    Serial.println("DFPlayer Online."); 
    player.volume(myVolume); 
    randomSeed(analogRead(34));
  } 
  else 
  {
    Serial.println("Waiting for Player...");
    player.volume(myVolume);
  }
}

void loop() 
{
  // Trigger: Touch D4 to 3.3V
  if (digitalRead(TRIGGER_PIN) == HIGH) 
  { 
    int currentSound = random(1, 5);
    while (currentSound == lastSound) 
    { 
      currentSound = random(1, 5); 
    }
    
    Serial.print("Alert! Playing Physical File #"); 
    Serial.println(currentSound);
    
    // This command ignores folders and plays the physical 1st, 2nd, etc. file 
    player.play(currentSound);
    lastSound = currentSound;
    
    // Stay in this loop until the signal is removed 
    while (digitalRead(TRIGGER_PIN) == HIGH) 
    { 
      delay(10); 
    }
    
    player.stop(); 
    Serial.println("Stopped.");
  }
}
