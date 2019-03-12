#ifndef DTIME_H
#define DTIME_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class DTime {
  public:
    const uint8_t &month = _month, &weekday = _weekday, &day = _day, &hour = _hour, &minute = _minute, &second = _second;
    const uint16_t &year = _year;
    const uint32_t &timestamp = _timestamp;

    explicit DTime() {};
    explicit DTime(uint32_t t): _timestamp(t) {
      decode();
    };
    DTime(uint16_t Y, uint8_t M, uint8_t D, uint8_t h, uint8_t m, uint8_t s);

    DTime setDate(uint16_t Y, uint8_t M, uint8_t D);
    DTime setTime(uint8_t h, uint8_t m, uint8_t s);
    DTime setTimestamp(uint32_t t);
    DTime tick();

    bool isLeapYear(uint16_t Y);
    uint8_t wday(uint16_t Y, uint8_t M, uint8_t D);

  private:
    uint8_t _month = 1, _weekday = 4, _day = 1, _hour = 0, _minute = 0, _second = 0;
    uint16_t _year = 1970;
    uint32_t _timestamp = 0;

    void decode();
    void encode();
};

#endif
