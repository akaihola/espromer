#include "device.h"

#ifdef ONE_WIRE_BUS
  #include <OneWire.h>
  #include <DallasTemperature.h>
#endif

#ifdef DHTPIN
  #include <DHT.h>
#endif

float getTemperature();
