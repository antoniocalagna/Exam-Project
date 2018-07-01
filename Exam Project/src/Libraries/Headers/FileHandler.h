//
// Created by Alex on 06/06/2018.
//

#ifndef SOCIAL_NETWORK_FILEHANDLER_H
#define SOCIAL_NETWORK_FILEHANDLER_H

#include <fstream>
#include <sstream>
#include <unordered_set>
#include <deque>
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

protected:
  std::fstream _file;        //File
  std::string _filename;     //Nome del file
  
  /**Static data*/
  static const std::string windows_relative_path;
  static const std::string mac_relative_path;
  static const bool win_system;
  static std::deque<std::string> common_actions;
  
  /**Private functions*/
  void _flushData(const std::unordered_set<std::string> &lines);
  std::unordered_set<std::string> _getContent();
  static bool _lineHasData(const std::string &line);
  
  /**Virtual private functions*/
  virtual Error _checkLine(const std::string &line) const = 0;
  virtual std::unordered_set<std::string> _emptyBuffer(IOBuffer &buffer) const = 0;
  virtual void _fillBuffer(IOBuffer &buffer, const std::string &data) const = 0;
  
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
  void clear();
  
  /**File data exchange*/
  //Viene assunto che tutte le righe siano correttamente formattate. Qui vengono eseguiti controlli quali ripetizioni.
  Error checkFile();
  void fetchData(IOBuffer &buffer);
  void putData(IOBuffer &new_data);
  void putData(IOBuffer &new_data, IOBuffer &data_to_delete);
  
};  //Class FileHandler
bool isFormatChar(const std::string &s, size_t pos);
std::string formatString(std::string str);           //Aggiunge i parser_char per proteggere i caratteri di formato
std::string unformatString(std::string str);
std::string readField(const std::string &field, const std::string &data);
Error checkField(const std::string &field, bool (*validity_checker)(const std::string&), unsigned int field_code, unsigned int error_if_invalid);

};//Namespace FH
#endif //SOCIAL_NETWORK_FILEHANDLER_H
