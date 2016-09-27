// ESP8266 DS18B20 ArduinoIDE Thingspeak IoT Example code
// http://vaasa.hacklab.fi
//
// https://github.com/milesburton/Arduino-Temperature-Control-Library
// https://gist.github.com/jeje/57091acf138a92c4176a

#include "device.h"

#ifdef ONE_WIRE_BUS
#include <OneWire.h>
#include <DallasTemperature.h>
#endif

#ifdef DHTPIN
#include <DHT.h>
#endif

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "voltage.h"
#include "temperature.h"
#include "deepsleep.h"
#include "thingSpeak.h"

#define SECONDS_DS(seconds)  ((seconds)*1000000UL)

struct Measurements {
  float temperature;
  uint16_t voltage;
} measurements;

void setup(void) {
  Serial.begin(115200);
  Serial.println("");
  switch (getState(&measurements, sizeof(measurements))) {
    case STATE_COLDSTART:
      reboot(10, STATE_NO_WIFI);
      break;
    case STATE_NO_WIFI:
      measurements.temperature = getTemperature();
      measurements.voltage = getVoltage();
      reboot(10, STATE_WIFI, &measurements, sizeof(measurements));
      break;
    case STATE_WIFI:
      thingSpeakSend(measurements.temperature, measurements.voltage);
      reboot(SECONDS_DS(15*60), STATE_NO_WIFI);
      break;
  }
}

void loop() {
}
