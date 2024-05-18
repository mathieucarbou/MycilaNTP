// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou and others
 */
#include <MycilaNTP.h>
#include <MycilaNTPSpecs.h>

#include <Arduino.h>
#include <sys/time.h>

bool Mycila::NTPClass::setTimeZone(const String& timezone) {
  if (timezone.isEmpty())
    return false;

  char* found = strstr(MYCILA_NTP_SPEC, (timezone + "=").c_str());
  if (found == nullptr)
    return false;

  const char* start = found + timezone.length() + 1;
  _spec = String(start, static_cast<unsigned int>(strstr(start, "\n") - start));

  setenv("TZ", _spec.c_str(), 1);
  tzset();

  return true;
}

bool Mycila::NTPClass::sync(const String& server, const uint8_t retryInterval) {
  if (server.isEmpty())
    return false;

  _server = server;
  _ticker.detach();

  configTime(0, 0, _server.c_str());
  if (!_spec.isEmpty()) {
    setenv("TZ", _spec.c_str(), 1);
    tzset();
  }

  struct tm timeInfo;
  getLocalTime(&timeInfo, 5);

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

  return true;
}

bool Mycila::NTPClass::sync(const timeval& tv) {
  _ticker.detach();

  settimeofday(&tv, nullptr);
  if (!_spec.isEmpty()) {
    setenv("TZ", _spec.c_str(), 1);
    tzset();
  }

  struct tm timeInfo;
  if (getLocalTime(&timeInfo, 5)) {
    _synced = true;
    return true;
  }

  return false;
}

#ifdef MYCILA_JSON_SUPPORT
void Mycila::NTPClass::timezonesToJsonObject(const JsonObject& doc) const {
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

void Mycila::NTPClass::timezonesToJsonArray(const JsonArray& doc) const {
  char* start = const_cast<char*>(MYCILA_NTP_SPEC);
  char* token = strstr(start, "=");
  while (token != nullptr) {
    doc.add(String(start, static_cast<size_t>(token - start)));
    token = strstr(token + 1, "\n");
    start = token + 1;
    token = strstr(start, "=");
  }
}
#endif

namespace Mycila {
  NTPClass NTP;
} // namespace Mycila
