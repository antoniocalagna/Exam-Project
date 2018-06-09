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

vector<User> Manager::getAllUsers() const
{
  return _users;
}

vector<Company> Manager::getAllCompanies() const
{
  return _companies;
}

vector<Group> Manager::getAllGroups() const
{
  return _groups;
}

User Manager::getUser(const std::string &ID) const
{
  size_t pos=FindPosbyID(_users, ID);
  if (pos!=_users.size())
  {
    return _users[pos];
  }
  else
    return User();
}

Company Manager::getCompany(const std::string &ID) const
{
  size_t pos=FindPosbyID(_companies, ID);
  if (pos!=_companies.size())
  {
    return _companies[pos];
  }
  else
    return Company();
}

Group Manager::getGroup(const std::string &ID) const
{
  size_t pos=FindPosbyID(_groups, ID);
  if (pos!=_groups.size())
  {
    return _groups[pos];
  }
  else
    return Group();
}

//addAccount polimorfica controlla che l'ID non sia già esistente e poi lo aggiunge ordinatamente nel vettore opportuno

bool Manager::addAccount(const User &account_to_add)
{
  if (!_exist_as_node(account_to_add.getID()))
    return false;
  
  insert_sorted<User, User>(_users, account_to_add);
  _graph.addNode(account_to_add.getID());
  return true;
}

bool Manager::addAccount(const Company &account_to_add)
{
  if (!_exist_as_node(account_to_add.getID()))
    return false;
  
  insert_sorted<Company, Company>(_companies, account_to_add);
  _graph.addNode(account_to_add.getID());
  return true;
}

bool Manager::addAccount(const Group &account_to_add)
{
  if (!_exist_as_node(account_to_add.getID()))
    return false;
  
  insert_sorted<Group, Group>(_groups, account_to_add);
  _graph.addNode(account_to_add.getID());
  return true;
}

void Manager::deleteAccount (const string &ID)
{
  size_t pos=0;
  
  pos=FindPosbyID(_users, ID);
  if (pos!=_users.size())
  {
    _users.erase(_users.begin()+pos);
  }
  
  pos=FindPosbyID(_companies, ID);
  if (pos!=_companies.size())
  {
    _companies.erase(_companies.begin()+pos);
  }
  
  pos=FindPosbyID(_groups, ID);
  if (pos!=_groups.size())
  {
    _groups.erase(_groups.begin()+pos);
  }
  
  if (pos==_groups.size())
  {
    return;
  }
  
  _graph.popNode(ID);
}

void Manager::deleteRelationship(const std::string &root, const std::string &target)
{
  _graph.setEdge(root, target, _graph.no_edge);
}

//replaceAccount polimorfica controlla che il nuovo ID non sia già esistente, cerca la posizione dell'ID da sostituire e procede.

bool Manager::replaceAccount(const std::string &ID_to_replace, const User &new_account)
{
  if (!_exist_as_node(new_account.getID()))
    return false;
  
  size_t pos=FindPosbyID(_users, ID_to_replace);
  if (pos!=_users.size())
  {
    _users[pos]=new_account;
    return true;
  }
  else
    return false;
}

bool Manager::replaceAccount(const std::string &ID_to_replace, const Company &new_account)
{
  if (!_exist_as_node(new_account.getID()))
    return false;
  
  size_t pos=FindPosbyID(_companies, ID_to_replace);
  if (pos!=_companies.size())
  {
    _companies[pos]=new_account;
    return true;
  }
  else
    return false;
}

bool Manager::replaceAccount(const std::string &ID_to_replace, const Group &new_account)
{
  if (!_exist_as_node(new_account.getID()))
    return false;
  
  size_t pos=FindPosbyID(_groups, ID_to_replace);
  if (pos!=_groups.size())
  {
    _groups[pos]=new_account;
    return true;
  }
  else
    return false;
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
  
  return all; //vettore binary-sorted
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

//PRIVATE METHODS

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

bool Manager::_exist_as_node(const string &ID_to_check)
{
  size_t pos=_graph.find(ID_to_check);
  if (pos!=_graph.nodesNumber())
  {
    return false; //ID already exists!
  }
  else
    return true;
}

//STATISTICS

size_t Manager::NumAccounts() const
{
  return _graph.nodesNumber();
}

size_t Manager::NumUsers() const
{
  return _users.size();
}

size_t Manager::NumCompanies() const
{
  return _companies.size();
}

size_t Manager::NumGroups() const
{
  return _groups.size();
}

size_t Manager::NumFriends(const std::string &whose_ID) const
{
  if (FindPosbyID(_users, whose_ID)==_users.size())
    return -1; //Controllo che sia un utente.
  
  return _graph.outDegree_withEdge(whose_ID, relation::friendship);
}

size_t Manager::NumRelatives(const std::string &whose_ID) const
{
  if (FindPosbyID(_users, whose_ID)==_users.size())
    return -1; //Controllo che sia un utente.
  
  size_t count=0;
  count = _graph.outDegree_withEdge(whose_ID, relation::fatherhood);
  count=count+_graph.outDegree_withEdge(whose_ID, relation::motherhood);
  return count;
}

size_t Manager::NumEmployees(const std::string &company_employer) const
{
  if (FindPosbyID(_companies, company_employer)==_companies.size())
    return -1; //Controllo che sia un'azienda.
  
  return _graph.outDegree_withEdge(company_employer, relation::employee);
}

size_t Manager::NumSubsidiaries(const std::string &company_main) const
{
  if (FindPosbyID(_companies, company_main)==_companies.size())
    return -1; //Controllo che sia un'azienda.
  
  return _graph.outDegree_withEdge(company_main, relation::co_worker);
}

size_t Manager::NumMembers(const std::string &group) const
{
  if (FindPosbyID(_groups, group)==_groups.size())
    return -1; //Controllo che sia un'azienda.
  
  return _graph.outDegree_withEdge(group, relation::membership);
}

size_t Manager::NumBornAfter(const Date &start_date) const
{
  if(!Date::CheckDate(start_date))
    return -1;
  
  int count=0;
  for (auto it=_users.begin(); it!=_users.end(); it++)
  {
    if (it->getBirth()>start_date)
      count++;
  }
  return count;
}

string Manager::MostEmployingCompany() const
{
  Company best;
  size_t num_members=0;
  for (auto it=_companies.begin(); it!=_companies.end(); it++)
  {
    if((_graph.outDegree_withEdge(it->getID(), relation::employee))>num_members)
    {
      best=*it;
    }
  }
  return best.getID();
}

string Manager::MostEmployingPartnership() const
{
  Company best;
  vector<string> subs;
  size_t num_members=0, num_prev=0;
  for (auto it=_companies.begin(); it!=_companies.end(); it++)
  {
    num_members=_graph.outDegree_withEdge(it->getID(), relation::employee);
    
    subs=_graph.branches(it->getID(), relation::co_worker);
    for (auto it2=subs.begin(); it2!=subs.end(); it++)
    {
      num_members=num_members+_graph.outDegree_withEdge(*it2, relation::employee);
    }
    
    if (num_members>num_prev)
    {
      best=*it;
    }
  }
  return best.getID();
}

string Manager::UserWithMostFriends() const
{
  User best;
  size_t num_friends=0;
  for (auto it=_users.begin(); it!=_users.end(); it++)
  {
    if ((_graph.outDegree_withEdge(it->getID(), relation::friendship))>num_friends)
    {
      best=*it;
    }
  }
  return best.getID();
}

string Manager::UserWithMostAcquaintances() const
{
  User best;
  size_t num_acquaintances=0;
  for (auto it=_users.begin(); it!=_users.end(); it++)
  {
    if ((_graph.outDegree_withEdge(it->getID(), relation::knowings))>num_acquaintances)
    {
      best=*it;
    }
  }
  return best.getID();
}

float Manager::UsersMiddleAge() const
{
  float sum=0;
  Date now = Date::Now();
  for (auto it=_users.begin(); it!=_users.end(); it++)
  {
    Date tmp = it->getBirth();
    sum=sum+tmp.yearsFrom(now);
  }
  return (float)sum/(_users.size());
}
