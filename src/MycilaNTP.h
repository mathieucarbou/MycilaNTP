// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou
 */
#pragma once

#include <Ticker.h>
#include <time.h>

#include <functional>
#include <string>

#ifdef MYCILA_JSON_SUPPORT
  #include <ArduinoJson.h>
#endif

#define MYCILA_NTP_VERSION          "6.0.2"
#define MYCILA_NTP_VERSION_MAJOR    6
#define MYCILA_NTP_VERSION_MINOR    0
#define MYCILA_NTP_VERSION_REVISION 2

#ifndef MYCILA_NTP_RETRY_INTERVAL
  #define MYCILA_NTP_RETRY_INTERVAL 2
#endif

namespace Mycila {
  class NTPClass {
    public:
      // returns false if timezone  invalid
      bool setTimeZone(const char* timezone);

      // ge the timezone information used
      const std::string& getTimezoneInfo() const { return _spec; }

      // sync with server
      bool sync(const char* server, const uint8_t retryInterval = MYCILA_NTP_RETRY_INTERVAL);

      // manually sync
      bool sync(const timeval& tv);

      // true when time is synced, never changes after
      bool isSynced() const { return _synced; }

#ifdef MYCILA_JSON_SUPPORT
      // map of timezone names and timezone info
      void timezonesToJsonObject(const JsonObject& doc) const;
      // list of timezone names
      void timezonesToJsonArray(const JsonArray& doc) const;
#endif

    private:
      volatile bool _synced = false;
      std::string _spec;
      std::string _server;
      Ticker _ticker;
  };

  extern NTPClass NTP;
} // namespace Mycila
