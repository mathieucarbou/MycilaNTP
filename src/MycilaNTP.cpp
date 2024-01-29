// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou and others
 */
#include <MycilaNTP.h>
#include <MycilaNTPSpecs.h>

#include <Arduino.h>

void Mycila::NTPClass::setNTPServer(const String& server, const uint8_t retryInterval) {
  _server = server;
  _ticker.detach();
  if (!server.isEmpty()) {
    configTime(0, 0, _server.c_str());
    if (!_synced) {
      _ticker.attach(
        retryInterval, +[](NTPClass* instance) {
          // Serial.println("NTP Tick");
          if (!instance->_synced) {
            struct tm timeInfo;
            if (getLocalTime(&timeInfo, 5)) {
              instance->_synced = true;
              instance->_ticker.detach();
            }
          } else {
            instance->_ticker.detach();
          }
        },
        this);
    }
  }
}

bool Mycila::NTPClass::setTimezone(const String& timezone) {
  if (!timezone.isEmpty()) {
    _spec = emptyString;
    char* found = strstr(MYCILA_NTP_SPEC, (timezone + "=").c_str());
    if (found != nullptr) {
      const char* start = found + timezone.length() + 1;
      found = strstr(start, "\n");
      const int len = found == nullptr ? strlen(start) : static_cast<unsigned int>(found - start);
      _spec = String(start, len);
      setenv("TZ", _spec.c_str(), 1);
      tzset();
      return true;
    }
  }
  return false;
}

bool Mycila::NTPClass::update(const timeval* tv) {
  if (tv)
    settimeofday(tv, nullptr);
  struct tm timeInfo;
  if (getLocalTime(&timeInfo, 5)) {
    _synced = true;
    _ticker.detach();
    return true;
  }
  return false;
}

#ifdef MYCILA_NTP_JSON_SUPPORT
void Mycila::NTPClass::timezonesToJson(const JsonObject& doc) const {
  char* start = const_cast<char*>(MYCILA_NTP_SPEC);
  char* token = strstr(start, "=");
  while (token != nullptr) {
    const String timezone = String(start, static_cast<size_t>(token - start));
    start = token + 1;
    token = strstr(start, "\n");
    const String spec = String(start, static_cast<size_t>(token - start));
    start = token + 1;
    token = strstr(start, "=");
    doc[timezone] = spec;
  }
}
#endif

namespace Mycila {
  NTPClass NTP;
} // namespace Mycila
