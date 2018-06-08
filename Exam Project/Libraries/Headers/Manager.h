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
  template <typename AccountType>
  void addAccount (const AccountType &account_to_add);
  
  void deleteAccount (const string &ID);
  void deleteRelationship (const string &root, const string &target);
  
  void modifyUserAtPos (const unsigned int &i, const User &modified_user);
  void modifyCompanyAtPos (const unsigned int &i, const Company &modified_company);
  void modifyGroupAtPos (const unsigned int &i, const Group &modified_group);
  
  vector<Account> getAllAccounts() const;
  
  template <typename AccountType>
  AccountType getAccount (const string &ID);
  
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

template <typename AccountType>
size_t FindPosbyID (const vector<AccountType> &v, const string &ID)
{
  //Ricerchiamo l'id richiesto all'interno del vettore (opportunamente ordinato)
  
  
  size_t a = 0, b = v.size() - 1;  //Estremi di ricerca
  size_t m;
  
  while (a < b)
  {
    m = (a + b) / 2;  //Dividiamo l'intervallo a metà
    
    if (v[a].getID() == ID) {
      return a;
    }
    else if (v[b].getID() == ID) {
      return b;
    }
    
    if(v[m].getID() == ID)
    {
      //Abbiamo trovato l'elemento
      return m;
    }
    
    //L'elemento non è stato trovato
    else if(v[m].getID() < ID)
    {
      //Abbiamo scelto un elemento che si trova prima di quello ricercato. Il nuovo intervallo è la seconda metà
      a = m;
    }
    else
    {
      //L'elemento ricercato si trova prima del punto medio (v[m] > id). L'intervallo è la prima metà
      b = m;
    }
    a++;
    b--;
  }
  /*
   * Arrivati a questo punto, l'intervallo è stato suddiviso così tante volte che a = b oppure a = b-1.
   * L'elemento non era presente.
   */
  return v.size();
}

template <typename AccountType>
void Manager::addAccount(const AccountType &account_to_add)
{
  if (account_to_add.getType()==Account::user_type)
  {
    insert_sorted(_users, account_to_add);
  }
  else if (account_to_add.getType()==Account::company_type)
  {
    insert_sorted(_companies, account_to_add);
  }
  else if (account_to_add.getType()==Account::group_type)
  {
    insert_sorted(_groups, account_to_add);
  }
  else
    return;
  
  _graph.addNode(account_to_add.getID());
}

template <typename AccountType>
AccountType Manager::getAccount(const std::string &ID)
{
  size_t pos=0;
  
  pos=FindPosbyID(_users, ID);
  if (pos!=_users.size())
  {
    return _users[pos];
  }
  
  pos=FindPosbyID(_companies, ID);
  if (pos!=_companies.size())
  {
    return _companies[pos];
  }
  
  pos=FindPosbyID(_groups, ID);
  if (pos!=_groups.size())
  {
    return _groups[pos];
  }
  
  if (pos==_groups.size())
  {
    return AccountType();
  }
}
#endif /* Manager_h */
