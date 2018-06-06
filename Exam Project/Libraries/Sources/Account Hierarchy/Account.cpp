//
//  Account.cpp
//  Social Network
//
//  Created by Antonio Calagna on 22/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include "Account.h"

Account::Account ()
{
    _name = "blank";
    _ID = "blank";
    _type = '\0';
    _subscription=Date();
}

void Account::setName(const std::string &name_to_set)
{
    _name=name_to_set;
}

void Account::setID(const std::string &id_to_set)
{
    _ID=id_to_set;
}

void Account::setType(const char &type_to_set)
{
    _type=type_to_set;
}

void Account::setSubscription(const Date &date_to_set)
{
    _subscription=date_to_set;
}

string Account::getName() const
{
    return _name;
}

string Account::getID() const
{
    return _ID;
}

char Account::getType() const
{
    return _type;
}

Date Account::getSubscription() const
{
    return _subscription;
}

bool Account::operator==(const Account &to_be_compared) const
{
    return (this->_ID==to_be_compared._ID);
}

bool Account::operator!=(const Account &to_be_compared) const
{
    return (!(this->_ID==to_be_compared._ID));
}

bool Account::operator>(const Account &is_smaller) const
{
    return (this->_ID>is_smaller._ID);
}

bool Account::operator<(const Account &is_greater) const
{
    return (this->_ID<is_greater._ID);
}

void Account::operator=(const Account &to_be_assigned)
{
    this -> _name = to_be_assigned._name;
    this -> _ID = to_be_assigned._ID;
    this -> _type = to_be_assigned._type;
    this -> _subscription = to_be_assigned._subscription;
}
