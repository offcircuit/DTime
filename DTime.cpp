#include "DTime.h"

// PUBLIC

DTime::DTime(unsigned short Y, byte M, byte D, byte h, byte m, byte s) {
  if (!((Y < 1970) || (Y > 9999) || (M < 1) || (M > 12) || (D < 1) || (h < 0) || (h > 23) || (m < 0) || (m > 59) || (s < 0) || (s > 59))) {
    byte n[12] = {31, 28 + isLeapYear(Y), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (D <= n[M - 1]) {
      _year = Y;
      _month = M;
      _day = D;
      _hour = h;
      _minute = m;
      _second = s;
      encode();
    }
  }
}

DTime DTime::setDate(unsigned short Y, byte M, byte D) {
  if (!((Y < 1970) || (Y > 9999) || (M < 1) || (M > 12) || (D < 1))) {
    byte n[12] = {31, 28 + isLeapYear(Y), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (D <= n[M - 1]) {
      _year = Y;
      _month = M;
      _day = D;
      encode();
      _weekday = (_timestamp - (_timestamp % 86400UL) + 4) % 7;
    }
  }
  return *this;
}

DTime DTime::setTime(byte h, byte m, byte s) {
  if (!((h < 0) || (h > 23) || (m < 0) || (m > 59) || (s < 0) || (s > 59))) {
    _hour = h;
    _minute = m;
    _second = s;
    encode();
  }
  return *this;
}

DTime DTime::setTimestamp(unsigned long t) {
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
  unsigned long t = _timestamp;
  _second = t % 60;
  _minute = (t /= 60) % 60;
  _hour = (t /= 60) % 24;
  _weekday = (((t /= 24) + 4) % 7);
  for (_year = 1970; t > (365 + isLeapYear(_year)); _year++) t -= (365 + isLeapYear(_year));
  byte n[12] = {31, 28 + isLeapYear(_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for (_month = 1; t >= n[_month - 1]; _month++) t -= n[_month - 1];
  _day = t + 1;
}

void DTime::encode() {
  _timestamp = ((_day - 1) * 86400UL) + (_hour * 3600UL) + (_minute * 60UL) + _second;
  byte n[12] = {31, 28 + isLeapYear(_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for (byte M = _month; 1 < M; M--) _timestamp += (n[M - 2] * 86400UL);
  for (unsigned short Y = _year; 1970 < Y; Y--) _timestamp += ((isLeapYear(Y - 1) + 365) * 86400UL);
}

bool DTime::isLeapYear(unsigned short Y) {
  return !((Y % 4) * (!(Y % 100) + (Y % 400)));
}
