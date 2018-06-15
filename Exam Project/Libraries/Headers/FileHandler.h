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
  static const char parser_char;

private:
  std::fstream _file;        //File
  std::string _filename;     //Nome del file
  
  /**Static data*/
  static const std::string windows_relative_path;
  static const std::string mac_relative_path;
  static const char format_chars[];
  
  /**Private functions*/
  std::string _formatString(const std::string &line);           //Aggiunge i parser_char per proteggere i caratteri di formato
public:
  /**Constructors & Destructor*/
  FileHandler() = default;
  explicit FileHandler(const std::string &filename);
  ~FileHandler();
  
  /**General*/
  const std::string &filename() const;
  bool open(std::string filename, bool windows_system = true, bool direct_access = true);
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

//Acquisizione dei file

//Stampa su file

};//Namespace FH
#endif //SOCIAL_NETWORK_FILEHANDLER_H 
