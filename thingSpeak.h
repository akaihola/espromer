#include <Arduino.h>

void thingSpeakSend(const char *wifiSsid, const char *wifiPassword, String apiKey,
                    float temperature, uint16_t voltage);
