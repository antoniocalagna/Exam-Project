//
// Created by Alex on 01/07/2018.
//

#ifndef SOCIAL_NETWORK_SOCIAL_HANDLERS_H
#define SOCIAL_NETWORK_SOCIAL_HANDLERS_H

#include "FileHandler.h"
#include "User.h"
#include "Group.h"
#include "Company.h"
#include "Post.h"

//Specializzazioni del FileHandler
namespace FH {
/** ## Specializzazione dei metodi virtuali di FH ## **/
class AccountsHandler : public FileHandler {
private:
  Error _checkLine(const std::string &line) const override;
  std::unordered_set<std::string> _emptyBuffer(IOBuffer &buffer) const override;
  void _fillBuffer(IOBuffer &buffer, const std::string &data) const override;
};

class RelationsHandler : public FileHandler {
private:
  Error _checkLine(const std::string &line) const override;
  std::unordered_set<std::string> _emptyBuffer(IOBuffer &buffer) const override;
  void _fillBuffer(IOBuffer &buffer, const std::string &data) const override;
};

class PostsHandler : public FileHandler {
private:
  Error _checkLine(const std::string &line) const override;
  std::unordered_set<std::string> _emptyBuffer(IOBuffer &buffer) const override;
  void _fillBuffer(IOBuffer &buffer, const std::string &data) const override;
};

/*
 * Funzioni che trasformano il dato in una stringa
 */
std::string formatOutput(const User &user);
std::string formatOutput(const Group &group);
std::string formatOutput(const Company &company);
std::string formatOutput(const IOBuffer::Relation &relation);
std::string formatOutput(const IOBuffer::m_Post &post);
} //namespace FH
#endif //SOCIAL_NETWORK_SOCIAL_HANDLERS_H
