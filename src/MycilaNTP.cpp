// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou
 */
#include <MycilaNTP.h>
#include <MycilaNTPSpecs.h>

#include <Arduino.h>
#include <sys/time.h>
#include "lwip/apps/sntp.h"
#include <esp_netif.h>

#ifdef CONFIG_LWIP_TCPIP_CORE_LOCKING
  #include "lwip/priv/tcpip_priv.h"
#endif

#include <string>

#ifdef MYCILA_LOGGER_SUPPORT
  #include <MycilaLogger.h>
extern Mycila::Logger logger;
  #define LOGD(tag, format, ...) logger.debug(tag, format, ##__VA_ARGS__)
  #define LOGI(tag, format, ...) logger.info(tag, format, ##__VA_ARGS__)
  #define LOGW(tag, format, ...) logger.warn(tag, format, ##__VA_ARGS__)
  #define LOGE(tag, format, ...) logger.error(tag, format, ##__VA_ARGS__)
#else
  #define LOGD(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
  #define LOGI(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
  #define LOGW(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
  #define LOGE(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#endif

#define TAG "NTP"

bool Mycila::NTPClass::setTimeZone(const char* timezone) {
  if (!timezone)
    return false;

  size_t len = strlen(timezone);
  if (len == 0)
    return false;

  char* withEqual = new char[len + 2];
  memcpy(withEqual, timezone, len);
  withEqual[len] = '=';
  withEqual[len + 1] = '\0';

  char* found = strstr(MYCILA_NTP_SPEC, withEqual);
  delete[] withEqual;

  if (found == nullptr) {
    LOGE(TAG, "Timezone not found: %s", timezone);
    return false;
  }

  const char* start = found + len + 1;
  _spec = std::string(start, static_cast<unsigned int>(strstr(start, "\n") - start));

  LOGI(TAG, "Set timezone to %s (%s)", timezone, _spec.c_str());

  setenv("TZ", _spec.c_str(), 1);
  tzset();

  return true;
}

bool Mycila::NTPClass::sync(const char* server, const uint8_t retryInterval) {
  if (!server || strlen(server) == 0)
    return false;

  _server = server;
  _ticker.detach();

  esp_netif_init();
  if (sntp_enabled()) {
    sntp_stop();
  }

#ifdef CONFIG_LWIP_TCPIP_CORE_LOCKING
  if (!sys_thread_tcpip(LWIP_CORE_LOCK_QUERY_HOLDER))
    LOCK_TCPIP_CORE();
#endif

  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, _server.c_str());
  sntp_init();

#ifdef CONFIG_LWIP_TCPIP_CORE_LOCKING
  if (sys_thread_tcpip(LWIP_CORE_LOCK_QUERY_HOLDER))
    UNLOCK_TCPIP_CORE();
#endif

  if (!_spec.empty()) {
    setenv("TZ", _spec.c_str(), 1);
    tzset();
  }

  struct tm timeInfo;
  getLocalTime(&timeInfo, 5);

  if (!_synced) {
    LOGI(TAG, "Syncing time with %s", _server.c_str());
    _ticker.attach(
      retryInterval,
      +[](NTPClass* instance) {
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
  if (!_spec.empty()) {
    setenv("TZ", _spec.c_str(), 1);
    tzset();
  }

  LOGI(TAG, "Time synced manually");

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
    const std::string timezone(start, static_cast<size_t>(token - start));
    start = token + 1;
    token = strstr(start, "\n");
    const std::string spec(start, static_cast<size_t>(token - start));
    start = token + 1;
    token = strstr(start, "=");
    doc[timezone] = spec;
  }
}

void Mycila::NTPClass::timezonesToJsonArray(const JsonArray& doc) const {
  char* start = const_cast<char*>(MYCILA_NTP_SPEC);
  char* token = strstr(start, "=");
  while (token != nullptr) {
    doc.add(std::string(start, static_cast<size_t>(token - start)));
    token = strstr(token + 1, "\n");
    start = token + 1;
    token = strstr(start, "=");
  }
}
#endif

namespace Mycila {
  NTPClass NTP;
} // namespace Mycila
