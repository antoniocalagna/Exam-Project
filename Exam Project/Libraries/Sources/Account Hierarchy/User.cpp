//
//  User.cpp
//  Social Network
//
//  Created by Antonio Calagna on 22/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include "User.h"

User::User()
{
  _surname="blank";
  _address="blank";
  _type=user_type;
  _birth=Date();
}

User::User(const string &name, const string &surname, const string &ID, const string &address, const Date &subscription, const Date &birth, const char &gender)
{
  _name=name;
  _surname=surname;
  _ID=ID;
  _address=address;
  _subscription=subscription;
  _birth=birth;
  _gender=gender;
  _type=user_type;
}

User::User(const User &to_copy)
{
  *this=to_copy;
}

void User::setSurname(const std::string &surname_to_set)
{
  _surname=surname_to_set;
}

void User::setAddress(const std::string &address_to_set)
{
  _address=address_to_set;
}

void User::setGender(const char &gender_to_set)
{
  _gender=gender_to_set;
}

void User::setBirth(const Date &birth_to_set)
{
  _birth=birth_to_set;
}

string User::getSurname() const
{
  return _surname;
}

string User::getAddress() const
{
  return _address;
}

char User::getGender() const
{
  return _gender;
}

Date User::getBirth() const
{
  return _birth;
}

void User::operator=(const User &to_be_assigned)
{
  this -> _name = to_be_assigned._name;
  this -> _ID = to_be_assigned._ID;
  this -> _type = to_be_assigned._type;
  this -> _subscription = to_be_assigned._subscription;
  
  this -> _surname = to_be_assigned._surname;
  this -> _address = to_be_assigned._address;
  this -> _gender = to_be_assigned._gender;
  this -> _birth = to_be_assigned._birth;
}

