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
  
  Clock(const unsigned int &hours, const unsigned int &minutes, const unsigned int &seconds);
  Clock(const unsigned int &hours, const unsigned int &minutes);
  explicit Clock (const string &time);
  
  Clock (const Clock &to_copy);
  
  void setHours(const unsigned int &hours);
  void setMinutes(const unsigned int &minutes);
  void setSeconds(const unsigned int &seconds);
  
  int getHours() const;
  int getMinutes() const;
  int getSeconds() const;
  string getFullTime() const;
  string getShortTime() const;
  
  void CorrectValues();
  void setCurrentTime();
  void scanTimeByStr (string s);
  
  static bool CheckTime (const string &time_to_check);
  
  //Overloading operators:
  
  friend ostream& operator<< (ostream& stream, const Clock& d);
  Clock operator+ (const Clock &to_be_added);
  Clock &operator=(const Clock &to_be_assigned) = default;  //Operatore di uguaglianza triviale
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
