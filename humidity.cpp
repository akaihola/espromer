#include <Arduino.h>
#include "device.h"
#include "humidity.h"

char humidityString[6];

float getHumidity() {
  float hum;
  
  #ifdef DHTPIN
    DHT dht(DHTPIN, DHTTYPE);
  #endif

  Serial.print("Reading humidity...");
  #if defined DHTPIN
    hum = dht.readHumidity();
  #else
    Serial.println("No humidity sensor defined.");
    return 0.0;
  #endif
  
  dtostrf(hum, 2, 2, humidityString);
  Serial.print(" ");
  Serial.print(humidityString);
  delay(100);
  Serial.println();
  return hum;
}


