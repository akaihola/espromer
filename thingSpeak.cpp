#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "device.h"
#include "thingSpeak.h"

#define THINGSPEAK_HOST "api.thingspeak.com"
#define THINGSPEAK_PORT 80

void thingSpeakSend(struct Measurements measurements) {
  byte tryNum = 0;

  Serial.println("WiFi force sleep wake");
  WiFi.forceSleepWake();
  delay(100);

  Serial.println("Setting WiFi mode to WIFI_STA");
  WiFi.mode(WIFI_STA);
  
  Serial.print("WiFi mode is ");
  Serial.println(WiFi.getMode());

  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    if (++tryNum >= 100) {
      Serial.println("\nNo WiFi.");
      return;
    } else {
      delay(100);
      Serial.print(tryNum);
      Serial.print(" ");
    }
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  WiFiClient client;
  const int httpPort = 80;
  Serial.println("Connecting to ThingSpeak...");
  if (client.connect(THINGSPEAK_HOST, THINGSPEAK_PORT)) {
    char temperatureString[6];
    dtostrf(measurements.temperature, 2, 2, temperatureString);
#ifdef DHTPIN
    char humidityString[6];
    dtostrf(measurements.humidity, 2, 2, humidityString);
#endif
    String path = "/update?key=" + String(THINGSPEAK_API_KEY)
                  + "&field1=" + temperatureString
                  + "&field2=" + measurements.voltage
                  + "&field3=" + millis()
#ifdef DHTPIN
                  + "&field4=" + humidityString
#endif
                  ;  
    Serial.println("Sending measurement...");
    String request = String("GET ") + path + " HTTP/1.1\r\n" +
                     "Host: " + THINGSPEAK_HOST + "\r\n" + 
                     "Connection: keep-alive\r\n\r\n";
    Serial.println(request);
    client.print(request);
  } else {
    Serial.println("connection failed");
  }  
}
