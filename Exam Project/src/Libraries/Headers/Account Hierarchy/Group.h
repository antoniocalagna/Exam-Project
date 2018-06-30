//
//  Group.h
//  Social Network
//
//  Created by Antonio Calagna on 22/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#ifndef Group_h
#define Group_h

#include "Account.h"

class Group:
public Account
{
public:
  
  //Constructors and Destructor
  Group();
  Group(const string &name, const string &ID, const string &legal_loc, const string &type_activity, const Date &subscription, const Date &inception);
  Group(const Group &to_copy);
  ~Group()=default;
  
  //Setters
  void setLegalLocation (const string &location_to_set);
  void setTypeOfActivity (const string &type_to_set);
  void setInception (const Date &date_to_set);
  
  //Getters
  string getLegalLocation() const;
  string getTypeOfActivity() const;
  Date getInception() const;
  
  //Overloading Operators
  Group &operator=(const Group &to_be_assigned);
  
  
private:
  
  string _legal_location;
  string _type_of_activity;
  Date _inception;
  
};

#endif /* Group_h */ 
