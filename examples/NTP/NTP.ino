#include <MycilaNTP.h>

#include <HardwareSerial.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  Mycila::NTP.setTimezone("Europe/Paris");
  Mycila::NTP.setNTPServer("pool.ntp.org");

  Serial.println(Mycila::NTP.getTimezoneInfo());

  WiFi.begin("IoT");
}

void loop() {
  Serial.println(Mycila::NTP.isSynced());
  delay(1000);
}
