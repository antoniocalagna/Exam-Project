//
//  Manager.hpp
//  Social Network
//
//  Created by Antonio Calagna on 02/06/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#ifndef Manager_h
#define Manager_h

#include <map>
#include <unordered_map>
#include "User.h"
#include "Company.h"
#include "Group.h"
#include "Graph.hpp"
#include "Post.h"
#include "TemplateFunctions.hpp"

#define MAX 6 //Gender's array.

//NAMESPACES

namespace relation
{
  const string friendship = "friend";
  const string knowings = "acquaintances";
  
  const string parent = "mother/father";
  const string born = "daughter/son";
  
  const string partner = "partner";
  
  const string employer = "boss";
  const string employee = "hired";
  const string co_worker = "subsidiaries";
  
  const string membership = "member";
  
  bool belong (const string &r);
}

namespace gender
{
  const char genders[MAX] = {'M', 'F', 'N', 'A', 'B', 'O'};
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
  void setPosts (const vector<Post> &all_posts_of_account, const string &whose_ID);
  
  //Getters
  vector<User> getAllUsers() const;
  vector<Company> getAllCompanies() const;
  vector<Group> getAllGroups() const;
  
  User getUser (const string &ID) const;
  Company getCompany (const string &ID) const;
  Group getGroup (const string &ID) const;
  pair<string,vector<Post>> getPosts (const string &ID) const;
  
  //Management Functions
  bool addAccount(const User &account_to_add);
  bool addAccount(const Company &account_to_add);
  bool addAccount(const Group &account_to_add);
  
  bool deleteAccount (const string &ID);
  void deleteRelationship (const string &root, const string &target);
  
  bool replaceAccount (const string &ID_to_replace, const User &new_account);
  bool replaceAccount (const string &ID_to_replace, const Company &new_account);
  bool replaceAccount (const string &ID_to_replace, const Group &new_account);
  
  vector<Account> getAllAccounts() const;
  
  bool addDirectedRelationship (const string &ID_start, const string &ID_target, const string &relationship);
  
  bool addUndirectedRelationship (const string &ID_start, const string &ID_target, const string &relationship);
  
  vector<string> getListConnection (const string &starting_ID, const string &relationship);
  
  bool addPost (const Post &post_to_add, const string &whose_ID);
  bool deletePost (const Post &post_to_delete, const string &whose_ID);
  bool addLike_Dislike (const bool &like_1_dislike_0, const Post &post_liked, const string &ID);
  
  //Statistics Functions
  
  size_t NumAccounts() const;
  size_t NumUsers() const;
  size_t NumCompanies() const;
  size_t NumGroups() const;
  
  size_t NumFriends (const string &whose_ID) const;
  size_t NumRelatives (const string &whose_ID) const;
  size_t NumEmployees (const string &company_employer) const;
  size_t NumSubsidiaries (const string &company_main) const;
  size_t NumMembers (const string &group) const;
  size_t NumBornAfter (const Date &start_date) const;
  
  pair<string, Company> MostEmployingCompany() const;
  pair<string, Company> MostEmployingPartnership() const;
  pair<string, User> UserWithMostFriends() const;
  pair<string, User> UserWithMostAcquaintances() const;
  
  float UsersAverageAge() const;
  
  pair<string, Post> MostLikedPost() const;
  pair<string, Post> MostDislikedPost() const;
  string MostLiked_DislikedAccount(const bool &like_1_dislike_0) const;
  pair<string, Post> RatioReactionPost(const bool &best_1_worst_0) const;
  string RatioReactionAccount(const bool &best_1_worst_0) const;
  
  unordered_set<string> LonerPeople(const unsigned int &relations, const unsigned int &memberships, const bool &employed, const unsigned int &newsreactions);
  
private:
  unordered_map<string, User> _map_users;
  unordered_map<string, Company> _map_companies;
  unordered_map<string, Group> _map_groups;
  Graph<string, string> _graph;
  unordered_map<string, vector<Post>> _map_posts;
  
  void _setNodes();
  bool _exist_as_node(const string &ID_to_check);
  
  void _setKeys(const vector<User> &users);
  void _setKeys(const vector<Company> &companies);
  void _setKeys(const vector<Group> &groups);
  
  bool _checkAge(const string &ID_old, const string &ID_young) const;
  
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
    if(v[m].getID() == ID) {
      //Abbiamo trovato l'elemento
      return m;
    }
    //L'elemento non è stato trovato
    else if(v[m].getID() < ID) {
      //Abbiamo scelto un elemento che si trova prima di quello ricercato. Il nuovo intervallo è la seconda metà
      a = m;
    }
    else {
      //L'elemento ricercato si trova prima del punto medio (v[m] > id). L'intervallo è la prima metà
      b = m;
    }
    
    a++;
    b--;
  }
  /*
   * Arrivati a questo punto, l'intervallo è stato suddiviso così tante volte che a >= b.
   * L'elemento non era presente.
   */
  return v.size();
}


#endif /* Manager_h */ 
