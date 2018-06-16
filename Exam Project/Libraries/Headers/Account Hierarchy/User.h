//
//  User.h
//  Social Network
//
//  Created by Antonio Calagna on 22/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#ifndef User_h
#define User_h

#include "Account.h"

class User :
public Account {
public:
  
  //Constructors and Destructor
  User();
  User(const string &name, const string &surname, const string &ID, const string &address, const Date &subscription,
       const Date &birth, const char &gender);
  User(const User &to_copy);
  ~User() = default;
  
  //Setters
  void setSurname(const string &surname_to_set);
  void setAddress(const string &address_to_set);
  void setGender(const char &gender_to_set);
  void setBirth(const Date &birth_to_set);
  
  //Getters
  string getSurname() const;
  string getAddress() const;
  char getGender() const;
  Date getBirth() const;
  
  //Overloading Operators
  User &operator=(const User &to_be_assigned);


private:
  
  string _surname;
  string _address;
  char _gender;
  Date _birth;
  //..and all the encapsulated attributes inherited by Account.
};

namespace gender {
const char genders[] = {'M', 'F', 'N', 'A', 'B', 'O'};
bool isValid(char g);
}

#endif /* User_h */
