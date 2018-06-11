//
// Created by Alex on 06/06/2018.
//

#ifndef SOCIAL_NETWORK_FILEHANDLER_H
#define SOCIAL_NETWORK_FILEHANDLER_H

#include <fstream>
#include <sstream>
#include <queue>
#include "User.h"
#include "Group.h"
#include "Company.h"

namespace FH {
struct Error {
  unsigned int code;
  unsigned int data;
  
};

class FileHandler {
private:
  std::fstream _file;        //File
  std::string _filename;     //Nome del file
  
  /**Buffers*/
  std::queue<User> _user_buffer;
  std::queue<Group> _group_buffer;
  std::queue<Company> _company_buffer;
  
public:
  /**Constructors & Destructor*/
  FileHandler() = default;
  FileHandler(const std::string &filename);
  ~FileHandler();
  
  /**General*/
  bool open(const std::string &filename);
  bool is_open();
  void close();
  bool updateInfo(const std::string &new_info, Error (*checker_func)(const std::stringstream &line));
  Error checkLineFormat(Error (*checker_func)(const std::stringstream &), const std::string &line);
  Error checkFile(Error (*checker_func)(const std::stringstream &));
  
  /**Buffer flushing*/
  User flushUser();
  Group flushGroup();
  Company flushCompany();
};

} //Namespace FH
#endif //SOCIAL_NETWORK_FILEHANDLER_H
