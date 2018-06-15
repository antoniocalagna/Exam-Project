//
//  Company.h
//  Social Network
//
//  Created by Antonio Calagna on 22/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#ifndef Company_h
#define Company_h

#include "Account.h"

class Company:
public Account
{
public:
  
  //Constructors and Destructor
  Company();
  Company(const string &name, const string &ID, const string &financial_loc, const string &operative_loc, const string &type_product, const Date &subscription, const Date &inception);
  Company(const Company &to_copy);
  ~Company()=default;
  
  //Setters
  void setFinancialLocation (const string &location_to_set);
  void setOperativeLocation (const string &location_to_set);
  void setTypeOfProduct (const string &type_to_set);
  void setInception (const Date &date_to_set);
  
  //Getters
  string getFinancialLocation() const;
  string getOperativeLocation() const;
  string getTypeOfProduct() const;
  Date getInception() const;
  
  //Overloading Operators
  void operator=(const Company &to_be_assigned);
  
private:
  
  string _financial_location;
  string _operative_location;
  string _type_of_product;
  Date _inception;
  //..and all the encapsulated attributes inherited by Account.
};


#endif /* Company_h */ 
