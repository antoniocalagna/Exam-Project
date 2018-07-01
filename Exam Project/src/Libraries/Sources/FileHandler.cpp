//
// Created by Alex on 06/06/2018.
//

#include "FileHandler.h"

#ifdef _WIN32
const bool FH::FileHandler::win_system = true;
#elif __APPLE__
const bool FH::FileHandler::win_system = false;
#endif

/**#################
 * ## Static Data ##
 * #################
 */
const std::string FH::FileHandler::windows_relative_path = "../../input/";
const std::string FH::FileHandler::mac_relative_path = "../../../Exam Project/input/";
const char FH::FileHandler::parser_char = '&';
const char FH::FileHandler::format_chars[] = {'{', '}', '&', ','};

/**#####################
 * ## Private Methods ##
 * #####################
 */
void FH::FileHandler::_flushData(const std::unordered_set<std::string> &lines) {
  _file.close();
  _file.open(_filename, std::ios::out | std::ios::app);                //Riapri il file senza cancellarne i contenuti
  
  if (!_file.is_open()) {
    return;
  }
  
  for (auto it = lines.begin(); it != lines.end(); it++) {            //Stampa tutto il contenuto del set
    _file << *it << std::endl;
  }
}

std::unordered_set<std::string> FH::FileHandler::_getContent() {
  std::unordered_set<std::string> lines;
  
  _file.close();
  _file.open(_filename, std::ios::in);  //Riapri il file in lettura
  
  if (!_file.is_open()) {
    return lines;                       //Se il file non è aperto, ritorna il set vuoto
  }
  
  std::string line;
  while (_file.good()) {           //Acquisisci il file riga per riga
    std::getline(_file, line);
    if (!line.empty()) {           //Cancellando solo le righe vuote
      lines.insert(line);
    }
  }
  return lines;
}

bool FH::FileHandler::_lineHasData(const std::string &line) {
  if (line.empty())
    return false;                     //Riga vuota
  
  if (line.size() >= 2) {
    if (line.substr(0, 2) == "//") {
      return false;                   //Riga commento
    }
  }
  return true;
}

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
const std::string &FH::FileHandler::filename() const {
  return _filename;
}

bool FH::FileHandler::open(std::string filename, bool relative_path) {
  if (_file.is_open()) {
    close();
  }
  //Accesso al file tramite path relativo
  if (relative_path) {
    if (FH::FileHandler::win_system) {
      filename = windows_relative_path + filename;
    }
    else {
      filename = mac_relative_path + filename;
    }
  }
  filename += ".dat";
  _file.open(filename);
  _filename = filename;
  return _file.is_open();
}

bool FH::FileHandler::is_open() const {
  return _file.is_open();
}

void FH::FileHandler::close() {
  _filename.clear();
  _file.close();
}

FH::Error FH::FileHandler::checkFile() {
  if (!_file.is_open()) {
    return {0xFFFFFFFF, 0};
  }
  
  _file.close();
  _file.open(_filename, std::ios::in);
  std::string line;                   //Per acquisire il contenuto
  unsigned int current_line = 0;      //Per contare le righe ed eventualmente segnalare la posizione degli errori
  
  while (_file.good()) {
    std::getline(_file, line);
    
    if (_lineHasData(line)) {
      Error line_status = _checkLine(line);    //Analizza la riga con la funzione adatta
      
      if (line_status.code != 0)
        return {line_status.code, current_line + 1};                  //Errore! Ritorna il codice di errore ottenuto
      current_line++;
    }
  }
  
  return {0, current_line};
}

void FH::FileHandler::clear() {
  _file.close();
  _file.open(_filename, std::ios::out | std::ios::trunc);
  _file.flush();
}

void FH::FileHandler::fetchData(IOBuffer &buff) {
  _file.close();
  _file.open(_filename, std::ios::in);
  
  std::string line;
  while (_file.good()) {                //Scorri tutto il file:
    std::getline(_file, line);          //Acquisisci una riga
    if (_lineHasData(line)) {
      _fillBuffer(buff, line);          //Se è una riga di dati, processali e mettili nel buffer
    }
  }
}

void FH::FileHandler::putData(IOBuffer &to_add) {
  
  std::unordered_set<std::string> data_to_print = _emptyBuffer(to_add);         //Prepara i dati da stampare
  _flushData(data_to_print);                                                    //Stampali nel file
}

void FH::FileHandler::putData(IOBuffer &to_add, IOBuffer &to_delete) {
  std::unordered_set<std::string> data_to_skip = _emptyBuffer(to_delete);   //Prepara un container contenente le righe del file da non copiare
  std::unordered_set<std::string> data_to_print = _emptyBuffer(to_add);     //Questo container raccoglie tutte le righe del file
  
  if (data_to_skip.empty()) {                 //Se non ci sono dati da saltare, si può passare direttamente alla stampa
    _flushData(data_to_print);
    return;
  }
  
  std::unordered_set<std::string> data_on_file = _getContent();           //Copia il contenuto di tutto il file
  for (auto it = data_on_file.begin(); it != data_on_file.end(); it++) {
    data_to_print.insert(*it);                                            //I contenuti vecchi vanno stampati nuovamente
  }
  
  data_on_file.clear();                                                   //Questo set non ci serve più
  
  for (auto it = data_to_skip.begin();
       it != data_to_skip.end(); it++) {  //Elimina le righe da saltare dal set delle righe da copiare
    if (data_to_print.find(*it) != data_to_print.end()) {
      data_to_print.erase(*it);
    }
  }
  
  data_to_skip.clear();
  clear();                                                                //Cancella tutto il contenuto del file
  _flushData(data_to_print);                                              //Stampa on_file + to_print - to_skip
}
///////////////////////////////////////////////  NAMESPACE FH FUNCTIONS  ///////////////////////////////////////////////

std::string FH::formatString(std::string str) {
  for (size_t i = 0; i < str.size(); i++) {
    if (isFormatChar(str, i)) {
      std::string parser(1, FileHandler::parser_char);
      str.insert(i, parser);
      i++;
      /*i avanza di uno perchè se l'i-esimo carattere è di formato e viene inserito un carattere segnalatore prima di
      * esso, l'i-esimo carattere slitta di una posizione ed è necessario non considerarlo nuovamente. */
    }
  }
  return str;
}

std::string FH::unformatString(std::string str) {
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] == FileHandler::parser_char) {                       //Controlla se il carattere i-esimo è un parser
      str.erase(i, 1);
    }
  }
  return str;
}


bool FH::isFormatChar(const std::string &s, size_t pos) {
  //Controlla se il carattere nella posizione richiesta è un carattere di formato
  //Controlla innanzitutto che appaia nella lista dei caratteri di formato
  char c = s[pos];
  int format_char_n = sizeof(FileHandler::format_chars) / sizeof(FileHandler::format_chars[0]);
  bool found = false;
  for (int i = 0; i < format_char_n; i++) {
    if (c == FileHandler::format_chars[i])
      found = true;
  }
  if (!found)                        //Non presente nella lista
    return false;
  
  //Conta il numero di caratteri segnale che lo precedono:
  size_t count = 0;
  while (pos >= 1 && s[pos - 1] == FH::FileHandler::parser_char) {
    count++;
    pos--;
  }
  //Se i caratteri segnale sono in numero pari, si annullano a vicenda.
  return (count % 2 == 0);
}

std::string FH::readField(const std::string &field, const std::string &data) {
  size_t field_pos = data.find(field + ":{");                         //Ricerca del campo
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

FH::Error FH::checkField(const std::string &field,
                         bool (*validity_checker)(const string &),
                         unsigned int field_code,
                         unsigned int error_if_invalid) {
  field_code <<= 4 * (8 - 2);                                 //Il FieldCode è la seconda cifra di un numero esadecimale
  if (field.empty())
    return {unsigned(0x10000000) | field_code | error_if_invalid, 0};
  if (!validity_checker(field))
    return {unsigned(0x20000000) | field_code | error_if_invalid, 0};
  return {0, 0};
}

std::string FH::formatOutput(const User &user) {
  std::stringstream out;
  out << user.getID() << "," << Account::user_type << ",{"
      << "name:{" << user.getName()
      << "},surname:{" << user.getSurname()
      << "},gender:{" << user.getGender()
      << "},addr:{" << user.getAddress()
      << "},sub:{" << user.getSubscription()
      << "},birth:{" << user.getBirth() << "}}";
  return out.str();
}

std::string FH::formatOutput(const Group &group) {
  std::stringstream out;
  out << group.getID() << "," << Account::group_type
      << ",{" << "name:{" << group.getName()
      << "},location:{" << group.getLegalLocation()
      << "},activity:{" << group.getTypeOfActivity()
      << "},inception:{" << group.getInception().getDate()
      << "},sub:{" << group.getSubscription().getDate() << "}}";
  return out.str();
}

std::string FH::formatOutput(const Company &company) {
  std::stringstream out;
  out << company.getID() << "," << Account::company_type
      << ",{" << "name:{" << company.getName()
      << "},financial_loc:{" << company.getFinancialLocation()
      << "},operative_loc:{" << company.getOperativeLocation()
      << "},prod:{" << company.getTypeOfProduct()
      << "},inception:{" << company.getInception()
      << "},sub:{" << company.getSubscription() << "}}";
  return out.str();
}

std::string FH::formatOutput(const IOBuffer::Relation &relation) {
  std::stringstream out;
  std::string id1 = relation.first.first, id2 = relation.first.second,
          rel = relation.second;
  if (relation::isSymmetrical(rel)) {
    if (id1 > id2) { //Ordinali alfabeticamente dato che friendship è simmetrica
      std::swap(id1, id2);
    }
  }
  else if (!relation::isDominant(rel)) {
    //In questo caso la relazione è indicata come non dominante. Scambia gli ID e inverti la relazione
    std::swap(id1, id2);
    rel = relation::getInverse(rel);
  }
  out << id1 << "," << id2 << "," + rel;
  return out.str();
}

std::string FH::formatOutput(const IOBuffer::m_Post &post) {
  std::stringstream out;
  out << post.first << "," << formatString(post.second.getNews()) << ",";    //Formatta il messaggio
  
  out << post.second.getDate() << ",";                        //Formatta la data
  
  out << "likes:{";                                                  //Elabora la lista dei likes
  std::vector<std::string> temp = post.second.getLikes();
  for (int i = 0; i < temp.size(); i++) {
    out << temp[i];
    if (i !=
        temp.size() - 1)                                         //Metti la virgola soltanto se non sei all'ultimo like
      out << ",";
  }
  temp.clear();
  
  out << "},dislikes:{";                                              //Elabora la lista dei dislikes
  temp = post.second.getDislikes();
  for (int i = 0; i < temp.size(); i++) {
    out << temp[i];
    if (i != temp.size() - 1)
      out << ",";
  }
  
  out << "}";
  return out.str();
}