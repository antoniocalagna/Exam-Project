//
// Created by Alex on 13/06/2018.
//

#include "IOBuffer.h"

IOBuffer::IOBuffer(const IOBuffer &to_copy) {
  *this = to_copy;
}

bool IOBuffer::usersEmpty() const { return _users.empty(); }

bool IOBuffer::groupsEmpty() const { return _groups.empty(); }

bool IOBuffer::companiesEmpty() const { return _companies.empty(); }

bool IOBuffer::postsEmpty() const { return _posts.empty(); }

bool IOBuffer::relationsEmpty() const { return _relations.empty(); }

size_t IOBuffer::size() const {
  return _users.size() + _groups.size() + _companies.size() + _posts.size() + _relations.size();
}

void IOBuffer::clear() {
  _users.clear();
  _companies.clear();
  _groups.clear();
  _posts.clear();
}

void IOBuffer::append(const IOBuffer &buff) {
  //Copia (SENZA SCARICARE) il contenuto del buffer buff in *this.
  for(auto it = buff._users.begin(); it != buff._users.end(); it++) {
    _users.push_back(*it);
  }
  for(auto it = buff._groups.begin(); it != buff._groups.end(); it++) {
    _groups.push_back(*it);
  }
  for(auto it = buff._companies.begin(); it != buff._companies.end(); it++) {
    _companies.push_back(*it);
  }
  for(auto it = buff._posts.begin(); it != buff._posts.end(); it++) {
    _posts.push_back(*it);
  }
  for(auto it = buff._relations.begin(); it != buff._relations.end(); it++) {
    _relations.push_back(*it);
  }
}

IOBuffer &IOBuffer::operator<<(const User &to_get) {
  _users.push_back(to_get);     //Inserisci i nuovi elementi al fondo...
  return *this;
}

IOBuffer &IOBuffer::operator<<(const Group &to_get) {
  _groups.push_back(to_get);
  return *this;
}

IOBuffer &IOBuffer::operator<<(const Company &to_get) {
  _companies.push_back(to_get);
  return *this;
}

IOBuffer &IOBuffer::operator<<(const m_Post &to_get) {
  _posts.push_back(to_get);
  return *this;
}

IOBuffer &IOBuffer::operator<<(const IOBuffer::Relation &to_get) {
  _relations.push_back(to_get);
  return *this;
}

IOBuffer &IOBuffer::operator>>(User &to_return) {
  User temp = _users.front();     //E pesca quelli nuovi dalla testa
  _users.pop_front();             //Scaricandoli dal buffer
  to_return = temp;
  return *this;
}

IOBuffer &IOBuffer::operator>>(Group &to_return) {
  Group temp = _groups.front();
  _groups.pop_front();
  to_return = temp;
  return *this;
}

IOBuffer &IOBuffer::operator>>(Company &to_return) {
  Company temp = _companies.front();
  _companies.pop_front();
  to_return = temp;
  return *this;
}

IOBuffer &IOBuffer::operator>>(m_Post &to_return) {
  m_Post temp = _posts.front();
  _posts.pop_front();
  to_return = temp;
  return *this;
}

IOBuffer &IOBuffer::operator>>(IOBuffer::Relation &to_return) {
  Relation temp = _relations.front();
  _relations.pop_front();
  to_return = temp;
  return *this;
}