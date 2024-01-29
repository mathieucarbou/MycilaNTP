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

#define MYCILA_NTP_VERSION "1.0.0"
#define MYCILA_NTP_VERSION_MAJOR 1
#define MYCILA_NTP_VERSION_MINOR 0
#define MYCILA_NTP_VERSION_REVISION 1

#ifndef MYCILA_NTP_RETRY_INTERVAL
#define MYCILA_NTP_RETRY_INTERVAL 2
#endif

namespace Mycila {
  class NTPClass {
    public:
      void setNTPServer(const String& server, const uint8_t retryInterval = MYCILA_NTP_RETRY_INTERVAL);

      // set timezone, returns false if empty or invalid
      bool setTimezone(const String& timezone);

      // ge the timezone information used
      const String& getTimezoneInfo() const { return _spec; }

      // trigger a time request, eventually by setting a time (fro ma browser)
      bool update(const timeval* tv = nullptr);

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
