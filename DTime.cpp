#include "DTime.h"

// PUBLIC

DTime::DTime(uint16_t Y, uint8_t M, uint8_t D, uint8_t h, uint8_t m, uint8_t s) {
  _year = Y;
  _month = M;
  _day = D;
  _hour = h;
  _minute = m;
  _second = s;
  encode();
  _weekday = (_timestamp - (_timestamp % 86400UL) + 4) % 7;
}

DTime DTime::setDate(uint16_t Y, uint8_t M, uint8_t D) {
  _year = Y;
  _month = M;
  _day = D;
  encode();
  _weekday = (_timestamp - (_timestamp % 86400UL) + 4) % 7;
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
  return *this;
}

//PRIVATE

void DTime::decode() {
  uint32_t t = _timestamp;
  _second = t % 60;
  _minute = (t /= 60) % 60;
  _hour = (t /= 60) % 24;
  _weekday = (((t /= 24) + 4) % 7);
  for (_year = 1970; t > (365 + isLeapYear(_year)); _year++) t -= (365 + isLeapYear(_year));
  uint8_t n[12] = {31, 28 + isLeapYear(_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for (_month = 1; t >= n[_month - 1]; _month++) t -= n[_month - 1];
  _day = t + 1;
}

void DTime::encode() {
  _timestamp = ((_day - 1) * 86400UL) + (_hour * 3600UL) + (_minute * 60UL) + _second;
  uint8_t n[12] = {31, 28 + isLeapYear(_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for (uint8_t M = _month; 1 < M; M--) _timestamp += (n[M - 2] * 86400UL);
  for (uint16_t Y = _year; 1970 < Y; Y--) _timestamp += ((isLeapYear(Y - 1) + 365) * 86400UL);
}

bool DTime::isLeapYear(uint16_t Y) {
  return !((Y % 4) * (!(Y % 100) + (Y % 400)));
}
