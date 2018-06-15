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
  _setKeys(users);
  _setKeys(companies);
  _setKeys(groups);
  _setNodes();
}

void Manager::setUsers(const vector<User> &users_to_set)
{
  _setKeys(users_to_set);
  _setNodes();
}

void Manager::setCompanies(const vector<Company> &companies_to_set)
{
  _setKeys(companies_to_set);
  _setNodes();
}

void Manager::setGroups(const vector<Group> &groups_to_set)
{
  _setKeys(groups_to_set);
  _setNodes();
}

void Manager::setPosts(const vector<Post> &all_posts_of_account, const std::string &whose_ID)
{
  _map_posts[whose_ID]=all_posts_of_account;
}

vector<User> Manager::getAllUsers() const
{
  vector<User> users;
  for (auto it=_map_users.begin(); it!=_map_users.end(); it++)
  {
    users.push_back(it->second);
  }
  return users;
}

vector<Company> Manager::getAllCompanies() const
{
  vector<Company> companies;
  for (auto it=_map_companies.begin(); it!=_map_companies.end(); it++)
  {
    companies.push_back(it->second);
  }
  return companies;
}

vector<Group> Manager::getAllGroups() const
{
  vector<Group> groups;
  for (auto it=_map_groups.begin(); it!=_map_groups.end(); it++)
  {
    groups.push_back(it->second);
  }
  return groups;
}

User Manager::getUser(const std::string &ID) const
{
  size_t count = _map_users.count(ID);
  if (count!=0)
  {
    return _map_users.at(ID);
  }
  else
    return User();
}

Company Manager::getCompany(const std::string &ID) const
{
  size_t count = _map_companies.count(ID);
  if (count!=0)
  {
    return _map_companies.at(ID);
  }
  else
    return Company();
}

Group Manager::getGroup(const std::string &ID) const
{
  size_t count = _map_groups.count(ID);
  if (count!=0)
  {
    return _map_groups.at(ID);
  }
  else
    return Group();
}

pair<string,vector<Post>> Manager::getPosts(const std::string &ID) const
{
  return pair<string,vector<Post>>(ID, _map_posts.at(ID));
}

//addAccount polimorfica controlla che l'ID non sia già esistente e poi lo aggiunge ordinatamente nel vettore opportuno

bool Manager::addAccount(const User &account_to_add)
{
  if (_exist_as_node(account_to_add.getID()))
    return false;
  
  _map_users[account_to_add.getID()]=account_to_add;
  _graph.addNode(account_to_add.getID());
  _map_posts[account_to_add.getID()]=vector<Post>();
  return true;
}

bool Manager::addAccount(const Company &account_to_add)
{
  if (_exist_as_node(account_to_add.getID()))
    return false;
  
  _map_companies[account_to_add.getID()]=account_to_add;
  _graph.addNode(account_to_add.getID());
  _map_posts[account_to_add.getID()]=vector<Post>();
  return true;
}

bool Manager::addAccount(const Group &account_to_add)
{
  if (_exist_as_node(account_to_add.getID()))
    return false;
  
  _map_groups[account_to_add.getID()]=account_to_add;
  _graph.addNode(account_to_add.getID());
  _map_posts[account_to_add.getID()]=vector<Post>();
  return true;
}

bool Manager::deleteAccount (const string &ID)
{
  size_t count=0;
  
  count=_map_users.count(ID);
  if (count!=0)
  {
    _map_users.erase(ID);
  }
  
  count=_map_companies.count(ID);
  if (count!=0)
  {
    _map_companies.erase(ID);
  }
  
  count=_map_groups.count(ID);
  if (count!=0)
  {
    _map_groups.erase(ID);
  }
  
  if (count==0)
  {
    return false;
  }
  
  _graph.popNode(ID);
  _map_posts.erase(ID);
  return true;
}

void Manager::deleteRelationship(const std::string &root, const std::string &target)
{
  _graph.setEdge(root, target, _graph.no_edge);
}

//replaceAccount polimorfica controlla che il nuovo ID non sia già esistente, cerca la posizione dell'ID da sostituire e procede.

bool Manager::replaceAccount(const std::string &ID_to_replace, const User &new_account)
{
  if (_exist_as_node(new_account.getID()))
    return false;
  
  size_t count=_map_users.count(ID_to_replace);
  if (count!=0)
  {
    _map_users[ID_to_replace]=new_account;
    _graph.editNode(ID_to_replace, new_account.getID());
    _map_posts.erase(ID_to_replace);
    _map_posts[new_account.getID()]=vector<Post>();
    return true;
  }
  else
    return false;
}

bool Manager::replaceAccount(const std::string &ID_to_replace, const Company &new_account)
{
  if (_exist_as_node(new_account.getID()))
    return false;
  
  size_t count=_map_companies.count(ID_to_replace);
  if (count!=0)
  {
    _map_companies[ID_to_replace]=new_account;
    _graph.editNode(ID_to_replace, new_account.getID());
    _map_posts.erase(ID_to_replace);
    _map_posts[new_account.getID()]=vector<Post>();
    return true;
  }
  else
    return false;
}

bool Manager::replaceAccount(const std::string &ID_to_replace, const Group &new_account)
{
  if (_exist_as_node(new_account.getID()))
    return false;
  
  size_t count=_map_users.count(ID_to_replace);
  if (count!=0)
  {
    _map_groups[ID_to_replace]=new_account;
    _graph.editNode(ID_to_replace, new_account.getID());
    _map_posts.erase(ID_to_replace);
    _map_posts[new_account.getID()]=vector<Post>();
    return true;
  }
  else
    return false;
}
vector<Account> Manager::getAllAccounts() const
{
  vector<Account> all;
  
  for (auto it_u=_map_users.begin(); it_u!=_map_users.end(); it_u++)
  {
    insert_sorted(all, it_u->second);
  }
  
  for (auto it_c=_map_companies.begin(); it_c!=_map_companies.end(); it_c++)
  {
    insert_sorted(all, it_c->second);
  }
  
  for (auto it_g=_map_groups.begin(); it_g!=_map_groups.end(); it_g++)
  {
    insert_sorted(all, it_g->second);
  }
  
  return all; //vettore binary-sorted
}

bool Manager::addDirectedRelationship(const string &ID_start, const string &ID_target, const string &relationship)
{
  if(!relation::belong(relationship))
    return false;
  if((!_exist_as_node(ID_start))||(!_exist_as_node(ID_target)))
    return false;
  if((relationship==relation::parent)&&(_checkAge(ID_start, ID_target)))
    return false;
  if((relationship==relation::born)&&(_checkAge(ID_target, ID_start)))
    return false;
  
  if ((relationship==relation::friendship)||(relationship==relation::knowings)||(relationship==relation::partner))
    _graph.bsetEdge(ID_start,ID_target,relationship);
  else if (relationship==relation::parent)
    _graph.setEdge(ID_target, ID_start, relation::born);
  else if (relationship==relation::employee)
    _graph.setEdge(ID_target, ID_start, relation::employer);
  
  _graph.setEdge(ID_start, ID_target, relationship);

  return true;
}

bool Manager::addUndirectedRelationship (const string &ID_start, const string &ID_target, const string &relationship)
{
  if(!relation::belong(relationship))
    return false;
  if((!_exist_as_node(ID_start))||(!_exist_as_node(ID_target)))
    return false;
  if ((relationship==relation::parent)||(relationship==relation::born)||(relationship==relation::employee)||(relationship==relation::employer)||(relationship==relation::membership))
    return false;
  
  _graph.bsetEdge(ID_start, ID_target, relationship);
  return true;
}

vector<string> Manager::getListConnection(const std::string &starting_ID, const std::string &relationship)
{
  if(!relation::belong(relationship))
    return vector<string>();
  return _graph.branches(starting_ID, relationship);
}

bool Manager::addPost(const Post &post_to_add, const std::string &whose_ID)
{
  if (_map_posts.count(whose_ID)==0)
    return false;
  _map_posts.at(whose_ID).push_back(post_to_add);
  return true;
}

bool Manager::deletePost(const Post &post_to_delete, const std::string &whose_ID)
{
  if (_map_posts.count(whose_ID)==0)
    return false;
  vector<Post>::iterator it=find(_map_posts.at(whose_ID).begin(), _map_posts.at(whose_ID).end(), post_to_delete);
  _map_posts.at(whose_ID).erase(it);
  return true;
}

bool Manager::addLike_Dislike(const bool &like_1_dislike_0, const Post &post_liked, const std::string &ID)
{
  if (_map_posts.count(ID)==0)
    return false;
  vector<Post>::iterator it=find(_map_posts.at(ID).begin(), _map_posts.at(ID).end(), post_liked);
  if (like_1_dislike_0==true)
  {
    it->AddLike(ID);
    return true;
  }
  if (like_1_dislike_0==false)
  {
    it->AddDislike(ID);
    return true;
  }
  return false;
}

//PRIVATE METHODS

void Manager::_setNodes()
{
  for (auto it_u=_map_users.begin(); it_u!=_map_users.end(); it_u++)
  {
    _graph.addNode(it_u->second.getID());
  }
  
  for (auto it_c=_map_companies.begin(); it_c!=_map_companies.end(); it_c++)
  {
    _graph.addNode(it_c->second.getID());
  }
  
  for (auto it_g=_map_groups.begin(); it_g!=_map_groups.end(); it_g++)
  {
    _graph.addNode(it_g->second.getID());
  }
}

bool Manager::_exist_as_node(const string &ID_to_check)
{
  size_t pos=_graph.find(ID_to_check);
  if (pos!=_graph.nodesNumber())
  {
    return true; //ID already exists!
  }
  else
    return false;
}

void Manager::_setKeys(const vector<User> &users)
{
  for (auto it=users.begin(); it!=users.end(); it++)
  {
    _map_users[it->getID()]=*it;
    if (_map_posts.count(it->getID())==0)
      _map_posts[it->getID()]=vector<Post>();
  }
}

void Manager::_setKeys(const vector<Company> &companies)
{
  for (auto it=companies.begin(); it!=companies.end(); it++)
  {
    _map_companies[it->getID()]=*it;
    if (_map_posts.count(it->getID())==0)
      _map_posts[it->getID()]=vector<Post>();
  }
}

void Manager::_setKeys(const vector<Group> &groups)
{
  for (auto it=groups.begin(); it!=groups.end(); it++)
  {
    _map_groups[it->getID()]=*it;
    if (_map_posts.count(it->getID())==0)
      _map_posts[it->getID()]=vector<Post>();
  }
}

bool Manager::_checkAge(const std::string &ID_old, const std::string &ID_young) const
{
  return _map_users.at(ID_old).getBirth()<_map_users.at(ID_young).getBirth();
}

//STATISTICS

size_t Manager::NumAccounts() const
{
  return _graph.nodesNumber();
}

size_t Manager::NumUsers() const
{
  return _map_users.size();
}

size_t Manager::NumCompanies() const
{
  return _map_companies.size();
}

size_t Manager::NumGroups() const
{
  return _map_groups.size();
}

size_t Manager::NumFriends(const std::string &whose_ID) const
{
  if (_map_users.count(whose_ID)==0)
    return -1; //Controllo che sia un utente.
  
  return _graph.outDegree_withEdge(whose_ID, relation::friendship);
}

size_t Manager::NumRelatives(const std::string &whose_ID) const
{
  if (_map_users.count(whose_ID)==0)
    return -1; //Controllo che sia un utente.
  
  size_t count=0;
  count = _graph.outDegree_withEdge(whose_ID, relation::parent);
  return count;
}

size_t Manager::NumEmployees(const std::string &company_employer) const
{
  if (_map_companies.count(company_employer)==0)
    return -1; //Controllo che sia un'azienda.
  
  return _graph.outDegree_withEdge(company_employer, relation::employee);
}

size_t Manager::NumSubsidiaries(const std::string &company_main) const
{
  if (_map_companies.count(company_main)==0)
    return -1; //Controllo che sia un'azienda.
  
  return _graph.outDegree_withEdge(company_main, relation::co_worker);
}

size_t Manager::NumMembers(const std::string &group) const
{
  if (_map_groups.count(group)==0)
    return -1; //Controllo che sia un'azienda.
  
  return _graph.outDegree_withEdge(group, relation::membership);
}

size_t Manager::NumBornAfter(const Date &start_date) const
{
  if(!Date::CheckDate(start_date))
    return -1;
  
  int count=0;
  for (auto it=_map_users.begin(); it!=_map_users.end(); it++)
  {
    if (it->second.getBirth()>start_date)
      count++;
  }
  return count;
}

pair<string, Company> Manager::MostEmployingCompany() const
{
  Company best;
  size_t num_members=0;
  for (auto it=_map_companies.begin(); it!=_map_companies.end(); it++)
  {
    if((_graph.outDegree_withEdge(it->second.getID(), relation::employee))>num_members)
    {
      best=it->second;
    }
  }
  return pair<string, Company>(best.getID(), best);
}

pair<string, Company> Manager::MostEmployingPartnership() const
{
  Company best;
  vector<string> subs;
  size_t num_members=0, num_prev=0;
  for (auto it=_map_companies.begin(); it!=_map_companies.end(); it++)
  {
    num_members=_graph.outDegree_withEdge(it->second.getID(), relation::employee);
    
    subs=_graph.branches(it->second.getID(), relation::co_worker);
    for (auto it2=subs.begin(); it2!=subs.end(); it++)
    {
      num_members=num_members+_graph.outDegree_withEdge(*it2, relation::employee);
    }
    
    if (num_members>num_prev)
    {
      best=it->second;
    }
  }
  return pair<string, Company>(best.getID(), best);
}

pair<string, User> Manager::UserWithMostFriends() const
{
  User best;
  size_t num_friends=0;
  for (auto it=_map_users.begin(); it!=_map_users.end(); it++)
  {
    if ((_graph.outDegree_withEdge(it->second.getID(), relation::friendship))>num_friends)
    {
      best=it->second;
    }
  }
  return pair<string, User>(best.getID(), best);
}

pair<string, User> Manager::UserWithMostAcquaintances() const
{
  User best;
  size_t num_acquaintances=0;
  for (auto it=_map_users.begin(); it!=_map_users.end(); it++)
  {
    if ((_graph.outDegree_withEdge(it->second.getID(), relation::knowings))>num_acquaintances)
    {
      best=it->second;
    }
  }
  return pair<string, User>(best.getID(), best);
}

float Manager::UsersAverageAge() const
{
  float sum=0;
  Date now = Date::Now();
  for (auto it=_map_users.begin(); it!=_map_users.end(); it++)
  {
    Date tmp = it->second.getBirth();
    sum=sum+tmp.yearsFrom(now);
  }
  return (float)sum/(_map_users.size());
}

pair<string, Post> Manager::MostLikedPost() const
{
  vector<string> all_ids=_graph.nodesVector();
  vector<Post> tmp;
  Post ext_best;
  string id_best;
  
  for (auto it=all_ids.begin(); it!=all_ids.end(); it++)
  {
    tmp=_map_posts.at(*it);
    if (tmp.size()!=0)
    {
      vector<Post>::iterator it2 = tmp.begin();
      Post best = *it2;
      for (; it2!=tmp.end(); it2++)
      {
        if (*it2>best)
          best=*it2;
      }
    
    if(best>ext_best)
    {
      ext_best=best;
      id_best=*it;
    }
    }
  }
  return pair<string, Post>(id_best,ext_best);
}

pair<string, Post> Manager::MostDislikedPost() const
{
  vector<string> all_ids=_graph.nodesVector();
  vector<Post> tmp;
  Post ext_best;
  string id_best;
  for (auto it=all_ids.begin(); it!=all_ids.end(); it++)
  {
    tmp=_map_posts.at(*it);
    vector<Post>::iterator it2 = tmp.begin();
    Post best = *it2;
    
    for (; it2!=tmp.end(); it2++)
    {
      if (it2->NumDislikes()>best.NumDislikes())
        best=*it2;
    }
    
    if(best>ext_best)
    {
      ext_best=best;
      id_best=*it;
    }
  }
  return pair<string, Post>(id_best,ext_best);
}

string Manager::MostLiked_DislikedAccount(const bool &like_1_dislike_0) const
{
  vector<string> all_ids=_graph.nodesVector();
  vector<Post> tmp;
  string best_ID;
  int likes=0, best_likes=0;
  for (auto it=all_ids.begin(); it!=all_ids.end(); it++)
  {
    tmp=_map_posts.at(*it);
    vector<Post>::iterator it2 = tmp.begin();
    for (; it2!=tmp.end(); it2++)
    {
      if (like_1_dislike_0==true)
        likes=likes+it2->NumLikes();
      if (like_1_dislike_0==false)
        likes=likes+it2->NumDislikes();
    }
   if (likes>best_likes)
   {
     best_ID=*it;
     best_likes=likes;
   }
  }
  return best_ID;
}

pair<string, Post> Manager::RatioReactionPost(const bool &best_1_worst_0) const
{
  vector<string> all_ids=_graph.nodesVector();
  vector<Post> tmp;
  Post ext_best;
  string best_ID;
  
  for (auto it=all_ids.begin(); it!=all_ids.end(); it++)
  {
    tmp=_map_posts.at(*it);
    vector<Post>::iterator it2 = tmp.begin();
    Post best = *it2;
    for (; it2!=tmp.end(); it2++)
    {
      if (best_1_worst_0==true)
      {
        if (it2->RatioReaction()>best.RatioReaction())
          best=*it2;
      }
      if (best_1_worst_0==false)
      {
        if (it2->RatioReaction()<best.RatioReaction())
          best=*it2;
      }
    }
    if (best_1_worst_0==true)
    {
      if(best>ext_best)
      {
        ext_best=best;
        best_ID=*it;
      }
    }
    if (best_1_worst_0==false)
    {
      if(best<ext_best)
      {
        ext_best=best;
        best_ID=*it;
      }
    }
  }
  return pair<string, Post>(best_ID, ext_best);
}

string Manager::RatioReactionAccount(const bool &best_1_worst_0) const
{
  vector<string> all_ids=_graph.nodesVector();
  vector<Post> tmp;
  string best_ID;
  float ratio=0, best_ratio=0;
  for (auto it=all_ids.begin(); it!=all_ids.end(); it++)
  {
    tmp=_map_posts.at(*it);
    vector<Post>::iterator it2 = tmp.begin();
    for (; it2!=tmp.end(); it2++)
    {
      ratio=ratio+it2->RatioReaction();
    }
    ratio=ratio/tmp.size();
    if (best_1_worst_0==true)
    {
      if (ratio>best_ratio)
      {
        best_ID=*it;
        best_ratio=ratio;
      }
    }
    if (best_1_worst_0==false)
    {
      if (ratio<best_ratio)
      {
        best_ID=*it;
        best_ratio=ratio;
      }
    }
  }
  return best_ID;
}

unordered_set<string> Manager::LonerPeople(const unsigned int &relations, const unsigned int &memberships, const bool &employed, const unsigned int &newsreactions)
{
  unordered_set<string> set;
  bool isLoner = true;
  bool isValid = false;
  
  if((relations!=0)||(memberships!=0)||(employed!=false)||(newsreactions!=0))
    isValid=true;
  else
    return set;
  
  for (auto it=_map_users.begin(); it!=_map_users.end(); it++)
  {
    if (memberships!=0)
    {
      if(_graph.outDegree_withEdge(it->first, relation::membership)>memberships)
        isLoner=false;
    }
    
    if ((employed!=false)&&(isLoner==true))
    {
      if (_graph.outDegree_withEdge(it->first, relation::employee)!=0)
        isLoner=false;
    }
    
    if ((relations!=0)&&(isLoner==true))
    {
      int count_relations=0;
      count_relations=(int)_graph.outDegree_withEdge(it->first, relation::friendship);
      count_relations=count_relations+(int)_graph.outDegree_withEdge(it->first, relation::knowings);
      count_relations=count_relations+(int)_graph.outDegree_withEdge(it->first, relation::parent);
      count_relations=count_relations+(int)_graph.outDegree_withEdge(it->first, relation::born);
      count_relations=count_relations+(int)_graph.outDegree_withEdge(it->first, relation::partner);
      
      if (count_relations>relations)
        isLoner=false;
    }
    
    if(isLoner==true)
      set.insert(it->second.getID());
    else
      isLoner=true; //per ricominciare il ciclo correttamente
  }
  
  if (newsreactions!=0)
  {
    for (auto it=_map_posts.begin(); it!=_map_posts.end(); it++)
    {
      int count_reactions=0;
      
      if (_map_users.count(it->first)!=0)
      {
        for (auto it_post = it->second.begin(); it_post!=it->second.end(); it_post++)
        {
          if (it_post->SearchLike(it->first)!=-1)
            count_reactions++;
          if (it_post->SearchDislike(it->first)!=-1)
            count_reactions++;
        }
        if (count_reactions<newsreactions)
        {
          if (set.count(it->first)==0)
            set.insert(it->first);
        }
        else
        {
          if (set.count(it->first)!=0)
            set.erase(it->first);
        }
      }
    }
  }
  
  return set;
} 
