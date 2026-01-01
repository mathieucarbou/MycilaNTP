// SPDX-License-Identifier: MIT
/*
 * Copyright (C) Mathieu Carbou
 */
#include <MycilaNTP.h>

#include <HardwareSerial.h>
#include <WiFi.h>

#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  uint32_t start = ESP.getFreeHeap();

  JsonDocument doc;

  Mycila::NTP.timezonesToJsonObject(doc.to<JsonObject>());
  serializeJsonPretty(doc, Serial);
  Serial.println();

  Serial.print("Heap used: ");
  Serial.print(start - ESP.getFreeHeap());
  Serial.println(" bytes");

  Mycila::NTP.timezonesToJsonArray(doc.to<JsonArray>());
  serializeJsonPretty(doc, Serial);
  Serial.println();

  Serial.print("Heap used: ");
  Serial.print(start - ESP.getFreeHeap());
  Serial.println(" bytes");
}

void loop() {
  delay(1000);
}
