//
// Created by Alex on 13/06/2018.
//

#ifndef SOCIAL_NETWORK_IOBUFFER_H
#define SOCIAL_NETWORK_IOBUFFER_H

#include <deque>
#include <algorithm>
#include "User.h"
#include "Group.h"
#include "Company.h"
#include "Post.h"

class IOBuffer {
public:
  /**Aliases*/
  using Users = std::deque<User>;
  using Groups = std::deque<Group>;
  using Companies= std::deque<Company>;
  using m_Post = std::pair<std::string, Post>;
  using Posts = std::deque<m_Post>;
  using Relation = std::pair<std::pair<std::string, std::string>, std::string>;
  using Relations = std::deque<Relation>;

private:
  Users _users;
  Groups _groups;
  Companies _companies;
  Posts _posts;
  Relations _relations;

public:
  /**Constructors & Destructor*/
  IOBuffer() = default;
  ~IOBuffer() = default;
  
  /**Getters*/
  bool replicatedID(const std::string &ID) const;
  bool overwritingRelation(const Relation &rel);
  bool usersEmpty() const;
  bool groupsEmpty() const;
  bool companiesEmpty() const;
  bool postsEmpty() const;
  
  /**General*/
  void clear();
  
  /**Operators*/
  IOBuffer &operator<<(const User &to_get);
  IOBuffer &operator>>(User &to_return);
  IOBuffer &operator<<(const Company &to_get);
  IOBuffer &operator>>(Company &to_return);
  IOBuffer &operator<<(const Group &to_get);
  IOBuffer &operator>>(Group &to_return);
  IOBuffer &operator<<(const m_Post &to_get);
  IOBuffer &operator>>(m_Post &to_return);
  IOBuffer &operator<<(const Relation &to_get);
  IOBuffer &operator>>(Relation &to_return);
};


#endif //SOCIAL_NETWORK_IOBUFFER_H 
