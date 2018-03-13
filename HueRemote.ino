#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <PersWiFiManager.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial

void setup() {

  ESP8266WebServer server(80);
  DNSServer dnsServer;
  PersWiFiManager persWM(server, dnsServer);
  server.on("/io", [&]() {
    server.send(200, "text/html", "<a href='wifi.htm'>wifi</a>");
  });
  persWM.begin();

  USE_SERIAL.begin(115200);

} //setup

void getHueIp() {
  StaticJsonBuffer<200> jsonBuffer;
  
  HTTPClient http;

  http.begin("https://www.meethue.com/api/nupnp", "*");
  int httpCode = http.GET();
  if (httpCode > 0) {
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      JsonArray& root = jsonBuffer.parseArray(http.getString());
      String ip = root[0]["internalipaddress"];
      Serial.println(ip);
//      String payload = http.getString();
//      //[{"id":"001788fffe2f6aae","internalipaddress":"192.168.86.118"}]
//      USE_SERIAL.println(payload);
    }
  } else {
    USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void loop() {
  // wait for WiFi connection

  delay(1000);
  getHueIp();

  delay(5000);
} //loop

