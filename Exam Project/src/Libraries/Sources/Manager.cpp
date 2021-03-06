//
//  Manager.cpp
//  Social Network
//
//  Created by Antonio Calagna on 02/06/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include "Manager.h"

Manager::Manager(const vector<User> &users, const vector<Company> &companies, const vector<Group> &groups) {
  _setKeys(users);
  _setKeys(companies);
  _setKeys(groups);
  _setNodes();
}

void Manager::setUsers(const vector<User> &users_to_set) {
  _setKeys(users_to_set);
  _setNodes();
}

void Manager::setCompanies(const vector<Company> &companies_to_set) {
  _setKeys(companies_to_set);
  _setNodes();
}

void Manager::setGroups(const vector<Group> &groups_to_set) {
  _setKeys(groups_to_set);
  _setNodes();
}

void Manager::setPosts(const vector<Post> &all_posts_of_account, const std::string &whose_ID) {
  if (!_exist_as_node(whose_ID))
    return;

  for (auto it = all_posts_of_account.begin(); it != all_posts_of_account.end(); it++) {
    addPost(*it, whose_ID);
  }
}

vector<User> Manager::getAllUsers() const {
  vector<User> users;
  for (auto it = _map_users.begin(); it != _map_users.end(); it++) {
    users.push_back(it->second);
  }
  return users;
}

vector<Company> Manager::getAllCompanies() const {
  vector<Company> companies;
  for (auto it = _map_companies.begin(); it != _map_companies.end(); it++) {
    companies.push_back(it->second);
  }
  return companies;
}

vector<Group> Manager::getAllGroups() const {
  vector<Group> groups;
  for (auto it = _map_groups.begin(); it != _map_groups.end(); it++) {
    groups.push_back(it->second);
  }
  return groups;
}

vector<Account> Manager::getAllAccountsSorted() const {
  vector<Account> all;
  
  for (auto it_u = _map_users.begin(); it_u != _map_users.end(); it_u++) {
    all.insert (upper_bound(all.begin(), all.end(), it_u->second), it_u->second);
  }
  
  for (auto it_c = _map_companies.begin(); it_c != _map_companies.end(); it_c++) {
    all.insert (upper_bound(all.begin(), all.end(), it_c->second), it_c->second);
  }
  
  for (auto it_g = _map_groups.begin(); it_g != _map_groups.end(); it_g++) {
    all.insert (upper_bound(all.begin(), all.end(), it_g->second), it_g->second);
  }
  
  return all; //vettore binary-sorted
}

User Manager::getUser(const std::string &ID) const {
  size_t count = _map_users.count(ID);
  if (count != 0) {
    return _map_users.at(ID);
  } else
    return User();
}

Company Manager::getCompany(const std::string &ID) const {
  size_t count = _map_companies.count(ID);
  if (count != 0) {
    return _map_companies.at(ID);
  } else
    return Company();
}

Group Manager::getGroup(const std::string &ID) const {
  size_t count = _map_groups.count(ID);
  if (count != 0) {
    return _map_groups.at(ID);
  } else
    return Group();
}

vector<Post> Manager::getPosts(const std::string &ID) const {
  vector<Post> output;
  if (_map_posts.count(ID)) {
    output = _map_posts.at(ID);
  }
  return output;
}

vector<Post> Manager::getAllPosts() const {
  vector<Post> all_posts;
  for (auto it_map = _map_posts.begin(); it_map != _map_posts.end(); it_map++) {
    for (auto it_vect = it_map->second.begin(); it_vect != it_map->second.end(); it_vect++) {
      all_posts.push_back(*it_vect);
    }
  }
  return all_posts;
}

vector<pair<string, vector<Post>>> Manager::getPostsReactedBy(const string &ID) const {
  vector<pair<string, vector<Post>>> reacted_posts;
  pair<string, vector<Post>> tmp_post;

  for (auto it_map = _map_posts.begin(); it_map != _map_posts.end(); it_map++) {
    for (auto it_vect = it_map->second.begin(); it_vect != it_map->second.end(); it_vect++) {

      if (it_vect->ReactionExists(ID)) {
        tmp_post.first = it_map->first; //Definisco la coppia: ID del proprietario
        tmp_post.second.push_back(*it_vect); //Inserisco il post individuato nel vettore della coppia
        
        reacted_posts.push_back(tmp_post); //Inserisco la coppia nel vettore di coppie da ritornare
        
        tmp_post.second.clear(); //Pulisco la coppia prima di controllare i post di un nuovo account
        tmp_post.first.clear();
      }
    }
  }

  return reacted_posts;
}

vector<pair<string, vector<Post>>> Manager::getPostsWithoutReactionsOf(const string &ID) const {
  vector<pair<string, vector<Post>>> reacted_posts = getPostsReactedBy(ID);

  for (auto it_vec1 = reacted_posts.begin(); it_vec1 != reacted_posts.end(); it_vec1++) {
    for (auto it_vec2 = it_vec1->second.begin(); it_vec2 != it_vec1->second.end(); it_vec2++) {
      it_vec2->RemoveReaction(ID);
    }
  }

  return reacted_posts;
}

string Manager::getRelationship(const std::string &starting_ID, const std::string &target_ID) const {
  return _graph.edge(starting_ID, target_ID);
}

vector<string> Manager::getRelated(const string &ID, const string &relation) const {
  if (!relation::isValid(relation))
    return vector<string>();
  
  return _graph.branches(ID, relation);
}

vector<pair<pair<string, string>, string>> Manager::getAllRelationships(const string &ID) const {
  vector<pair<pair<string, string>, string>> relations;
  vector<pair<string, string>> outwardsBranches = _graph.outwardsBranches(ID);
  
  for (auto it_in = outwardsBranches.begin(); it_in != outwardsBranches.end(); it_in++) {
    relations.push_back(make_pair(make_pair(ID, it_in->first), it_in->second));
  }
  
  return relations;
}

vector<string> Manager::getUsersIDs() const {
  vector<string> IDs;

  for (auto it = _map_users.begin(); it != _map_users.end(); it++) {
    IDs.push_back(it->first);
  }

  return IDs;
}

vector<string> Manager::getCompaniesIDs() const {
  vector<string> IDs;

  for (auto it = _map_companies.begin(); it != _map_companies.end(); it++) {
    IDs.push_back(it->first);
  }

  return IDs;
}

vector<string> Manager::getGroupsIDs() const {
  vector<string> IDs;

  for (auto it = _map_groups.begin(); it != _map_groups.end(); it++) {
    IDs.push_back(it->first);
  }

  return IDs;
}

char Manager::getAccountType(const std::string &ID) const {
  if (_map_users.count(ID) == 1) //L'ID è un utente
    return Account::user_type;
  else if (_map_groups.count(ID) == 1)
    return Account::group_type;
  else if (_map_companies.count(ID) == 1)
    return Account::company_type;
  return 0;
}

bool Manager::accountExists(const std::string &ID) const {
  return _exist_as_node(ID);
}

//addAccount polimorfica controlla che l'ID non sia già esistente e poi lo aggiunge ordinatamente nel vettore opportuno

bool Manager::addAccount(const User &account_to_add) {
  if (_exist_as_node(account_to_add.getID()))
    return false;

  _map_users[account_to_add.getID()] = account_to_add;
  _graph.addNode(account_to_add.getID());
  _map_posts[account_to_add.getID()] = vector<Post>();
  return true;
}

bool Manager::addAccount(const Company &account_to_add) {
  if (_exist_as_node(account_to_add.getID()))
    return false;

  _map_companies[account_to_add.getID()] = account_to_add;
  _graph.addNode(account_to_add.getID());
  _map_posts[account_to_add.getID()] = vector<Post>();
  return true;
}

bool Manager::addAccount(const Group &account_to_add) {
  if (_exist_as_node(account_to_add.getID()))
    return false;

  _map_groups[account_to_add.getID()] = account_to_add;
  _graph.addNode(account_to_add.getID());
  _map_posts[account_to_add.getID()] = vector<Post>();
  return true;
}

bool Manager::deleteAccount(const string &ID) {
  size_t count = 0;

  count = _map_users.count(ID) + _map_groups.count(ID) + _map_companies.count(ID);
  if (count == 0) {
    //L'account non esiste
    return false;
  }

  char type = getAccountType(ID);
  if (type==Account::user_type)
    _map_users.erase(ID);
  if (type==Account::company_type)
    _map_companies.erase(ID);
  if (type==Account::group_type)
    _map_groups.erase(ID);
  
  _graph.popNode(ID);
  _map_posts.erase(ID);

  if (type == Account::user_type) {
    //Elimino da tutti i post eventuali interazioni effettuate dall'account da cancellare
    for (auto it_map = _map_posts.begin(); it_map != _map_posts.end(); it_map++) {
      for (auto it_vect = it_map->second.begin(); it_vect != it_map->second.end(); it_vect++) {
        it_vect->RemoveLike(ID);
        it_vect->RemoveDislike(ID);
      }
    }
  }

  return true;
}

//replaceAccount polimorfica controlla che il nuovo ID non sia già esistente, cerca la posizione dell'ID da sostituire e procede.

bool Manager::replaceAccount(const std::string &ID_to_replace, const User &new_account) {
  if (!_exist_as_node(new_account.getID()))
    return false;

  if (ID_to_replace == new_account.getID()) {
    _map_users[ID_to_replace] = new_account;
    return true;
  }

  size_t count = _map_users.count(ID_to_replace);
  if (count != 0) {
    deleteAccount(ID_to_replace);
    _map_users[new_account.getID()] = new_account;
    _graph.addNode(new_account.getID());
    _map_posts[new_account.getID()] = vector<Post>();
    return true;
  }
  else
    return false;
}

bool Manager::replaceAccount(const std::string &ID_to_replace, const Company &new_account) {
  if (!_exist_as_node(new_account.getID()))
    return false;

  if (ID_to_replace == new_account.getID()) {
    _map_companies[ID_to_replace] = new_account;
    return true;
  }

  size_t count = _map_companies.count(ID_to_replace);
  if (count != 0) {
    deleteAccount(ID_to_replace);
    _map_companies[new_account.getID()] = new_account;
    _graph.addNode(new_account.getID());
    _map_posts[new_account.getID()] = vector<Post>();
    return true;
  }
  else
    return false;
}

bool Manager::replaceAccount(const std::string &ID_to_replace, const Group &new_account) {
  if (!_exist_as_node(new_account.getID()))
    return false;

  if (ID_to_replace == new_account.getID()) {
    _map_groups[ID_to_replace] = new_account;
    return true;
  }

  size_t count = _map_users.count(ID_to_replace);
  if (count != 0) {
    deleteAccount(ID_to_replace);
    _map_groups[new_account.getID()] = new_account;
    _graph.addNode(new_account.getID());
    _map_posts[new_account.getID()] = vector<Post>();
    return true;
  }
  else
    return false;
}


int Manager::addRelationship(const string &ID_start, const string &ID_target, const string &relationship) {
  if (!_exist_as_node(ID_start)) //Controllo che gli ID esistano.
    return -1;
  if (!_exist_as_node(ID_target))
    return -2;
  if (!relation::isValid(relationship))
    return -3; //Controllo che la relazione esista.

  //Controllo che l'età dei due utenti non entri in contraddizione con una parentela.
  if ((relationship == relation::parent) && (!_checkAge(ID_start, ID_target)))
    return -4;
  if ((relationship == relation::born) && (!_checkAge(ID_target, ID_start)))
    return -4;
  
  //Controllo che la relazione avvenga tra tipologie di account appropriate
  
  if (!relation::isCoherent(relationship, getAccountType(ID_start), getAccountType(ID_target)))
    return -5;
  
  //Le relazioni mutue richiedono un arco bi-direzionato
  if ((relationship == relation::friendship) || (relationship == relation::partner) ||
      (relationship == relation::membership) || (relationship == relation::partnership))
    _graph.bsetEdge(ID_start, ID_target, relationship);

    //Per ogni relazione non mutua fisso anche la complementare inversa.
  else if (relationship == relation::parent)
    _graph.setEdge(ID_target, ID_start, relation::born);
  else if (relationship == relation::born)
    _graph.setEdge(ID_target, ID_start, relation::parent);

  else if (relationship == relation::employee)
    _graph.setEdge(ID_target, ID_start, relation::employer);
  else if (relationship == relation::employer)
    _graph.setEdge(ID_target, ID_start, relation::employee);

  _graph.setEdge(ID_start, ID_target, relationship);

  return 1;
}

bool Manager::deleteRelationship(const std::string &ID_start, const std::string &ID_target) {
  if ((!_exist_as_node(ID_start)) || (!_exist_as_node(ID_target)))
    return false; //Controllo che gli ID esistano.

  _graph.bsetEdge(ID_start, ID_target, Graph<string, string>::no_edge);
  return true;
}

bool Manager::addPost(const Post &post_to_add, const std::string &whose_ID) {
  if (_map_posts.count(whose_ID) == 0) //Controllo che esista il proprietario del post
    return false;

  vector<string> likes = post_to_add.getLikes();
  for (int i = 0; i < likes.size(); i++) {
    //Controllo che ogni like corrisponda ad un ID utente esistente.
    if (_map_users.count(likes[i]) == 0)
      return false;
  }

  vector<string> dislikes = post_to_add.getDislikes();
  for (int i = 0; i < dislikes.size(); i++) {
    //Controllo che ogni dislike corrisponda ad un ID utente esistente.
    if (_map_users.count(dislikes[i]) == 0)
      return false;
  }

  _map_posts.at(whose_ID).push_back(post_to_add);
  return true;
}

bool Manager::deletePost(const std::string &whose_ID, int post_num) {
  if (_map_posts.count(whose_ID) == 0)
    return false;
  vector<Post> &posts = _map_posts.at(whose_ID);
  if (posts.size() <= post_num) {
    return false;                                   //L'account non ha il post richiesto
  }
  _map_posts.at(whose_ID).erase(posts.begin() + post_num);
  return true;
}

bool Manager::setReaction(const bool &like_1_dislike_0, const bool &add1_remove_0, const Post &post_liked,
                          const std::string &reacting_ID) {
  if (_map_users.count(reacting_ID) ==
      0) //L'interazione è possibile solo per utenti semplici. Ne verifico l'esistenza e mi assicuro che sia un user
    return false;

  //Non essendo fornito l'ID del proprietario del post devo scorrere tutta la mappa e ogni vettore alla ricerca di quel post.
  for (auto it_map = _map_posts.begin(); it_map != _map_posts.end(); it_map++) {
    //Per ogni vettore di post..
    for (auto it_post = it_map->second.begin(); it_post != it_map->second.end(); it_post++) {
      //Lo scorro alla ricerca del post di interesse
      if (*it_post == post_liked) {
        if (reacting_ID == it_map->first) //Evito gli autolikes.
          return false;

        //Se lo trovo aggiungo il like/dislike
        if (like_1_dislike_0) {
          if (add1_remove_0)
            return it_post->AddLike(reacting_ID);
          else
            return it_post->RemoveLike(reacting_ID);
        } else {
          if (add1_remove_0)
            return it_post->AddDislike(reacting_ID);
          else
            return it_post->RemoveDislike(reacting_ID);
        }
      }
    }
  }

  return false; //Il post non è stato trovato
}

bool Manager::setReaction(const bool &like_1_dislike_0, const bool &add1_remove_0, const pair<string, Post> &post_liked,
                          const std::string &reacting_ID) {
  //Controllo l'esistenza degli ID nella mappa dei vettori di Post.
  //Per l'univocità degli ID questo controllo si riflette anche sull'esistenza dell'ID stesso in generale.
  if (_map_posts.count(post_liked.first) == 0) //Esiste il proprietario del post
    return false;
  if (reacting_ID == post_liked.first) //Non sono ammessi autolikes.
    return false;
  if (_map_users.count(reacting_ID) ==
      0) //L'interazione è possibile solo per utenti semplici. Ne verifico l'esistenza e mi assicuro che sia un user
    return false;

  //Cerco nella mappa il vettore di post associato al proprietario
  auto it_map = _map_posts.find(post_liked.first);

  //Scorro in quel vettore alla ricerca del post desiderato
  for (auto it_post = it_map->second.begin(); it_post != it_map->second.end(); it_post++) {
    if (*it_post == post_liked.second) {
      //Trovato il post di interesse aggiungo il like/dislike
      if (like_1_dislike_0) //Controllo se si tratta di un like o un dislike
      {
        if (add1_remove_0) //Controllo se è stata richiesta l'aggiunta o la rimozione
          return it_post->AddLike(reacting_ID);
        else
          return it_post->RemoveLike(reacting_ID);
      } else {
        if (add1_remove_0)
          return it_post->AddDislike(reacting_ID);
        else
          return it_post->RemoveDislike(reacting_ID);
      }
    }
  }

  return false; //Il post non è stato trovato
}

bool Manager::setReaction(const bool &like_1_dislike_0, const bool &add1_remove_0, const string &owner_ID,
                          const unsigned int &pos, const string &reacting_ID) {
  //Controllo l'esistenza degli ID nella mappa dei vettori di Post.
  //Per l'univocità degli ID questo controllo si riflette anche sull'esistenza dell'ID stesso in generale.
  if (_map_posts.count(owner_ID) == 0) //Esiste il proprietario del post
    return false;
  if (reacting_ID == owner_ID) //Non sono ammessi autolikes.
    return false;
  if (_map_users.count(reacting_ID) ==
      0) //L'interazione è possibile solo per utenti semplici. Ne verifico l'esistenza e mi assicuro che sia un user
    return false;

  //Cerco nella mappa il vettore di post associato al proprietario
  auto it_map = _map_posts.find(owner_ID); //Prelevo il vettore di post di quell'account

  if (it_map->second.size() - 1 < pos) //Se il vettore non presenta post alla posizione desiderata ritorno
    return false; //La dimensione - 1 indica l'ultima posizione del vettore disponibile all'accesso

  if (add1_remove_0) {
    if (like_1_dislike_0)
      return it_map->second[pos].AddLike(reacting_ID);
    else
      return it_map->second[pos].AddDislike(reacting_ID);
  } else {
    if (like_1_dislike_0)
      return it_map->second[pos].RemoveLike(reacting_ID);
    else
      return it_map->second[pos].RemoveDislike(reacting_ID);
  }
}

//STATISTICS

size_t Manager::NumAccounts() const {
  return _graph.nodesNumber();
}

size_t Manager::NumUsers() const {
  return _map_users.size();
}

size_t Manager::NumCompanies() const {
  return _map_companies.size();
}

size_t Manager::NumGroups() const {
  return _map_groups.size();
}

size_t Manager::NumFriends(const std::string &whose_ID) const {
  if (_map_users.count(whose_ID) == 0)
    return 0; //Controllo che sia un utente.

  return _graph.outDegree_withEdge(whose_ID, relation::friendship);
}

size_t Manager::NumRelatives(const std::string &whose_ID) const {
  if (_map_users.count(whose_ID) == 0)
    return 0; //Controllo che sia un utente.

  size_t count = 0;
  vector<vector<string>> gen_tree = GenealogicalTree(whose_ID);
  for (auto it = gen_tree.begin(); it != gen_tree.end(); it++) {
    count += it->size();
  }
  return count - 1;
}

size_t Manager::NumEmployees(const std::string &company_employer) const {
  if (_map_companies.count(company_employer) == 0)
    return 0; //Controllo che sia un'azienda.

  return _graph.outDegree_withEdge(company_employer, relation::employer);
}

size_t Manager::NumSubsidiaries(const std::string &company_main) const {
  if (_map_companies.count(company_main) == 0)
    return 0; //Controllo che sia un'azienda.

  return _graph.outDegree_withEdge(company_main, relation::partnership);
}

size_t Manager::NumMembers(const std::string &group) const {
  if (_map_groups.count(group) == 0)
    return 0; //Controllo che sia un'azienda.

  return _graph.outDegree_withEdge(group, relation::membership);
}

size_t Manager::NumBornAfter(const Date &start_date) const {
  if (!Date::CheckDate(start_date))
    return 0; //Controllo che la data abbia senso.

  size_t count = 0;
  for (auto it = _map_users.begin(); it != _map_users.end(); it++) {
    if (it->second.getBirth() > start_date)
      count++;
  }
  return count;
}

Company Manager::MostEmployingCompany() const {
  Company best;
  size_t num_members = 0;
  for (auto it = _map_companies.begin(); it != _map_companies.end(); it++) {
    if ((_graph.outDegree_withEdge(it->second.getID(), relation::employer)) > num_members) {
      best = it->second;
    }
  }
  return best;
}

vector<string> Manager::MostEmployingPartnership() const {
  Company best;
  vector<string> subs;
  size_t num_members = 0, num_prev = 0;

  //Per ogni azienda calcolo il numero di impiegati della stessa e poi sommo ad esso il numero di impiegati delle consociate. Ritorno un vettore di ID contenente la partnership con più assunzioni.
  for (auto it = _map_companies.begin(); it != _map_companies.end(); it++) {
    num_members = _graph.outDegree_withEdge(it->second.getID(), relation::employer);

    subs = _graph.branches(it->second.getID(), relation::partnership);
    for (auto it2 = subs.begin(); it2 != subs.end(); it2++) {
      num_members = num_members + _graph.outDegree_withEdge(*it2, relation::employer);
    }

    if (num_members > num_prev) {
      best = it->second;
    }
  }
  subs = _graph.branches(best.getID(),
                         relation::partnership); //Ricavo il vettore delle consociate della azienda migliore.
  subs.push_back(
          best.getID()); //Aggiungo anche l'azienda stessa che altrimenti sarebbe assente dalla lista della partnership.
  return subs;
}

User Manager::UserWithMostFriends() const {
  User best;
  size_t num_friends = 0;
  for (auto it = _map_users.begin(); it != _map_users.end(); it++) {
    if ((_graph.outDegree_withEdge(it->second.getID(), relation::friendship)) > num_friends) {
      best = it->second;
    }
  }
  return best;
}

User Manager::UserWithMostAcquaintances() const {
  User best;
  size_t num_acquaintances = 0;
  for (auto it = _map_users.begin(); it != _map_users.end(); it++) {
    if ((_graph.outDegree_withEdge(it->second.getID(), relation::knowings)) > num_acquaintances) {
      best = it->second;
    }
  }
  return best;
}

float Manager::UsersAverageAge() const {
  float sum = 0;
  Date now = Date::Now();
  //Sommo le età di tutti gli utenti e ne faccio la media aritmetica.
  for (auto it = _map_users.begin(); it != _map_users.end(); it++) {
    Date tmp = it->second.getBirth();
    sum = sum + tmp.yearsFrom(now);
  }
  return sum / (_map_users.size());
}

pair<string, Post> Manager::MostLikedPost() const {
  vector<string> all_ids = _graph.nodesVector();
  vector<Post> tmp;
  Post ext_best;
  string id_best;

  for (auto it = all_ids.begin(); it != all_ids.end(); it++) {
    tmp = _map_posts.at(*it);
    if (!tmp.empty()) {
      vector<Post>::iterator it2 = tmp.begin();
      Post best = *it2;
      for (; it2 != tmp.end(); it2++) {
        if (*it2 > best)
          best = *it2;
      }

      if (best > ext_best) {
        ext_best = best;
        id_best = *it;
      }
    }
  }
  return pair<string, Post>(id_best, ext_best);
}

pair<string, Post> Manager::MostDislikedPost() const {
  vector<string> all_ids = _graph.nodesVector();
  vector<Post> tmp;
  Post ext_best;
  string id_best;
  for (auto it = all_ids.begin(); it != all_ids.end(); it++) {
    tmp = _map_posts.at(*it);
    if (tmp.size()!=0)
    {
      vector<Post>::iterator it2 = tmp.begin();
      Post best = *it2;

      for (; it2 != tmp.end(); it2++) {
        if (it2->NumDislikes() > best.NumDislikes())
          best = *it2;
      }

      if (best.NumDislikes() > ext_best.NumDislikes()) {
        ext_best = best;
        id_best = *it;
      }
    }
  }
  return pair<string, Post>(id_best, ext_best);
}

string Manager::MostLiked_DislikedAccount(const bool &like_1_dislike_0) const {
  int best_sum = 0;
  std::string best_id;

  for (auto it = _map_posts.begin(); it != _map_posts.end(); it++) { //Scorri tutta la mappa dei post
    int sum = 0;
    for (int i = 0; i < it->second.size(); i++)
    {                                                       //Somma tutti i likes di tutti i post dell'account attuale
      if (like_1_dislike_0)
      {
        sum += it->second[i].NumLikes();
      }
      else
      {                                                        //Somma tutti i dislike dell'account attuale
        sum += it->second[i].NumDislikes();
      }
    }
    if (sum > best_sum) {                                           //Abbiamo contato più like/dislike
      best_sum = sum;                                               //Aggiorna la nuova somma
      best_id = it->first;                                          //L'ID dell'account migliore è in it->first
    }
  }
  return best_id;
}

pair<string, Post> Manager::RatioReactionPost(const bool &best_1_worst_0) const {
  std::string best_id;
  size_t best_post = 0;
  bool done = false;
  auto it_post = _map_posts.begin();
  while (!done) {
    if (it_post->second.size() != 0) {
      best_id = it_post->first;
      done = true;
    }

    it_post++;
  }

  //Non posso assumere che il primo sia il migliore, l'algoritmo accede alla mappa tramite .at, se il primo ID non possiede post il programma va in EXCEPTION.
  //Fisso come "migliore ID iniziale" il primo ID avente almeno un post.

  //Per ogni ID determino quale post abbia il miglior/peggior rapporto di gradimento
  for (auto it = _map_posts.begin(); it != _map_posts.end(); it++) {                    //Scorri tutti gli account
    if (best_1_worst_0) {                                                              //Ricerca del migliore
      for (size_t i = 0;
           i < it->second.size(); i++)                                   //Scorri ogni post dell'attuale account
        if (it->second[i].RatioReaction() > _map_posts.at(best_id)[best_post].RatioReaction()) {
          //Il post in posizione i dell'account it->first ha un RatioReaction migliore dell'attuale primo in classifica
          best_id = it->first;
          best_post = i;
        }
    } else {
      for (size_t i = 0; i < it->second.size(); i++)
        if (it->second[i].RatioReaction() < _map_posts.at(best_id)[best_post].RatioReaction()) {
          best_id = it->first;
          best_post = i;
        }
    }
  }
  return pair<string, Post>(best_id, _map_posts.at(best_id)[best_post]);
}

string Manager::RatioReactionAccount(const bool &best_1_worst_0) const {
  vector<string> all_ids = _graph.nodesVector();
  vector<Post> tmp;
  string best_ID;
  float ratio = 0, best_ratio = 0;
  for (auto it = all_ids.begin(); it != all_ids.end(); it++) {
    //Per ogni ID calcolo il rapporto di gradimento e stabilisco quale ID abbia il migliore/peggiore.
    tmp = _map_posts.at(*it);
    for (auto it2 = tmp.begin(); it2 != tmp.end(); it2++) {
      ratio = ratio + it2->RatioReaction();
    }
    ratio = ratio / tmp.size();

    if (ratio != 0) {
      if (best_1_worst_0) {
        if (ratio > best_ratio) {
          best_ID = *it;
          best_ratio = ratio;
        }
      }
      if (!best_1_worst_0) {
        if (ratio < best_ratio) {
          best_ID = *it;
          best_ratio = ratio;
        }
      }
    }
  }
  return best_ID;
}

vector<string> Manager::LonerPeople(const unsigned int &relations, const unsigned int &memberships, const bool &not_employed, const unsigned int &newsreactions) const {
  unordered_set<string> set; //Lavoro con i set per la buona complessità delle operazioni e poi infine ritornerò un vettore.
  bool isLoner = true; //Controllo sul lupo solitario
  bool isValid = false; //Controllo validità dei parametri

  if ((relations != 0) || (memberships != 0) || (not_employed) || (newsreactions != 0))
    isValid = true;
  else
    return vector<string>(); //Se i parametri non sono validi torna un vector vuoto

  for (auto it = _map_users.begin(); it != _map_users.end(); it++) {
    //Per ogni utente semplice controllo se esso è lupo solitario o meno in base ai parametri forniti
    if (memberships != 0) {
      if (_graph.outDegree_withEdge(it->first, relation::membership) > memberships)
        isLoner = false;
    }

    if ((not_employed) && (isLoner)) {
      //Se è richiesto che il lupo solitario non sia impiegato in una azienda (employed==true) allora controllo che effettivamente non lo sia.
      if (_graph.outDegree_withEdge(it->first, relation::employee) != 0)
        isLoner = false;
    }

    if ((relations != 0) && (isLoner)) {
      //Se è fornito un numero di relazioni considero come lupi solitari coloro che hanno meno relazioni del numero indicato.
      int count_relations = 0;
      count_relations = (int) _graph.outDegree_withEdge(it->first, relation::friendship);
      count_relations = count_relations + (int) _graph.outDegree_withEdge(it->first, relation::knowings);
      count_relations = count_relations + (int) _graph.outDegree_withEdge(it->first, relation::parent);
      count_relations = count_relations + (int) _graph.outDegree_withEdge(it->first, relation::born);
      count_relations = count_relations + (int) _graph.outDegree_withEdge(it->first, relation::partner);

      if (count_relations > relations)
        isLoner = false;
    }

    if (isLoner)
      set.insert(it->second.getID());
    else
      isLoner = true; //per ricominciare il ciclo correttamente
  }

  if (newsreactions != 0) {
    for (auto it_users = _map_users.begin(); it_users != _map_users.end(); it_users++) {
      //Per ogni utente conto in ogni vettore di post quante reazioni ha effettuato
      int count_reactions = 0;

      for (auto it = _map_posts.begin(); it != _map_posts.end(); it++) {
        if (it_users->first !=
            it->first) //Non conto le occorrenze di un utente tra i suoi post: non sono ammessi autolikes.
        {
          for (auto it_post = it->second.begin(); it_post != it->second.end(); it_post++) {
            //Per ogni post controllo l'occorrenza dell'utente tra i likes e i dislikes.
            if (it_post->LikeExists(it_users->first) != false)
              count_reactions++;
            if (it_post->DislikeExists(it_users->first) != false)
              count_reactions++;
          }
        }
      }
      if (count_reactions < newsreactions) {
        //Se il numero di reazioni è minore del parametro, l'utente è un lupo solitario
        if (set.count(it_users->first) == 0)
          set.insert(it_users->first);
      } else {
        //Se il numero di reazioni è maggiore del parametro allora non è un lupo solitario, indipententemente da tutti gli altri fattori, quindi lo rimuovo dal set.
        if (set.count(it_users->first) != 0)
          set.erase(it_users->first);
      }
    }
  }

  vector<string> list(set.begin(), set.end()); //Converto il set in un vector
  return list;
}

vector<pair<string, float>> Manager::SortedNewsRatioCompanies(const bool &with_partners, const float &min_ratio) const {
  vector<pair<string, float>> companies_ratio; //Vettore di coppie ID-rapporto
  vector<string> all_IDs = getCompaniesIDs(); //Vettore di ID delle companies

  for (auto it_ids = all_IDs.begin(); it_ids != all_IDs.end(); it_ids++)  //Per ogni compagnia..
  {
    vector<Post> posts_tmp = _map_posts.at(*it_ids); //Accedo al suo vettore di post
    float total_ratio_tmp = 0;
    float sum_ratios = 0;
    size_t num_posts = posts_tmp.size();

    for (auto it_post = posts_tmp.begin(); it_post != posts_tmp.end(); it_post++) {
      sum_ratios += it_post->RatioReaction(); //Per ogni post sommo i singoli rapporti
    }

    if (with_partners) //Se devo considerare anche le consociate..
    {
      vector<string> co_workers_IDs = _graph.branches(*it_ids,
                                                      relation::partnership); //Estraggo il vettore di ID delle consociate dal grafo

      for (auto it_coworkers = co_workers_IDs.begin();
           it_coworkers != co_workers_IDs.end(); it_coworkers++) //Per ogni consociata..
      {
        vector<Post> posts_tmp_coworker = _map_posts.at(*it_coworkers); //Accedo al suo vettore di post
        num_posts += posts_tmp_coworker.size(); //Sommo al numero di post complessivi anche quello dei post delle consociate

        for (auto it_post_coworker = posts_tmp_coworker.begin();
             it_post_coworker != posts_tmp_coworker.end(); it_post_coworker++) //Per ogni post..
        {
          sum_ratios += it_post_coworker->RatioReaction(); //Aggiungo i rapporti alla precedente somma
        }
      }
    }

    if (num_posts == 0) //Se complessivamente non ci sono post allora il rapporto sarà nullo, evito la divisione per 0.
      total_ratio_tmp = 0;
    else
      total_ratio_tmp = sum_ratios /
                        num_posts; //Calcolo il rapporto complessivo dividendo la somma dei rapporti per il numero di post

    if (total_ratio_tmp >= min_ratio) //Se supera il rapporto minimo lo inserisco nella lista delle coppie
      companies_ratio.emplace_back(make_pair(*it_ids, total_ratio_tmp));

  } //Fine delle compagnie

  sort(companies_ratio.begin(), companies_ratio.end(),
       [](const pair<string, float> &pair1, const pair<string, float> &pair2) {
         return pair1.second > pair2.second;
       }); //Ordino il vettore di coppie in base al rapporto sfruttando una lambda function.

  /*vector<string> sorted_list; //Sarà il vettore di ID che ritornerò
  
  for (auto it=companies_ratio.begin(); it!=companies_ratio.end(); it++)
  {
    sorted_list.push_back(it->first);
  }
  
  return sorted_list;*/

  return companies_ratio;
}

vector<vector<string>> Manager::GenealogicalTree(const string &whose_ID) const {
  //Rappresenta le varie generazioni della famiglia di whose_ID
  //Controlla che l'ID richiesto esista
  if (_map_users.count(whose_ID) == 0) {
    //L'account non esiste
    return std::vector<std::vector<std::string>>();       //Ritorna un vettore vuoto
  }

  using Node = std::pair<std::string, int>;

  std::set<std::string> analyzed_nodes;                   //Nodi già analizzati
  std::deque<Node> nodes_to_analyze;                      //Nodi da analizzare organizzati in una coda di nodo - generazione prevista
  std::map<int, std::vector<std::string>> generations;    //Generazioni

  Node current_node = {whose_ID, 0};                      //Il nodo di partenza viene imposto a generazione 0
  nodes_to_analyze.push_back(current_node);               //Il primo nodo viene aggiunto alla coda

  while (!nodes_to_analyze.empty()) {                     //Scorri finchè la lista non è completa
    current_node = nodes_to_analyze.front();              //Estrai il primo elemento della lista
    nodes_to_analyze.pop_front();

    //Ottieni dal grafo tutti i nodi connessi al current_node tramite una relazione di paternità (in entrambi i versi)
    std::vector<std::string> next_generation = _graph.branches(current_node.first, relation::parent);
    std::vector<std::string> previous_generation = _graph.branches(current_node.first, relation::born);

    for (int i = 0; i < previous_generation.size(); i++) {
      bool node_analyzed = analyzed_nodes.find(previous_generation[i]) != analyzed_nodes.end();
      /* Per controllare se il nodo è in lista bisogna usare la find_if, in quanto la lista è composta da pair, ma dei
       * pair in questo caso conta soltanto il primo elemento, l'ID del nodo. Viene perciò definita una Lambda function
       * per compattare il codice. */
      bool node_in_list =
              std::find_if(nodes_to_analyze.begin(),
                           nodes_to_analyze.end(),
                           [previous_generation, i](const Node &n) {
                             return n.first == previous_generation[i];
                           }) //Lambda function
              != nodes_to_analyze.end();

      if (!node_analyzed && !node_in_list) {
        //Metti il nodo i-esimo in lista ed assegnalo alla generazione antecedente a quella del current_node
        nodes_to_analyze.emplace_back(std::make_pair(previous_generation[i], current_node.second - 1));
      }
    }
    for (int i = 0; i < next_generation.size(); i++) {
      //Controlla che il nodo non sia già stato analizzato/non sia già segnato in lista (nello stesso modo di prima):
      bool node_analyzed = analyzed_nodes.find(next_generation[i]) != analyzed_nodes.end();
      bool node_in_list =
              std::find_if(nodes_to_analyze.begin(),
                           nodes_to_analyze.end(),
                           [next_generation, i](const Node &n) { return n.first == next_generation[i]; })
              != nodes_to_analyze.end();

      if (!node_analyzed && !node_in_list) {
        //Questa volta il nodo i-esimo deve appartenere alla generazione che segue quella del current_node
        nodes_to_analyze.emplace_back(next_generation[i], current_node.second + 1);
      }
    }
    //Aggiungi il nodo corrente alla mappa delle generazioni e segnalo come analizzato
    generations[current_node.second].push_back(current_node.first);
    analyzed_nodes.insert(current_node.first);
  }

  vector<vector<string>> tree(generations.size());
  int i = 0;
  for (auto it_gen = generations.begin(); it_gen != generations.end(); it_gen++, i++) {
    tree[i] = it_gen->second;
  }
  return tree;
}

vector<string> Manager::FormatTree(const vector<vector<string>> &tree_to_format) const {

  if (tree_to_format.size() < 3) {
    //L'albero non è lungo a sufficienza
    return vector<string>();
  }

  vector<string> tree;

  int gen_num = 1; //Conto il numero di generazioni ai fini della formattazione
  for (auto it_tree = tree_to_format.begin(); it_tree != tree_to_format.end(); it_tree++) //Scorro l'albero
  {
    stringstream ss;
    ss << "Gen. " << gen_num << ": ";

    for (auto it_gen = it_tree->begin(); it_gen != it_tree->end(); it_gen++) //Per ogni generazione compilo lo stream
    {
      ss << *it_gen << " ";

      vector<string> info_about = _graph.branches(*it_gen,
                                                  relation::partner); //Ricavo le eventuali informazioni sui partner
      if (!info_about.empty()) {
        ss << "(+";
        for (auto it_partners = info_about.begin(); it_partners != info_about.end(); it_partners++) {
          ss << " " << *it_partners;
        }
        ss << ") ";
      }

      info_about = _graph.branches(*it_gen, relation::born); //Ricavo le eventuali informazioni sui predecessori
      if (!info_about.empty()) {
        ss << "(from:";
        for (auto it_parent = info_about.begin(); it_parent != info_about.end(); it_parent++) {
          ss << " " << *it_parent;
        }
        ss << ") ";
      }

      auto it_check = it_gen;
      it_check++;
      if (it_check != it_tree->end())
        ss << " // ";
    }

    gen_num++;
    tree.push_back(ss.str()); //Inserisco la generazione formattata nell'albero
    ss.clear(); //Pulisco lo stream così da ricominciare correttamente
  }

  return tree;
}

string Manager::PrintTree(const std::string &whose_ID) const {
  vector<string> tree_to_print = FormatTree(GenealogicalTree(whose_ID)); //Invoco a cascata le funzioni di creazione dell'albero e la formattazione dello stesso
  stringstream ss;

  if (tree_to_print.size() < 3) {
    //L'albero non è valido poiché non lungo a sufficienza
    return string();
  }

  if (*tree_to_print.begin() == "ID") {
    return string();
  }

  for (auto it = tree_to_print.begin(); it != tree_to_print.end(); it++) //Stampo ogni livello generazionale
  {
    ss << *it << endl;
  }

  return ss.str();
}

vector<string> Manager::PrintAllTrees() const {
  //Adotto il set come una "coda", gli utenti stampati in un albero vengono rimossi da tale coda.
  //Il set mi consente di eliminare agilmente la key con .erase!

  vector<string> all_IDs = getUsersIDs();
  set<string> IDs;
  stringstream ss;
  vector<string> trees_to_print;

  for (auto it = all_IDs.begin(); it != all_IDs.end(); it++) {
    IDs.insert(
            *it); //Costruisco il set in questa maniera perché il costruttore a iteratori del set torna, stranamente, BAD_ACCESS strani..
  }

  int count = 1;
  while (!IDs.empty()) {
    vector<vector<string>> tree = GenealogicalTree(
            *IDs.begin()); //Ricavo l'albero così da estrapolare gli ID già trattati e rimuoverli dalla coda

    if (tree.size() >= 3) {
      ss.str(string());
      ss << "##### Tree no. " << count << " #####" << endl; //Formatto
      ss << PrintTree(*IDs.begin()); //Carico un albero

      for (auto it_tree = tree.begin(); it_tree != tree.end(); it_tree++) {
        for (auto it_IDs = it_tree->begin(); it_IDs != it_tree->end(); it_IDs++) {
          if (IDs.count(*it_IDs) != 0)
            IDs.erase(*it_IDs);
        }
      }

      count++;
      ss << endl;
      trees_to_print.push_back(ss.str());
    } else {
      IDs.erase(*IDs.begin());
    }
  }

  return trees_to_print;
}

//PRIVATE METHODS

void Manager::_setKeys(const vector<User> &users) {
  for (auto it = users.begin(); it != users.end(); it++) {
    _map_users[it->getID()] = *it;
    if (_map_posts.count(it->getID()) == 0)
      _map_posts[it->getID()] = vector<Post>();
  }
}

void Manager::_setKeys(const vector<Company> &companies) {
  for (auto it = companies.begin(); it != companies.end(); it++) {
    _map_companies[it->getID()] = *it;
    if (_map_posts.count(it->getID()) == 0)
      _map_posts[it->getID()] = vector<Post>();
  }
}

void Manager::_setKeys(const vector<Group> &groups) {
  for (auto it = groups.begin(); it != groups.end(); it++) {
    _map_groups[it->getID()] = *it;
    if (_map_posts.count(it->getID()) == 0)
      _map_posts[it->getID()] = vector<Post>();
  }
}

void Manager::_setNodes() {
  for (auto it_u = _map_users.begin(); it_u != _map_users.end(); it_u++) {
    _graph.addNode(it_u->second.getID());
  }
  
  for (auto it_c = _map_companies.begin(); it_c != _map_companies.end(); it_c++) {
    _graph.addNode(it_c->second.getID());
  }
  
  for (auto it_g = _map_groups.begin(); it_g != _map_groups.end(); it_g++) {
    _graph.addNode(it_g->second.getID());
  }
}

bool Manager::_exist_as_node(const string &ID_to_check) const {
  return _graph.find(ID_to_check) != _graph.nodesNumber();
}

bool Manager::_checkAge(const std::string &ID_old, const std::string &ID_young) const {
  //Funzione chiamata a controlli già effettuati
  Date old_birth = _map_users.at(ID_old).getBirth();
  Date young_birth = _map_users.at(ID_young).getBirth();
  return old_birth < young_birth;
}
