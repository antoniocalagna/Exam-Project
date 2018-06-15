//
// Created by Alex on 13/06/2018.
//

#include "IOBuffer.h"

bool IOBuffer::usersEmpty() const { return _users.empty(); }

bool IOBuffer::groupsEmpty() const { return _groups.empty(); }

bool IOBuffer::companiesEmpty() const { return _companies.empty(); }

bool IOBuffer::postsEmpty() const { return _posts.empty(); }

void IOBuffer::clear() {
  while (!_users.empty())
    _users.pop();
  while (!_companies.empty())
    _companies.pop();
  while (!_groups.empty())
    _groups.pop();
  while (!_users.empty())
    _posts.pop();
}

IOBuffer &IOBuffer::operator<<(const User &to_get) {
  _users.push(to_get);
  return *this;
}

IOBuffer &IOBuffer::operator<<(const Group &to_get) {
  _groups.push(to_get);
  return *this;
}

IOBuffer &IOBuffer::operator<<(const Company &to_get) {
  _companies.push(to_get);
  return *this;
}

IOBuffer &IOBuffer::operator<<(const std::pair<std::string, Post> &to_get) {
  _posts.push(to_get);
  return *this;
}

IOBuffer &IOBuffer::operator>>(User &to_return) {
  User temp = _users.front();
  _users.pop();
  to_return = temp;
  return *this;
}

IOBuffer &IOBuffer::operator>>(Group &to_return) {
  Group temp = _groups.front();
  _groups.pop();
  to_return = temp;
  return *this;
}

IOBuffer &IOBuffer::operator>>(Company &to_return) {
  Company temp = _companies.front();
  _companies.pop();
  to_return = temp;
  return *this;
}

IOBuffer &IOBuffer::operator>>(std::pair<std::string, Post> &to_return) {
  std::pair<std::string, Post> temp = _posts.front();
  _posts.pop();
  to_return.first = temp.first;
  to_return.second = temp.second;
  return *this;
} 
