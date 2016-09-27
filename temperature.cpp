#include "device.h"
#include "temperature.h"

char temperatureString[6];

float getTemperature() {
  float temp;
  
  #ifdef ONE_WIRE_BUS
    OneWire oneWire(ONE_WIRE_BUS);
    DallasTemperature DS18B20(&oneWire);
    Serial.println("Starting DS18B20.");
    DS18B20.begin();
    Serial.println("DS18B20 started.");
  #endif
  
  #ifdef DHTPIN
    DHT dht(DHTPIN, DHTTYPE);
  #endif

  do {
    Serial.print("Reading temperature...");
    #ifdef ONE_WIRE_BUS
      DS18B20.requestTemperatures(); 
      temp = DS18B20.getTempCByIndex(0);
    #elifdef DHTPIN
      temp = dht.readTemperature();
    #else
      Serial.println("No temperature sensor defined.");
      return 0.0;
    #endif
    
    dtostrf(temp, 2, 2, temperatureString);
    Serial.print(" ");
    Serial.print(temperatureString);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));  // these are DS18B20 "not ready" values?
  Serial.println();
  return temp;

}


