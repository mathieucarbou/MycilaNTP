# MycilaNTP

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Continuous Integration](https://github.com/mathieucarbou/MycilaNTP/actions/workflows/ci.yml/badge.svg)](https://github.com/mathieucarbou/MycilaNTP/actions/workflows/ci.yml)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/mathieucarbou/library/MycilaNTP.svg)](https://registry.platformio.org/libraries/mathieucarbou/MycilaNTP)

A simple and efficient NTP library for ESP32 / Arduino

## Basic Usage

Setup:

```c++
Mycila::NTP.setTimezone("Europe/Paris")
Mycila::NTP.setNTPServer("pool.ntp.org")
```

Then query state:

```c++
Mycila::NTP.isSynced();
Mycila::NTP.getTimezoneInfo();
```
