//
//  Clock.cpp
//  Social Network
//
//  Created by Antonio Calagna on 23/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include "Clock.h"

Clock::Clock()
{
  _hours=0;
  _minutes=0;
  _seconds=0;
}

Clock::Clock(const int &hours, const int &minutes, const int &seconds)
{
  _hours=hours;
  _minutes=minutes;
  _seconds=seconds;
  CorrectValues();
}

Clock::Clock(const int &hours, const int &minutes)
{
  _hours=hours;
  _minutes=minutes;
  _seconds=0;
  CorrectValues();
}

Clock::Clock(const Clock &to_copy)
{
  _hours=to_copy._hours;
  _minutes=to_copy._minutes;
  _seconds=to_copy._seconds;
}

Clock::Clock(const string &time)
{
  scanTimeByStr(time);
}

void Clock::setHours(const int &hours)
{
  _hours=hours;
}

void Clock::setMinutes(const int &minutes)
{
  _minutes=minutes;
}

void Clock::setSeconds(const int &seconds)
{
  _seconds=seconds;
}

int Clock::getHours() const
{
  return _hours;
}

int Clock::getMinutes() const
{
  return _minutes;
}

int Clock::getSeconds() const
{
  return _seconds;
}

string Clock::getFullTime() const
{
  stringstream ss;
  ss << _hours << ":" << _minutes << ":" << _seconds;
  return ss.str();
}

string Clock::getShortTime() const
{
  stringstream ss;
  ss << _hours << ":" << _minutes;
  return ss.str();
}

bool Clock::_isValid() const
{
  if (_hours>23)
    return false;
  if (_minutes>59)
    return false;
  if (_seconds>59)
    return false;
  
  return true;
}

void Clock::CorrectValues()
{
  while (!_isValid())
  {
    if (_seconds>59)
    {
      _seconds=_seconds-60;
      _minutes++;
    }
    if (_minutes>59)
    {
      _minutes=_minutes-60;
      _hours++;
    }
    if (_hours>23)
      _hours=_hours-24;
  }
}

void Clock::SetCurrentTime()
{
  time_t raw;
  struct tm * timeinfo;
  char buffer [9];
  time (&raw); //Tempo in secondi
  timeinfo = localtime(&raw); //Strutturizza
  strftime(buffer, 9, "%H %M %S", timeinfo); //Formatta
  
  stringstream sstream (buffer);
  sstream>>_hours;
  sstream>>_minutes;
  sstream>>_seconds;
}

void Clock::scanTimeByStr(std::string s)
{
  stringstream sstream (s);
  sstream>>_hours;
  sstream.ignore(1);
  sstream>>_minutes;
  sstream.ignore(1);
  if(!sstream.good()){
    _seconds = 0;
  }else {
    sstream >> _seconds;
  }
}

bool Clock::CheckTime (const string &time_to_check)
{
  Clock tmp (time_to_check);
  return tmp._isValid();
}

ostream &operator<< (ostream &stream, const Clock& c)
{
  stream << " @ ";
  
  if (c._hours<10)
    stream << "0";
  stream << c._hours <<":";
  if (c._minutes<10)
    stream << "0";
  stream<<c._minutes;
  if (c._seconds!=0)
  {
    if (c._seconds<10)
      stream<<":"<<"0";
    else
      stream<<":";
    
    stream<<c._seconds;
  }
  
  stream<< endl;
  return stream;
}

Clock Clock::operator+(const Clock &to_be_added)
{
  Clock output;
  output._hours = this -> _hours+to_be_added._hours;
  output._minutes = this -> _minutes+to_be_added._minutes;
  output._seconds = this -> _seconds+to_be_added._seconds;
  
  output.CorrectValues();
  
  return output;
}

void Clock::operator=(const Clock &to_be_assigned)
{
  this -> _hours = to_be_assigned._hours;
  this -> _minutes = to_be_assigned._minutes;
  this -> _seconds = to_be_assigned._seconds;
}

const Clock& Clock::operator++()
{
  _seconds++;
  this->CorrectValues();
  return *this;
}

const Clock Clock::operator++(int)
{
  Clock c(*this);
  ++(*this);
  return c;
}

bool Clock::operator==(const Clock &to_be_compared)
{
  return ((_hours == to_be_compared._hours) && (_minutes == to_be_compared._minutes) && (_seconds == to_be_compared._seconds));
}

bool Clock::operator!=(const Clock &to_be_compared)
{
  return !(*this == to_be_compared);
}

bool Clock::operator<(const Clock &is_greater)
{
  if (this->_hours < is_greater._hours)
    return true;
  if ((this->_hours == is_greater._hours)&&(this->_minutes < is_greater._minutes))
    return true;
  if ((this->_hours == is_greater._hours)&&(this->_minutes == is_greater._minutes)&&(this->_seconds < is_greater._seconds))
    return true;
  
  return false;
}

bool Clock::operator>(const Clock &is_smaller)
{
  return !(*this < is_smaller || *this == is_smaller);
}
 
