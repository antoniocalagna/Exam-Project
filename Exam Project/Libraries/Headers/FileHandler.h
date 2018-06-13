//
// Created by Alex on 06/06/2018.
//

#ifndef SOCIAL_NETWORK_FILEHANDLER_H
#define SOCIAL_NETWORK_FILEHANDLER_H

#include <fstream>
#include <sstream>
#include "IOBuffer.hpp"
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
  Error pushToFile(Error (*checker_func)(std::stringstream &), const std::string &new_data);
  
  template<typename data_T>
  void pullFromFile(bool (*getter_func)(std::stringstream &, IOBuffer<data_T> &), IOBuffer<data_T> &buffer);
};  //Class FileHandler

bool isFormatChar(const std::string &s, size_t pos);
std::string readField(const std::string &field, const std::string &data);

//Controllo dei file
Error IDsfile(std::stringstream &line);
Error relationsFile(std::stringstream &line);
Error postsFile(std::stringstream &line);

//Acquisizione dati
template <typename data_T>
bool IDsfile(std::stringstream &line, IOBuffer<data_T> &buffer);

std::string err(Error e);
} //Namespace FH

//Implementazione della funzione template
template<typename data_T>
void FH::FileHandler::pullFromFile(bool (*getter_func)(stringstream &, IOBuffer<data_T> &),
                                   IOBuffer<data_T> &buffer) {
  if (!_file.is_open()) { return; }
  std::string line;
  while (_file.good()) {
    std::getline(_file, line);
    std::stringstream line_stream(line);
    bool success = getter_func(line_stream, buffer);
    if (!success) { return; }
  }
}

#endif //SOCIAL_NETWORK_FILEHANDLER_H
