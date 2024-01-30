#include <Arduino.h>
#include <HardwareSerial.h>
#include <MycilaNTP.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  Mycila::NTP.begin("Europe/Paris", "pool.ntp.org");

  Serial.println(Mycila::NTP.getTimezoneInfo());

  WiFi.begin("IoT");
}

void loop() {
  Serial.println(Mycila::NTP.isSynced());
  struct tm timeInfo;
  if (getLocalTime(&timeInfo))
    Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");
  delay(1000);
}
