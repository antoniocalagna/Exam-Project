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

FH::Error FH::FileHandler::checkLineFormat(Error (*checker_func)(std::stringstream &),
                                           const std::string &line) {
  std::stringstream line_s(line);
  return checker_func(line_s);
}

FH::Error FH::FileHandler::checkFile(Error (*checker_func)(std::stringstream &)) {
  if (!_file.is_open()) {
    return {0xFFFFFFFF, 0};
  }
  std::string line;                   //Per acquisire il contenuto
  unsigned int current_line = 1;      //Per contare le righe ed eventualmente segnalare la posizione degli errori
  
  while (_file.good()) {
    std::getline(_file, line);
    
    FH::Error line_status = checkLineFormat(checker_func, line);    //Analizza la riga con la funzione adatta
    if (line_status.code != 0)
      return {line_status.code, current_line};                      //Errore! Ritorna il codice di errore ottenuto
    current_line++;
  }
  return {0, current_line};
}

User FH::FileHandler::flushUser() {
  if (_user_buffer.empty()) { return User(); }  //Controlla che la queue non sia vuota
  User temp = _user_buffer.front();
  _user_buffer.pop();
  return temp;
}

Group FH::FileHandler::flushGroup() {
  if (_group_buffer.empty()) { return Group(); }
  Group temp = _group_buffer.front();
  _group_buffer.pop();
  return temp;
}

Company FH::FileHandler::flushCompany() {
  if (_company_buffer.empty()) { return Company(); }
  Company temp = _company_buffer.front();
  _company_buffer.pop();
  return temp;
}

///////////////////////////////////////////////  NAMESPACE FH FUNCTIONS  ///////////////////////////////////////////////
bool FH::isFormatChar(const std::string &s, size_t pos) {
  size_t count = 0;
  while (pos >= 1 && s[pos - 1] == FH::FileHandler::parser_char) {
    count++;
    pos--;
  }
  //Se i caratteri segnale sono in numero pari, si annullano a vicenda.
  return (count % 2 == 0);
}

std::string FH::readField(const std::string &field, const std::string &data) {
  size_t field_pos = data.find(field + ":");                          //Ricerca del campo
  if (field_pos == std::string::npos) { return ""; }                  //Campo non presente
  
  size_t data_beg = field_pos + 2 + field.size();                     //Il dato comincia dopo i :{ e la size di field
  size_t data_end;
  do {                                                                //Cerca la prima '}' che sia valida
    data_end = data.find('}', field_pos + 1);                         //(guardando dal carattere successivo a '}')
  } while (!isFormatChar(data, data_end) && data_end != std::string::npos);
  if (data_end == std::string::npos)
    return "";
  return data.substr(data_beg, data_end - data_beg);
}

FH::Error FH::IDsfile(std::stringstream &line) {
  std::string id, type_s;
  //Acquisisci e controlla l'ID
  std::getline(line, id, ',');
  if (!line.good()) { return {0x11000000, 0}; }                //Errore nella lettura dell'ID
  if (!Account::IDValid(id)) { return {0x21000000, 0}; }        //L'ID non rispetta le regole
  //Acquisisci e controlla il tipo di account
  std::getline(line, type_s, ',');
  if (!line.good()) { return {0x12000000, 0}; }                //Errore nella lettura del tipo di account
  if (type_s.size() != 1 || !Account::typeValid(type_s[0])) {      //Errore nel tipo di account
    return {0x22000000, 0};
  }
  char type = type_s[0];
  
  //Acquisici e controlla il resto delle informazioni (che dipendono dal tipo di account)
  std::string info;
  std::getline(line, info);                                     //Acquisisci il resto delle informazioni
  info = info.substr(1, info.size() - 2);                       //Liberati delle parentesi graffe
  if (type == Account::user_type) {
    //L'account è un utente
    std::string name, surname, address;
    char gender;
    Date birth, subscription;
    name = readField("name", name);
    if (name.empty()) { return {0x13000001, 0}; }
    surname = readField("surname", info);
    if (surname.empty()) { return {0x13000002, 0}; }
    address = readField("address", info);
    if(address.empty()) { return {0x13000004, 0}; }
  }
  else if (type == Account::group_type) {
    //L'account è un gruppo
  }
  else if (type == Account::company_type) {
    //L'account è una compagnia
  }
  return {0, 0};
}