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

bool DTime::isLeapYear(uint16_t Y) {
  return !((Y % 4) * (!(Y % 100) + (Y % 400)));
}

void DTime::outset(uint16_t outset) {
  _rebound = false;
  _outset = outset;
  
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

uint8_t DTime::wday(uint16_t Y, uint8_t M, uint16_t D) {
  uint8_t n[12] = {31, 28 + isLeapYear(Y), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  while (--M > 1) D += n[M - 1];
  
  return uint8_t(Y + floor(Y / 4) - floor(Y / 100) + floor(Y / 400) + D + 3) % 7;
}

void DTime::decode() {
  uint32_t t = _timestamp;
  
  _second = t % 60;
  _minute = (t /= 60) % 60;
  _hour = (t /= 60) % 24;
  
  for (_year = _outset; t > (365 + isLeapYear(_year)); _year++) t -= (365 + isLeapYear(_year));
  
  uint8_t n[12] = {31, 28 + isLeapYear(_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for (_month = 1; t >= n[_month - 1]; _month++) t -= n[_month - 1];
  
  _day = t + 1;
  _weekday = wday(_year, _month, _day);
}

void DTime::encode() {
  if (_rebound) _outset = _year;
  
  _weekday = wday(_year, _month, _day);
  _timestamp = ((_day - 1) * 86400UL) + (_hour * 3600UL) + (_minute * 60UL) + _second;
  
  uint8_t n[12] = {31, 28 + isLeapYear(_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for (uint8_t M = _month; 1 < M; M--) _timestamp += (n[M - 1] * 86400UL);
  
  for (uint16_t Y = _year; _outset < Y; Y--) _timestamp += ((isLeapYear(Y - 1) + 365) * 86400UL);
}
