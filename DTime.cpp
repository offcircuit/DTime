#include "DTime.h"

DTime::DTime(uint16_t Y, uint8_t M, uint8_t D, uint8_t h, uint8_t m, uint8_t s) {
  _year = Y;
  _month = M;
  _day = D;
  _hour = h;
  _minute = m;
  _second = s;
  encode();
}

void DTime::rebound(bool rebound) {
  _rebound = rebound;
  encode();
}

DTime DTime::setDate(uint16_t Y, uint8_t M, uint8_t D) {
  _year = Y;
  _month = M;
  _day = D;
  encode();
  return *this;
}

DTime DTime::setTime(uint8_t h, uint8_t m, uint8_t s) {
  _hour = h;
  _minute = m;
  _second = s;
  encode();
  return *this;
}

DTime DTime::setTimestamp(uint32_t t) {
  _timestamp = t;
  decode();
  return *this;
}

DTime DTime::tick() {
  _timestamp++;
  decode();
  if (_rebound && (_year != _outset) && (_outset = _year)) encode();
  return *this;
}

void DTime::decode() {
  uint32_t t = _timestamp;
  _second = t % 60;
  _minute = (t /= 60) % 60;
  _hour = (t /= 60) % 24;

  t /= 24;
  for (_year = _outset; t >= (365 + DTIME_LEAP_YEAR(_year)); _year++) t -= (365 + DTIME_LEAP_YEAR(_year));
  for (_month = 1; t >= DTIME_MONTH_DAYS(_year, _month); _month++) t -= DTIME_MONTH_DAYS(_year, _month);

  _day = t + 1;
  _weekday = DTIME_WEEKDAY(_year, _month, _day);
}

void DTime::encode() {
  _timestamp = ((DTIME_DAYS_BETWEEN_YEARS((_rebound ? _outset : DTIME_OUTSET), _year) + DTIME_YEAR_DAYS_UNTIL(_year, _month) + (_day - 1)) * 86400UL) + (_hour * 3600UL) + (_minute * 60UL) + _second;
  _weekday = DTIME_WEEKDAY(_year, _month, _day);
}
