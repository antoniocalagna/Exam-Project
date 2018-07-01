//
// Created by Alex on 06/06/2018.
//

#ifndef SOCIAL_NETWORK_FILEHANDLER_H
#define SOCIAL_NETWORK_FILEHANDLER_H

#include <fstream>
#include <sstream>
#include <map>
#include <unordered_set>
#include "IOBuffer.h"

namespace FH {

struct Error {
  unsigned int code;
  unsigned int data;
};

class FileHandler {
public:
  /**Static data */
  static const char parser_char;    //Carattere che segnala quando un carattere di formato è un "falso"
  static const char format_chars[]; //Caratteri di formato

protected:
  std::fstream _file;        //File
  std::string _filename;     //Nome del file
  
  /**Static data*/
  static const std::string windows_relative_path;   //Percorso relativo ai file per un sistema windows
  static const std::string mac_relative_path;       //Percorso relativo per un mac
  static const bool win_system;                     //Bool che segnala il tipo di sistema
  
  /**Protected functions*/
  void _flushData(const std::unordered_set<std::string> &lines);    //Scarica i dati richiesti nel file
  std::unordered_set<std::string> _getContent();                    //Carica in memoria il contenuto del file
  static bool
  _lineHasData(const std::string &line);                //Controlla che una linea non sia vuota o di commento
  
  /**Virtual protected functions*/
  virtual Error
  _checkLine(const std::string &line) const = 0;                        //Controlla la correttezza formale di una riga
  virtual std::unordered_set<std::string>
  _emptyBuffer(IOBuffer &buffer) const = 0;   //Trasforma un buffer in un set di stringhe
  virtual void
  _fillBuffer(IOBuffer &buffer, const std::string &data) const = 0;      //Trasforma una stringa nel dato corrispondente
  
public:
  /**Constructors & Destructor*/
  FileHandler() = default;
  explicit FileHandler(const std::string &filename);
  ~FileHandler();
  
  /**General*/
  const std::string &filename() const;
  bool open(std::string filename, bool relative_path = true);     //Apri il file
  bool is_open() const;                                           //Controlla che sia aperto
  void close();                                                   //Chiudi il file
  void clear();                                                   //Cancella il contenuto del file
  
  /**File data exchange*/
  //Viene assunto che tutte le righe siano correttamente formattate. Qui vengono eseguiti controlli quali ripetizioni.
  Error checkFile();                                            //Controlla la correttezza formale del file
  void fetchData(IOBuffer &buffer);                             //Acquisisci i dati e mettili in un buffer
  void deleteData(IOBuffer &data_to_delete);
  void putData(IOBuffer &new_data);                             //Stampa i dati contenuti in un buffer
  void putData(IOBuffer &new_data, IOBuffer &data_to_delete);   //Stampa i dati saltandone alcuni
};  //Class FileHandler

bool isFormatChar(const std::string &s, size_t pos);  //Controlla che il carattere in posizione pos sia di formato
std::string formatString(std::string str);            //Aggiunge i parser_char per proteggere i caratteri di formato
std::string unformatString(std::string str);          //Rimuove i parser_cher per rendere una stringa leggibile
std::string readField(const std::string &field, const std::string &data);   //Legge un campo salvato come campo:{<dato>}
Error checkField(const std::string &field,
                 bool (*validity_checker)(const std::string &),
                 unsigned int field_code, unsigned int error_if_invalid);
  
};//Namespace FH
#endif //SOCIAL_NETWORK_FILEHANDLER_H
