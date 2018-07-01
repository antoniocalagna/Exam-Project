//
// Created by Alex on 01/07/2018.
//

#ifndef SOCIAL_NETWORK_SOCIAL_HANDLERS_H
#define SOCIAL_NETWORK_SOCIAL_HANDLERS_H

#include "FileHandler.h"

//Specializzazioni del FileHandler
namespace FH {

class AccountsHandler : public FileHandler {
private:
  Error _checkLine(const std::string &line) const;
  std::unordered_set<std::string> _emptyBuffer(IOBuffer &buffer) const;
  void _fillBuffer(IOBuffer &buffer, const std::string data) const;
};

class RelationsHandler : public FileHandler {
  Error _checkLine(const std::string &line) const;
  std::unordered_set<std::string> _emptyBuffer(IOBuffer &buffer) const;
  void _fillBuffer(IOBuffer &buffer, const std::string data) const;
};

class PostsHandler : public FileHandler {
  Error _checkLine(const std::string &line) const;
  std::unordered_set<std::string> _emptyBuffer(IOBuffer &buffer) const;
  void _fillBuffer(IOBuffer &buffer, const std::string data) const;
};
} //namespace FH
#endif //SOCIAL_NETWORK_SOCIAL_HANDLERS_H
