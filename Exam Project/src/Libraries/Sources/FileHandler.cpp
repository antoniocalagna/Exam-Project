//
// Created by Alex on 06/06/2018.
//

#include "FileHandler.hpp"

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
  while (_file.good()) {                //Acquisisci il file riga per riga
    std::getline(_file, line);
    if (!line.empty()) {                //Cancellando solo le righe vuote
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
      filename = windows_relative_path + filename;    //Attacca in testa il percorso relativo
    }
    else {
      filename = mac_relative_path + filename;        //Attacca in testa il percorso nel caso del mac
    }
  }
  filename += ".dat";                                 //Incoda l'estensione
  _file.open(filename);                               //Apri il file
  _filename = filename;                               //Memorizza il nome del file
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
  _file.open(_filename, std::ios::in);  //Apri il file in lettura
  std::string line;                     //Per acquisire il contenuto
  unsigned int current_line = 0;        //Per contare le righe ed eventualmente segnalare la posizione degli errori
  
  while (_file.good()) {
    std::getline(_file, line);
    
    if (_lineHasData(line)) {
      Error line_status = _checkLine(line);     //Analizza la riga con la funzione (virtuale in generale)
      
      if (line_status.code != 0)                        //Se c'è un codice di errore...
        return {line_status.code, current_line + 1};    //Errore! Ritorna il codice di errore ottenuto
      current_line++;                           //Incrementa il contatore driga
    }
  }
  return {0, current_line};             //Arrivati in fondo, non sono stati trovati errori
}

void FH::FileHandler::clear() {
  _file.close();
  _file.open(_filename, std::ios::out |
                        std::ios::trunc);   //Riapri il file in scrittura in modalità trunc (cancellando il contenuto)
  _file.flush();                              //Salva le modifiche nel file
}

void FH::FileHandler::deleteData(IOBuffer &data_to_delete) {
  IOBuffer empty_buffer;                  //Buffer temporaneo che non contiene dati
  putData(empty_buffer, data_to_delete);  //Chiama la funzione senza aggiungere nulla di nuovo
}

void FH::FileHandler::fetchData(IOBuffer &buff) {
  _file.close();
  _file.open(_filename, std::ios::in);  //Apri il file in lettura
  
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
    if (isFormatChar(str, i)) {                           //Controlla se il carattere è uguale ad un carattere di formato o no
      std::string parser(1, FileHandler::parser_char);    //In questo caso crea una stringa lunga 1 contenente il carattere parser
      str.insert(i, parser);                              //Inserisci il carattere parser prima di quello di falso-formato
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
      str.erase(i, 1);                                              //In tal caso eliminalo
    }
  }
  return str;
}


bool FH::isFormatChar(const std::string &s, size_t pos) {
  //Controlla se il carattere nella posizione richiesta è un carattere di formato
  //Controlla innanzitutto che appaia nella lista dei caratteri di formato
  char c = s[pos];                                          //Il carattere
  int format_char_n = sizeof(FileHandler::format_chars) /
                      sizeof(FileHandler::format_chars[0]); //Numero di caratteri di formato
  bool found = false;                                       //Flag
  for (int i = 0; i < format_char_n; i++) {                 //Controlla se il carattere è uno qualunque tra quelli di formato
    if (c == FileHandler::format_chars[i])
      found = true;
  }
  if (!found)                                               //Non presente nella lista
    return false;
  
  /*Se il carattere è preceduto da un parser, potrebbe essere un falso allarme. Ad esempio
   * ,    è un carattere di formato
   * &,   non è un carattere di formato, perchè ce lo segnala la &
   * &&,  è un carattere di formato, in quanto la & è "annullata" dall'altra &. */
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
  if (data_end == std::string::npos)                                  //Se non trovi la }, il campo è incompleto o mal formato
    return "";
  return data.substr(data_beg, data_end - data_beg);                  //Estrai il contenuto del campo
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