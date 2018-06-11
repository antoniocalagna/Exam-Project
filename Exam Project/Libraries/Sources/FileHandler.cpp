//
// Created by Alex on 06/06/2018.
//

#include "FileHandler.h"

/**###############################
 * ## Constructors & Destructor ##
 * ###############################
 */

FH::FileHandler::FileHandler(const std::string &filename) {
  open(filename);
}

FH::FileHandler::~FileHandler() {
  close();
}

/**#############
 * ## General ##
 * #############
 */
bool FH::FileHandler::open(const std::string &filename) {
  if (_file.is_open()) {
    close();
  }
  _file.open(filename);
  return _file.is_open();
}

bool FH::FileHandler::is_open() {
  return _file.is_open();
}

void FH::FileHandler::close() {
  _file.close();
}

FH::Error FH::FileHandler::checkLineFormat(Error (*checker_func)(const std::stringstream &),
                                           const std::string &line) {
  return checker_func(std::stringstream(line));
}

FH::Error FH::FileHandler::checkFile(Error (*checker_func)(const std::stringstream &)) {
  if (!_file.is_open()) {
    return {0xFFFFFFFF, 0};
  }
  std::string line;
  unsigned int current_line = 1;
  while (_file.good()) {
    std::getline(_file, line);
    FH::Error line_status = checkLineFormat(checker_func, line);
    if (line_status.code != 0)
      return {line_status.code, current_line};
    current_line++;
  }
}

User FH::FileHandler::flushUser() {
  if (_user_buffer.empty()) { return User(); }  //Controlla che la queue non sia vuota
  const User &temp = _user_buffer.front();
  _user_buffer.pop();
  return temp;
}

Group FH::FileHandler::flushGroup() {
  if (_group_buffer.empty()) { return Group(); }
  const Group &temp = _group_buffer.front();
  _group_buffer.pop();
  return temp;
}

Company FH::FileHandler::flushCompany() {
  if (_company_buffer.empty()) { return Company(); }
  const Company &temp = _company_buffer.front();
  _company_buffer.pop();
  return temp;
}