# MycilaNTP

[![Latest Release](https://img.shields.io/github/release/mathieucarbou/MycilaNTP.svg)](https://GitHub.com/mathieucarbou/MycilaNTP/releases/)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/mathieucarbou/library/MycilaNTP.svg)](https://registry.platformio.org/libraries/mathieucarbou/MycilaNTP)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](code_of_conduct.md)

[![Build](https://github.com/mathieucarbou/MycilaNTP/actions/workflows/ci.yml/badge.svg)](https://github.com/mathieucarbou/MycilaNTP/actions/workflows/ci.yml)
[![GitHub latest commit](https://badgen.net/github/last-commit/mathieucarbou/MycilaNTP)](https://GitHub.com/mathieucarbou/MycilaNTP/commit/)
[![Gitpod Ready-to-Code](https://img.shields.io/badge/Gitpod-Ready--to--Code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/mathieucarbou/MycilaNTP)

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

A=It is also possible to manually sync the time:

```c++
const timeval tv = {sec, millis};
Mycila::NTP.sync(tv);
```
