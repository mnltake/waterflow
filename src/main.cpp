#include <Arduino.h>

#include <M5Unified.h> 
#include <EEPROM.h>
const int sensor_pin = 33;
const int relay_pin = 32 ;
volatile double waterFlow ;
volatile double totalwaterFlow ;
int pomp_dsec ;//0.1sec
void pulse()   //measure the quantity of square wave
{
  waterFlow += 1.0 / 450.0;
  totalwaterFlow += 1.0 / 450.0;
}

void setup() {
  waterFlow =45;
  totalwaterFlow =-5;
  EEPROM.begin(1);
  // EEPROM.write(0,50); 
  pomp_dsec =EEPROM.read(0);
  auto cfg = M5.config();
  M5.begin(cfg);   
  M5.Display.setTextSize(2); 
  M5.Display.setRotation(1);   

  pinMode(relay_pin ,OUTPUT);
  attachInterrupt(sensor_pin , pulse, RISING);  
}
void loop() {
  M5.update();
  digitalWrite(33,LOW);
  Serial.print("waterFlow:");
  Serial.print(waterFlow);
  Serial.println("   L");
  M5.Display.setCursor(10, 10);
  M5.Display.setTextColor(WHITE);
  M5.Display.setTextSize(2); 
  M5.Display.printf("P time %3.1f \n\n",float(pomp_dsec)*0.1);
  M5.Display.setTextColor(CYAN);
  M5.Display.setTextSize(3); 
  M5.Display.printf(" %4.1f L", totalwaterFlow);
  if (waterFlow > 50){
    waterFlow =0;
    digitalWrite(relay_pin,HIGH);
    M5.Display.clear();
    M5.Display.setCursor(10, 10);
    M5.Display.setTextSize(3); 
    M5.Display.setTextColor(RED);
    M5.Display.print("Pumping");
    delay(pomp_dsec*100);
    digitalWrite(relay_pin,LOW);
    
  }
  if (M5.BtnA.isPressed()){
    pomp_dsec += 5;
    EEPROM.write(0, pomp_dsec); 
    EEPROM.commit();
  }
  if (M5.BtnB.isPressed()){
    pomp_dsec -= 5;
    EEPROM.write(0, pomp_dsec); 
    EEPROM.commit();
  }
  delay(500);
  M5.Display.clear();
}

