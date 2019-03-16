#ifndef DTIME_H
#define DTIME_H

bool isLeapYear(uint16_t Y) {
  return !((Y % 4) * (!(Y % 100) + (Y % 400)));
}

uint8_t weekday(uint16_t Y, uint8_t M, uint8_t D) {
  uint8_t n[12] = {31, 28 + isLeapYear(Y), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  Y = (D + (((Y - 1) * 365UL) + ((Y - 1) / 4) - ((Y - 1) / 100) + ((Y - 1) / 400))) % 7;
  while (M > 1) Y += n[--M - 1];
  return Y % 7;
}

class DTime {
  private:
    uint8_t _month = 1, _weekday = 4, _day = 1, _hour = 0, _minute = 0, _second = 0;
    uint16_t _year = 1970;
    uint32_t _timestamp = 0;

    void decode() {
      uint32_t t = _timestamp;
      _second = t % 60;
      _minute = (t /= 60) % 60;
      _hour = (t /= 60) % 24;

      t /= 24;
      for (_year = 1970; t >= (365 + isLeapYear(_year)); _year++) t -= (365 + isLeapYear(_year));

      uint8_t n[12] = {31, 28 + isLeapYear(_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
      for (_month = 1; t >= n[_month - 1]; _month++) t -= n[_month - 1];

      _day = t + 1;
      _weekday = ::weekday(_year, _month, _day);
    }

    void encode() {
      _timestamp = ((_day - 1) * 86400UL) + (_hour * 3600UL) + (_minute * 60UL) + _second;

      uint8_t n[12] = {31, 28 + isLeapYear(_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
      for (uint8_t M = _month; M > 1;) _timestamp += (n[--M - 1] * 86400UL);

      for (uint16_t Y = _year; 1970 < Y; Y--) _timestamp += ((isLeapYear(Y - 1) + 365) * 86400UL);
      _weekday = ::weekday(_year, _month, _day);
    }

  public:
    const uint8_t &month = _month, &weekday = _weekday, &day = _day, &hour = _hour, &minute = _minute, &second = _second;
    const uint16_t &year = _year;
    const uint32_t &timestamp = _timestamp;

    explicit DTime() {};

    DTime setDate(uint16_t Y, uint8_t M, uint8_t D) {
      _year = Y;
      _month = M;
      _day = D;
      encode();
      return *this;
    }

    DTime setTimestamp(uint32_t t) {
      _timestamp = t;
      decode();
      return *this;
    }

    DTime setTime(uint8_t h, uint8_t m, uint8_t s) {
      _hour = h;
      _minute = m;
      _second = s;
      encode();
      return *this;
    }

    DTime tick() {
      _timestamp++;
      decode();
      return *this;
    }
};

#endif
