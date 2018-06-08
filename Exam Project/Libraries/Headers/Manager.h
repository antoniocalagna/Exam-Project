//
//  Manager.hpp
//  Social Network
//
//  Created by Antonio Calagna on 02/06/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#ifndef Manager_h
#define Manager_h

#include "User.h"
#include "Company.h"
#include "Group.h"
#include "Graph.hpp"
#include "TemplateFunctions.hpp"


namespace relation
{
  const string friendship = "friend";
  const string fatherhood = "father";
  const string partner = "partner";
}

class Manager
{
public:
  
  //Constructors and Destructor
  Manager()=default;
  Manager(const vector<User> &users, const vector<Company> &companies, const vector<Group> &groups);
  ~Manager()=default;
  
  //Setters
  void setUsers (const vector<User> &users_to_set);
  void setCompanies (const vector<Company> &companies_to_set);
  void setGroups (const vector<Group> &groups_to_set);
  
  //Getters
  vector<User> getUsers() const;
  vector<Company> getCompanies() const;
  vector<Group> getGroups() const;
  
  //Functions
  void addUser (const User &user_to_add);
  void addCompany (const Company &company_to_add);
  void addGroup (const Group &group_to_add);
  void addNode (const string &ID_to_add);
  
  void delUserAtPos (const unsigned int &i);
  void delCompanyAtPos (const unsigned int &i);
  void delGroupAtPos (const unsigned int &i);
  void deleteEdge (const string &ID_to_modify, const string &relationship_to_remove);
  
  void modifyUserAtPos (const unsigned int &i, const User &modified_user);
  void modifyCompanyAtPos (const unsigned int &i, const Company &modified_company);
  void modifyGroupAtPos (const unsigned int &i, const Group &modified_group);
  
  User getUserByPos (const unsigned int &i) const;
  Company getCompanyByPos (const unsigned int &i) const;
  Group getGroupByPos (const unsigned int &i) const;
  vector<Account> getAllAccounts() const;
  
  void addDirectedRelationship (const string &ID_start, const string &ID_target, const string &relationship);
  
  void addUndirectedRelationship (const string &ID_start, const string &ID_target, const string &relationship);
  
  vector<string> getListConnection (const string &starting_ID, const string &relationship);
  
  
private:
  vector<User> _users;
  vector<Company> _companies;
  vector<Group> _groups;
  Graph<string, string> _graph;
  
  void _setNodes();
  void _deleteNode(const string &ID_to_delete);
  size_t _findNodePos(const string &ID_to_find);
  
};

#endif /* Manager_h */
