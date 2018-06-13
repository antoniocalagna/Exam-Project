//
// Created by Alex on 06/06/2018.
//

#ifndef SOCIAL_NETWORK_FILEHANDLER_H
#define SOCIAL_NETWORK_FILEHANDLER_H

#include <fstream>
#include <sstream>
#include "User.h"
#include "Group.h"
#include "Company.h"
#include "Post.h"

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

public:
  /**Constructors & Destructor*/
  FileHandler() = default;
  FileHandler(const std::string &filename);
  ~FileHandler();
  
  /**General*/
  bool open(const std::string &filename);
  bool is_open() const;
  void close();
  Error checkLineFormat(Error (*checker_func)(std::stringstream &), const std::string &line) const;
  Error checkFile(Error (*checker_func)(std::stringstream &));
  
  /**File data exchange*/
};  //Class FileHandler

bool isFormatChar(const std::string &s, size_t pos);
std::string readField(const std::string &field, const std::string &data);

//Controllo dei file
Error IDsfile(std::stringstream &line);
Error relationsFile(std::stringstream &line);
Error postsFile(std::stringstream &line);

};//Namespace FH
#endif //SOCIAL_NETWORK_FILEHANDLER_H
