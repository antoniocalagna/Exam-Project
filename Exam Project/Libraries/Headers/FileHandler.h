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
public:
  /**Static data */
  static const char parser_char = '&';
  
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
  bool updateInfo(const std::string &new_info, Error (*checker_func)(std::stringstream line));
  Error checkLineFormat(Error (*checker_func)(std::stringstream &), const std::string &line);
  Error checkFile(Error (*checker_func)(std::stringstream &));
  
  /**Buffer flushing*/
  User flushUser();
  Group flushGroup();
  Company flushCompany();
};  //Class FileHandler

bool isFormatChar(const std::string &s, size_t pos);
std::string readField(const std::string &field, const std::string &data);

Error IDsfile(std::stringstream &line);
Error relationsFile(std::stringstream &line);
Error postsFile(std::stringstream &line);

std::string err(Error e);
} //Namespace FH
#endif //SOCIAL_NETWORK_FILEHANDLER_H
