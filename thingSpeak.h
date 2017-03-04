#include <Arduino.h>

struct Measurements {
  float temperature;
#ifdef DHTPIN
  float humidity;
#endif
  uint16_t voltage;
};

void thingSpeakSend(struct Measurements measurements);
