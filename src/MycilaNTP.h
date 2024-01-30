// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou and others
 */
#pragma once

#include <Ticker.h>
#include <WString.h>
#include <functional>

#ifdef MYCILA_NTP_JSON_SUPPORT
#include <ArduinoJson.h>
#endif

#define MYCILA_NTP_VERSION "2.0.0"
#define MYCILA_NTP_VERSION_MAJOR 2
#define MYCILA_NTP_VERSION_MINOR 0
#define MYCILA_NTP_VERSION_REVISION 0

#ifndef MYCILA_NTP_RETRY_INTERVAL
#define MYCILA_NTP_RETRY_INTERVAL 2
#endif

namespace Mycila {
  class NTPClass {
    public:
      // returns false if timezone  invalid
      bool setTimeZone(const String& timezone);

      // ge the timezone information used
      const String& getTimezoneInfo() const { return _spec; }

      // sync with server
      bool sync(const String& server, const uint8_t retryInterval = MYCILA_NTP_RETRY_INTERVAL);

      // manually sync
      bool sync(const timeval* tv);

      // true when time is synced, never changes after
      bool isSynced() const { return _synced; }

#ifdef MYCILA_NTP_JSON_SUPPORT
      void timezonesToJson(const JsonObject& doc) const;
#endif

    private:
      volatile bool _synced = false;
      String _spec;
      String _server;
      Ticker _ticker;
  };

  extern NTPClass NTP;
} // namespace Mycila
