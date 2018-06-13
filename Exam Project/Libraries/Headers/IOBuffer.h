//
// Created by Alex on 13/06/2018.
//

#ifndef SOCIAL_NETWORK_IOBUFFER_H
#define SOCIAL_NETWORK_IOBUFFER_H

#include <queue>
#include "User.h"
#include "Group.h"
#include "Company.h"
#include "Post.h"

class IOBuffer {
private:
  std::queue<User> _users;
  std::queue<Group> _groups;
  std::queue<Company> _companies;
  std::queue<std::pair<std::string, Post>> _posts;

public:
  /**Constructors & Destructor*/
  IOBuffer() = default;
  ~IOBuffer() = default;
  
  /**Getters*/
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
  IOBuffer &operator<<(const std::pair<std::string, Post> &to_get);
  IOBuffer &operator>>(std::pair<std::string, Post> &to_return);
};


#endif //SOCIAL_NETWORK_IOBUFFER_H
