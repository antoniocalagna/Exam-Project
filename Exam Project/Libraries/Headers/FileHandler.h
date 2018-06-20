//
// Created by Alex on 06/06/2018.
//

#ifndef SOCIAL_NETWORK_FILEHANDLER_H
#define SOCIAL_NETWORK_FILEHANDLER_H

#include <fstream>
#include <sstream>
#include <unordered_set>
#include "User.h"
#include "Group.h"
#include "Company.h"
#include "Post.h"
#include "IOBuffer.h"

namespace FH {

struct Error {
  unsigned int code;
  unsigned int data;
};

class FileHandler {
public:
  /**Static data */
  static const char parser_char;
  static const char format_chars[];

private:
  std::fstream _file;        //File
  std::string _filename;     //Nome del file
  
  /**Static data*/
  static const std::string windows_relative_path;
  static const std::string mac_relative_path;
  static const bool win_system;
  
public:
  /**Constructors & Destructor*/
  FileHandler() = default;
  explicit FileHandler(const std::string &filename);
  ~FileHandler();
  
  /**General*/
  const std::string &filename() const;
  bool open(std::string filename, bool relative_path = true);
  bool is_open() const;
  void close();
  Error checkLineFormat(Error (*checker_func)(std::stringstream &), const std::string &line) const;
  Error checkFile(Error (*checker_func)(std::stringstream &));
  
  /**File data exchange*/
  //Viene assunto che tutte le righe siano correttamente formattate. Qui vengono eseguiti controlli quali ripetizioni.
  Error fetchLineData(Error (*fetcher_func)(std::stringstream &, IOBuffer &), const std::string &line, IOBuffer &buff);
  Error fetchData(Error (*fetcher_func)(std::stringstream &, IOBuffer &), IOBuffer &buff);
  Error putData(std::string(*printer_func)(IOBuffer &), IOBuffer &to_add);
  Error putData(std::string(*printer_func)(IOBuffer &), IOBuffer &to_add, IOBuffer &to_delete);
  
};  //Class FileHandler
bool isFormatChar(const std::string &s, size_t pos);
std::string formatString(std::string str);           //Aggiunge i parser_char per proteggere i caratteri di formato
std::string unformatString(std::string str);
std::string readField(const std::string &field, const std::string &data);
Error checkField(const std::string &field, bool (*validity_checker)(const std::string&), unsigned int field_code, unsigned int error_if_invalid);

std::string formatOutput(const User &user);
std::string formatOutput(const Group &group);
std::string formatOutput(const Company &company);
std::string formatOutput(const IOBuffer::Relation &relation);
std::string formatOutput(const IOBuffer::m_Post &post);


//Controllo dei file
Error IDsfile(std::stringstream &line);
Error relationsFile(std::stringstream &line);
Error postsFile(std::stringstream &line);

//Acquisizione dei file
Error IDsfile(std::stringstream &line, IOBuffer &buff);
Error relationsFile(std::stringstream &line, IOBuffer &buff);
Error postsFile(std::stringstream &line, IOBuffer &buff);

//Stampa su file
std::string IDsfile(IOBuffer &buff);
std::string relationsFile(IOBuffer &buff);
std::string postsFile(IOBuffer &buff);

std::string error(const Error &err);

};//Namespace FH
#endif //SOCIAL_NETWORK_FILEHANDLER_H
