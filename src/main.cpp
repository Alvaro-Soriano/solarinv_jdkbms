#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <modbus.h>
#include "secrets.h"


const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
String url = "http://192.168.1.58:5000/test";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  innitModbus();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    HTTPClient http;

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    String json = readJKBMS();

    int httpCode = http.POST(json);

    if (httpCode > 0) {
      String response = http.getString();
    }

    http.end();
  }

  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
}
