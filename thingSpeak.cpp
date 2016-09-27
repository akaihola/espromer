#include <Arduino.h>
#include <ESP8266WiFi.h>

#define THINGSPEAK_HOST "api.thingspeak.com"
#define THINGSPEAK_PORT 80

void thingSpeakSend(const char *wifiSsid, const char *wifiPassword, String apiKey,
                    float temperature, uint16_t voltage) {
  byte tryNum = 0;

  Serial.println("WiFi force sleep wake");
  WiFi.forceSleepWake();
  delay(100);

  Serial.println("Setting WiFi mode to WIFI_STA");
  WiFi.mode(WIFI_STA);
  
  Serial.print("WiFi mode is ");
  Serial.println(WiFi.getMode());

  Serial.print("Connecting to ");
  Serial.println(wifiSsid);
  WiFi.begin(wifiSsid, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    if (++tryNum >= 50) {
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
  Serial.println(wifiSsid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  WiFiClient client;
  const int httpPort = 80;
  Serial.println("Connecting to ThingSpeak...");
  if (client.connect(THINGSPEAK_HOST, THINGSPEAK_PORT)) {
    char temperatureString[6];
    dtostrf(temperature, 2, 2, temperatureString);
    String path = "/update?key=" + apiKey + 
                  "&field1=" + temperatureString + 
                  "&field2=" + voltage +
                  "&field3=" + millis();  
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
