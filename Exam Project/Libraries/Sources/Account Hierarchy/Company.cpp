//
//  Company.cpp
//  Social Network
//
//  Created by Antonio Calagna on 22/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include "Company.h"

Company::Company()
{
  _financial_location="blank";
  _operative_location="blank";
  _type_of_product="blank";
  _type=company_type;
  _inception=Date();
}

Company::Company(const string &name, const string &ID, const string &financial_loc, const string &operative_loc, const string &type_product, const Date &subscription, const Date &inception)
{
  _name=name;
  _ID=ID;
  _financial_location=financial_loc;
  _operative_location=operative_loc;
  _type_of_product=type_product;
  _subscription=subscription;
  _inception=inception;
  _type=company_type;
}

Company::Company(const Company &to_copy)
{
  *this=to_copy;
}

void Company::setFinancialLocation(const std::string &location_to_set)
{
  _financial_location=location_to_set;
}

void Company::setOperativeLocation(const std::string &location_to_set)
{
  _operative_location=location_to_set;
}

void Company::setTypeOfProduct(const std::string &type_to_set)
{
  _type_of_product=type_to_set;
}

void Company::setInception(const Date &date_to_set)
{
  _inception=date_to_set;
}

string Company::getFinancialLocation() const
{
  return _financial_location;
}

string Company::getOperativeLocation() const
{
  return _operative_location;
}

string Company::getTypeOfProduct() const
{
  return _type_of_product;
}

Date Company::getInception() const
{
  return _inception;
}

void Company::operator=(const Company &to_be_assigned)
{
  this -> _name = to_be_assigned._name;
  this -> _ID = to_be_assigned._ID;
  this -> _type = to_be_assigned._type;
  this -> _subscription = to_be_assigned._subscription;
  
  this -> _financial_location = to_be_assigned._financial_location;
  this -> _operative_location = to_be_assigned._operative_location;
  this -> _type_of_product = to_be_assigned._type_of_product;
  this -> _inception = to_be_assigned._inception;
} 

