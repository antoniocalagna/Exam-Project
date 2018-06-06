//
//  Manager.cpp
//  Social Network
//
//  Created by Antonio Calagna on 02/06/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include "Manager.h"

Manager::Manager(const vector<User> &users, const vector<Company> &companies, const vector<Group> &groups)
{
  _users=users;
  _companies=companies;
  _groups=groups;
  _setNodes();
}

void Manager::setUsers(const vector<User> &users_to_set)
{
  _users=users_to_set;
}

void Manager::setCompanies(const vector<Company> &companies_to_set)
{
  _companies=companies_to_set;
}

void Manager::setGroups(const vector<Group> &groups_to_set)
{
  _groups=groups_to_set;
}

vector<User> Manager::getUsers() const
{
  return _users;
}

vector<Company> Manager::getCompanies() const
{
  return _companies;
}

vector<Group> Manager::getGroups() const
{
  return _groups;
}

void Manager::addUser(const User &user_to_add)
{
  insert_sorted(_users, user_to_add);
}

void Manager::addCompany(const Company &company_to_add)
{
  insert_sorted(_companies, company_to_add);
}

void Manager::addGroup(const Group &group_to_add)
{
  insert_sorted(_groups, group_to_add);
}

void Manager::addNode(const std::string &ID_to_add)
{
  _graph.addNode(ID_to_add);
}

void Manager::delUserAtPos(const unsigned int &i)
{
  _users.erase(_users.begin()+i);
}

void Manager::delCompanyAtPos(const unsigned int &i)
{
  _companies.erase(_companies.begin()+i);
}

void Manager::delGroupAtPos(const unsigned int &i)
{
  _groups.erase(_groups.begin()+i);
}

void Manager::deleteEdge(const std::string &ID_to_modify, const std::string &relationship_to_remove)
{
  //NEED FUNCTION
}

void Manager::modifyUserAtPos(const unsigned int &i, const User &modified_user)
{
  _users[i]=modified_user;
}

void Manager::modifyCompanyAtPos (const unsigned int &i, const Company &modified_company)
{
  _companies[i]=modified_company;
}

void Manager::modifyGroupAtPos (const unsigned int &i, const Group &modified_group)
{
  _groups[i]=modified_group;
}

User Manager::getUserByPos(const unsigned int &i) const
{
  return _users[i];
}

Company Manager::getCompanyByPos(const unsigned int &i) const
{
  return _companies[i];
}

Group Manager::getGroupByPos(const unsigned int &i) const
{
  return _groups[i];
}

vector<Account> Manager::getAllAccounts() const
{
  vector<Account> all;
  
  vector<User>::const_iterator it_u=_users.begin();
  for (; it_u!=_users.end(); it_u++)
  {
    insert_sorted(all, *it_u);
  }
  
  vector<Company>::const_iterator it_c=_companies.begin();
  for (; it_c!=_companies.end(); it_c++)
  {
    insert_sorted(all, *it_c);
  }
  
  vector<Group>::const_iterator it_g=_groups.begin();
  for (; it_g!=_groups.end(); it_g++)
  {
    insert_sorted(all, *it_g);
  }
  
  return all;
}

void Manager::addDirectedRelationship(const string &ID_start, const string &ID_target, const string &relationship)
{
  _graph.setEdge(ID_start, ID_target, relationship);
}

void Manager::addUndirectedRelationship (const string &ID_start, const string &ID_target, const string &relationship)
{
  _graph.bsetEdge(ID_start, ID_target, relationship);
}

vector<string> Manager::getListConnection(const std::string &starting_ID, const std::string &relationship)
{
  return _graph.branches(starting_ID, relationship);
}

void Manager::_setNodes()
{
  vector<User>::const_iterator it_u=_users.begin();
  for (; it_u!=_users.end(); it_u++)
  {
    _graph.addNode(it_u->getID());
  }
  
  vector<Company>::const_iterator it_c=_companies.begin();
  for (; it_c!=_companies.end(); it_c++)
  {
    _graph.addNode(it_c->getID());
  }
  
  vector<Group>::const_iterator it_g=_groups.begin();
  for (; it_g!=_groups.end(); it_g++)
  {
    _graph.addNode(it_g->getID());
  }
}

void Manager::_deleteNode(const std::string &ID_to_delete)
{
  _graph.popNode(ID_to_delete);
}

size_t Manager::_findNodePos(const std::string &ID_to_find)
{
  return _graph.find(ID_to_find);
}

