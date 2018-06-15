//
//  Clock.hpp
//  Social Network
//
//  Created by Antonio Calagna on 23/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#ifndef Clock_h
#define Clock_h

#include <iostream>
#include <ctime>
#include <cstdio>
#include <string>
#include <sstream>

using namespace std;

class Clock
{
public:
  Clock();
  ~Clock()=default;
  
  Clock (const int &hours, const int &minutes, const int &seconds);
  Clock (const int &hours, const int &minutes);
  
  Clock (const Clock &to_copy);
  
  void setHours(const int &hours);
  void setMinutes(const int &minutes);
  void setSeconds(const int &seconds);
  
  int getHours() const;
  int getMinutes() const;
  int getSeconds() const;
  string getFullTime() const;
  string getShortTime() const;
  
  void CorrectValues();
  void SetCurrentTime();
  void Acquire (string s);
  
  //Overloading operators:
  
  friend ostream& operator<< (ostream& stream, const Clock& d);
  Clock operator+ (const Clock &to_be_added);
  void operator=(const Clock &to_be_assigned);
  const Clock& operator++();
  const Clock operator++(int);
  bool operator==(const Clock &to_be_compared);
  bool operator!=(const Clock &to_be_compared);
  bool operator>(const Clock &is_smaller);
  bool operator<(const Clock &is_greater);
  
  
private:
  bool _isValid() const;
  
  unsigned int _hours;
  unsigned int _minutes;
  unsigned int _seconds;
};

#endif /* Clock_h */ 
