//
//  Account.cpp
//  Social Network
//
//  Created by Antonio Calagna on 22/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include "Account.h"

Account::Account() {
  _name = "blank";
  _ID = "blank";
  _type = '\0';
  _subscription = Date();
}

void Account::setName(const std::string &name_to_set) {
  _name = name_to_set;
}

void Account::setID(const std::string &id_to_set) {
  _ID = id_to_set;
}

void Account::setType(const char &type_to_set) {
  _type = type_to_set;
}

void Account::setSubscription(const Date &date_to_set) {
  _subscription = date_to_set;
}

string Account::getName() const {
  return _name;
}

string Account::getID() const {
  return _ID;
}

char Account::getType() const {
  return _type;
}

Date Account::getSubscription() const {
  return _subscription;
}

bool Account::operator==(const Account &to_be_compared) const {
  return (this->_ID == to_be_compared._ID);
}

bool Account::operator==(const string &string_to_compare) const {
  return (this->_ID == string_to_compare);
}
bool Account::operator!=(const Account &to_be_compared) const {
  return (!(this->_ID == to_be_compared._ID));
}

bool Account::operator>(const Account &is_smaller) const {
  return (this->_ID > is_smaller._ID);
}

bool Account::operator<(const Account &is_greater) const {
  return (this->_ID < is_greater._ID);
}

Account& Account::operator=(const Account &to_be_assigned) {
  this->_name = to_be_assigned._name;
  this->_ID = to_be_assigned._ID;
  this->_type = to_be_assigned._type;
  this->_subscription = to_be_assigned._subscription;
  return *this;
}

bool Account::IDValid(const std::string &ID) {
  size_t size = ID.size();
  if (size < 3 || size > 20) return false;                      //Limiti sulle dimensioni (per "convenzione")
  //Scorri l'ID e controlla che sia puramente alfanumerico e che contenga al massimo underscore
  for (int i = 0; i < size; i++) {
    if (!isalnum(ID[i]) && ID[i] != '_')
      return false;
  }
  return true;
}

bool Account::typeValid(char type) {
  if (type != user_type && type != group_type && type != company_type)
    return false;
  return true;
}

bool Account::nameValid(const std::string &name) {
  if (name.empty())
    return false;
  for (int i = 0; i < name.size(); i++) {
    if (!isalnum(name[i]) && name[i] != ' ' && name[i] != '\'' && name[i] != '-')               //Caratteri permessi
      return false;
  }
  return true;
}

bool relation::belong(const std::string &r) {
  return ((r == relation::friendship) ||
          (r == relation::knowings) ||
          (r == relation::parent) ||
          (r == relation::partner) ||
          (r == (relation::employee)) ||
          (r == relation::membership));
} 
