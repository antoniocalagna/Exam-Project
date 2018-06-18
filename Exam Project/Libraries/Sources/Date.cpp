//
//  Date.cpp
//  Es.2 Lab5
//
//  Created by Antonio Calagna on 21/04/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include "Date.h"

Date::Date() {
  _day = 1;
  _month = 1;
  _year = 0;
}

Date::Date(const int &d, const int &m, const int &y) {
  _day = d;
  _month = m;
  _year = y;
  correctValues();
}

Date::Date(const string &date_to_set) {
  scanDateByStr(date_to_set);
}

Date::Date(const Date &to_copy) {
  _day = to_copy._day;
  _month = to_copy._month;
  _year = to_copy._year;
}

int Date::getDay() const {
  return _day;
}

int Date::getMonth() const {
  return _month;
}

int Date::getYear() const {
  return _year;
}

void Date::setDay(const int &d) {
  _day = d;
}

void Date::setMonth(const int &m) {
  _month = m;
}

void Date::setYear(const int &y) {
  _year = y;
}

string Date::getDate() {
  stringstream ss;
  ss<<*this;
  return ss.str();
}

bool Date::CheckDate(const Date &date_to_check) {
  return date_to_check._isValid();
}

bool Date::CheckDate(const string &date_to_check) {
  Date tmp(date_to_check);
  return tmp._isValid();
}

Date Date::Now() {
  Date tmp;
  tmp.setCurrentDate();
  return tmp;
}

void Date::correctValues() {
  while (!_isValid()) {
    if (_month > 12) {
      _month = _month - 12;
      _year++;
    }
    if ((_month == 2) && (_day > 28)) {
      _day = _day - 28;
      _month++;
    }
    if ((_month % 2 == _month / 8) && (_month != 2) && (_day > 31)) {
      _day = _day - 31;
      _month++;
    }
    if ((_month % 2 == 1 - _month / 8) && (_day > 30)) {
      _day = _day - 30;
      _month++;
    }
  }
}

void Date::setCurrentDate() {
  //Definisco le variabili di tipo "tempo" e la struttura richiesta da ctime.h
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[11];
  
  time(&rawtime); //Restituisce il tempo in secondi a partire da una data di default
  timeinfo = localtime(&rawtime); //Strutturizza quel tempo
  strftime(buffer, 11, "%Y %m %d", timeinfo); //Estraggo da tale struttura le informazioni desiderate convertendole in un vettore di caratteri. Lo specificatore "%x" determina il tipo di informazione, in rete sono reperibili tutti gli specificatori.
  
  stringstream sstream(buffer); //Traspongo il buffer in uno stringstream

  sstream>>_year; //Analisi formattata
  sstream>>_month;
  sstream>>_day;
}

int Date::yearsFrom(const Date &d) const {
  if (!((this->_isValid()) && (d._isValid())))
    return -1;
  if (d._month - _month > 0)
    return d._year - _year;
  if (d._month - _month < 0)
    return d._year - _year - 1;
  if (d._month == _month) {
    if (d._day - _day > 0)
      return d._year - _year;
    if (d._day - _day < 0)
      return d._year - _year - 1;
  }
  return -1;
}

void Date::scanDateByStr(const string &s) {
  //La funzione acquisisce una data da una stringa formattata nel seguente modo: dd/mm/yyyy
  std::stringstream ss(s);
  std::string day, month, year;
  std::getline(ss, day, '/');
  if (!ss.good())
    return;
  std::getline(ss, month, '/');
  if (!ss.good())
    return;
  std::getline(ss, year);
  if (!ss.good() && !ss.eof())
    return;
  _day = stoi(day);
  _month = stoi(month);
  _year = stoi(year);
}

ostream &operator<<(ostream &stream, const Date &d) {
  if (d._day<10)
    stream<<"0";
  stream << d._day << "/";
  if (d._month<10)
    stream<<"0";
  stream<< d._month << "/";
  if (d._year<1000)
  {
    if (d._year<10)
      stream<<"000";
    else if (d._year<100)
      stream<<"00";
    else if (d._year<1000)
      stream<<"0";
  }
  
  stream<< d._year;
  return stream;
}

void Date::operator=(const Date &to_be_assigned) {
  this->_day = to_be_assigned._day;
  this->_month = to_be_assigned._month;
  this->_year = to_be_assigned._year;
}

Date Date::operator+(const Date &to_be_added) {
  Date output;
  output._day = this->_day + to_be_added._day;
  output._month = this->_month + to_be_added._month;
  output._year = this->_year + to_be_added._year;
  
  output.correctValues();
  
  return output;
}


Date &Date::operator++() {
  _day++;
  this->correctValues();
  return *this; //Ritorno un nuovo oggetto copia del precedente
}

Date Date::operator++(int) {
  Date d(*this);
  ++(*this);
  return d;
}

bool Date::operator==(const Date &to_be_compared) {
  return ((_day == to_be_compared._day) && (_month == to_be_compared._month) && (_year == to_be_compared._year));
}

bool Date::operator!=(const Date &to_be_compared) {
  return !(*this == to_be_compared);
  
  // L'operatore "diverso" viene furbamente implementato richiamando l'operatore di uguaglianza e negandone il risultato.
  // In tal modo, correggendo un errore nel ==, != funzionerà di conseguenza.
  
}

bool Date::operator<(const Date &is_greater) {
  //Return true if *this comes before is_greater
  if (this->_year < is_greater._year)
    return true;
  if ((this->_year == is_greater._year) && (this->_month < is_greater._month))
    return true;
  if ((this->_year == is_greater._year) && (this->_month == is_greater._month) && (this->_day < is_greater._day))
    return true;
  
  return false;
}

bool Date::operator>(const Date &is_smaller) {
  return !(*this < is_smaller || *this == is_smaller);
}

bool Date::_isValid() const {
  if ((_year <= 0) || (_month <= 0) || (_day <= 0))
    return false;
  
  if (_month > 12)
    return false;
  
  if ((_month == 2) && (_day > 28))
    return false;
  
  if ((_month % 2 == 1 - _month / 8) && (_day > 31))
    return false;
  
  if ((_month % 2 == _month / 8) && (_month != 2) && (_day > 30))
    return false;
  
  return true;
} 

