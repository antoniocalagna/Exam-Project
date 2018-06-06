//
//  Account.h
//  Social Network
//
//  Created by Antonio Calagna on 22/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#ifndef Account_h
#define Account_h

#include <vector>
#include "Date.h"
#include "Clock.h"

class Account
{
public:
  
  //Constructors and Destructor
  Account();
  ~Account()=default;
  
  //Setters
  void setName (const string &name_to_set);
  void setID (const string &id_to_set);
  void setType (const char &type_to_set);
  void setSubscription (const Date &date_to_set);
  
  //Getters
  string getName() const;
  string getID() const;
  char getType() const;
  Date getSubscription() const;
  
  //Overloading Operators:
  bool operator==(const Account &to_be_compared) const;
  bool operator!=(const Account &to_be_compared) const;
  bool operator>(const Account &is_smaller) const;
  bool operator<(const Account &is_greater) const;
  void operator=(const Account &to_be_assigned);
  
  //Static types
  static const char user_type = 'U';
  static const char company_type = 'C';
  static const char group_type = 'G';
  
protected:
  
  string _name;
  string _ID;
  char _type;
  Date _subscription;
  
};


#endif /* Account_h */