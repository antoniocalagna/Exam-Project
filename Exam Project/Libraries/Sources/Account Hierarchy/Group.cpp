//
//  Group.cpp
//  Social Network
//
//  Created by Antonio Calagna on 22/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include "Group.h"

Group::Group()
{
  _legal_location="blank";
  _type_of_activity="blank";
  _type=group_type;
  _inception=Date();
}

Group::Group(const string &name, const string &ID, const string &legal_loc, const string &type_activity, const int &num, const Date &subscription, const Date &inception)
{
  _name=name;
  _ID=ID;
  _legal_location=legal_loc;
  _type_of_activity=type_activity;
  _subscription=subscription;
  _inception=inception;
  _type=group_type;
}

Group::Group(const Group &to_copy)
{
  *this=to_copy;
}

void Group::setLegalLocation(const std::string &location_to_set)
{
  _legal_location=location_to_set;
}

void Group::setTypeOfActivity(const std::string &type_to_set)
{
  _type_of_activity=type_to_set;
}

void Group::setInception(const Date &date_to_set)
{
  _inception=date_to_set;
}

string Group::getLegalLocation() const
{
  return _legal_location;
}

string Group::getTypeOfActivity() const
{
  return _type_of_activity;
}

Date Group::getInception() const
{
  return _inception;
}

void Group::operator=(const Group &to_be_assigned)
{
  this -> _name = to_be_assigned._name;
  this -> _ID = to_be_assigned._ID;
  this -> _type = to_be_assigned._type;
  this -> _subscription = to_be_assigned._subscription;
  
  this -> _legal_location = to_be_assigned._legal_location;
  this -> _type_of_activity = to_be_assigned._type_of_activity;
  this -> _inception = to_be_assigned._inception;
}

