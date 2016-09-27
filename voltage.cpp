#include <Arduino.h>

uint16_t getVoltage() {
  uint16_t voltage;
  Serial.print("Reading power supply voltage... ");
  voltage = ESP.getVcc();
  Serial.println(voltage);
  return voltage;
}


