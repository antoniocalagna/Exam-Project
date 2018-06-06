//
//  Date.h
//  Es.2 Lab5
//
//  Created by Antonio Calagna on 21/04/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#ifndef Date_h
#define Date_h

#include <iostream>
#include <ctime>
#include <cstdio>
#include <string>
#include <sstream>

using namespace std;

//Copio la classe fornita dall'esercizio e aggiungo gli overloading degli operatori che ritengo necessari.

class Date
{
public:
  
  // Dafault Constructor, set the date to the actual date
  Date();
  // Constructor, pass the date in the form of dd,mm,yyyy
  Date(const int &d, const int &m, const int &y);
  // Copy Constructor
  Date(const Date &to_copy);
  //Destructor
  ~Date()=default;
  
  // Getters
  int getYear() const;
  int getMonth() const;
  int getDay() const;
  string getDate();
  
  //Setters
  void setDay(const int &d);
  void setMonth(const int &m);
  void setYear(const int &y);
  
  // Returns the year from the date d, if d it not passed, it returns the year from now.
  // Useful to compute the age of a person, where this is the person’s birth date
  int yearsFrom(const Date &d) const;
  // Sets the private attribute _date (the date as a string in the form dd/mm/yyyy)
  void Acquire(string s);
  //Sets all attributes to actual time.
  void SetCurrentDate();
  void CorrectValues ();
  
  //Overloading operators:
  
  friend ostream& operator<< (ostream& stream, const Date& d);
  Date operator+ (const Date &to_be_added); //Somma due date
  void operator=(const Date &to_be_assigned);
  //Assegnare una data ad un'altra.
  const Date& operator++(); //Incremento di tipo prefix: (assegnazione post incremento) (ritorna una reference)
  const Date operator++(int);
  bool operator==(const Date &to_be_compared); //Confronto di uguaglianza
  bool operator!=(const Date &to_be_compared);
  bool operator>(const Date &is_smaller);
  bool operator<(const Date &is_greater);
  
  
  
private:
  
  bool _isValid() const;
  
  int _year;
  int _month;
  int _day;
};

#endif /* Date_h */
