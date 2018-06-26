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
const std::string FH::FileHandler::windows_relative_path = "../Files/";
const std::string FH::FileHandler::mac_relative_path = "../../../Exam Project/Files/";
const char FH::FileHandler::parser_char = '&';
const char FH::FileHandler::format_chars[] = {'{', '}', '&', ','};

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

FH::Error FH::FileHandler::checkLineFormat(Error (*checker_func)(std::stringstream &),
                                           const std::string &line) const {
  if (line.empty()) {
    return {0, 0};     //Ignora le righe vuote
  }
  
  if (line.size() >= 2) {
    if (line.substr(0, 2) == "//") {
      return {0, 0};    //Riga commento
    }
  }
  std::stringstream line_s(line);
  return checker_func(line_s);
}

FH::Error FH::FileHandler::checkFile(Error (*checker_func)(std::stringstream &)) {
  if (!_file.is_open()) {
    return {0xFFFFFFFF, 0};
  }
  
  _file.close();
  _file.open(_filename, std::ios::in);
  std::string line;                   //Per acquisire il contenuto
  unsigned int current_line = 0;      //Per contare le righe ed eventualmente segnalare la posizione degli errori
  
  while (_file.good()) {
    std::getline(_file, line);
    
    Error line_status = checkLineFormat(checker_func, line);    //Analizza la riga con la funzione adatta
    if (line_status.code != 0)
      return {line_status.code, current_line + 1};                  //Errore! Ritorna il codice di errore ottenuto
    current_line++;
  }
  return {0, current_line};
}

FH::Error FH::FileHandler::fetchLineData(Error (*fetcher_func)(stringstream &, IOBuffer &), const std::string &line,
                                         IOBuffer &buff) {
  if (line.empty()) {
    return {0, 0};     //Ignora le righe vuote
  }
  
  if (line.size() > 2) {
    if (line.substr(0, 2) == "//") {
      return {0, 0};    //Riga commento
    }
  }
  std::stringstream line_s(line);
  return fetcher_func(line_s, buff);
}

FH::Error FH::FileHandler::fetchData(Error (*fetcher_func)(stringstream &, IOBuffer &), IOBuffer &buff) {
  _file.close();
  _file.open(_filename, std::ios::in);                             //Riapri il file in modalità IN
  std::string line_to_parse;
  
  unsigned int current_line = 0;
  while (_file.good()) {                                            //Cicla attraverso tutto il file
    std::getline(_file, line_to_parse);
    Error err = fetchLineData(fetcher_func, line_to_parse, buff);
    if (err.code != 0)
      return {err.code, current_line + 1};
    
    current_line++;
  }
  return {0, current_line};
}

FH::Error FH::FileHandler::putData(std::string (*printer_func)(IOBuffer &), IOBuffer &to_add, IOBuffer &to_delete) {
  std::string data_to_skip = printer_func(to_delete);   //Prepara una stringa contenente le righe del file da non copiare
  std::stringstream data_to_copy;
  _file.close();
  _file.open(_filename, std::ios::out | std::ios::app);
  
  std::string data_to_add = printer_func(to_add);
  _file << data_to_add << std::endl;
  
  _file.close();
  _file.open(_filename, std::ios::in);                  //Apri il file in modalità lettura
  
  //Fai una copia dell'intero file in memoria
  while (_file.good()) {
    std::string line;
    std::getline(_file, line);
    if (data_to_skip.find(line) == std::string::npos) {  //Copia solo le righe che non siano da saltare
      data_to_copy << line << std::endl;
    }
  }
  data_to_skip.clear();                                 //Libera la memoria
  
  _file.close();
  _file.open(_filename, std::ios::out | std::ios::trunc); //Riapri il file cancellando tutti i contenuti vecchi
  _file << data_to_copy.str();
  _file.flush();
  return {0, 0};
}

FH::Error FH::FileHandler::putData(std::string (*printer_func)(IOBuffer &), IOBuffer &to_add) {
  IOBuffer empty_buffer;
  return putData(printer_func, to_add, empty_buffer);
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
      << "},inception:{" << company.getInception()
      << "},prod:{" << company.getTypeOfProduct()
      << "},sub:{" << company.getSubscription() << "}}";
  return out.str();
}

std::string FH::formatOutput(const IOBuffer::Relation &relation) {
  std::stringstream out;
  out << relation.first.first << "," << relation.first.second << "," + relation.second;
  return out.str();
}

std::string FH::formatOutput(const IOBuffer::m_Post &post) {
  std::stringstream out;
  out << post.first << "," << formatString(post.second.getNews());    //Formatta il messaggio
  
  out << "," << post.second.getDate() <<  ",";                        //Formatta la data
  
  out << ",likes:{";                                                  //Elabora la lista dei likes
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

/**##############
 * ## Checkers ##
 * ##############
 */
FH::Error FH::accountsFile(std::stringstream &line) {
  std::string id, type_s;
  FH::Error check_result;
  //Acquisisci e controlla l'ID
  std::getline(line, id, ',');                                  //Acquisisci l'ID
  check_result = check_result = checkField(id, Account::IDValid, 0x1, 0x1);
  if(check_result.code != 0)
    return check_result;                //Controllalo
  
  //Acquisisci e controlla il tipo di account
  std::getline(line, type_s, ',');                              //Acquisisci il tipo
  check_result = check_result = checkField(type_s, Account::typeValid, 0x2, 0x1);
  if(check_result.code != 0)
    return check_result;
  char type = type_s[0];
  type_s.clear();                                               //Libera la stringa
  
  //Acquisici e controlla il resto delle informazioni (che dipendono dal tipo di account)
  std::string info;
  std::getline(line, info);                                     //Acquisisci il resto delle informazioni
  info = info.substr(1, info.size() - 2);                       //Liberati delle parentesi graffe
  if (type == Account::user_type) {
    //L'account è un utente
    std::string name, surname, address, gender;
    std::string birth, subscription;
    
    name = readField("name", info);                             //Controllo nome
    check_result = checkField(name, Account::nameValid, 0x3, 0x1);
    if(check_result.code != 0)
      return check_result;
    surname = readField("surname", info);                       //Controllo cognome
    check_result = checkField(surname, Account::nameValid, 0x3, 0x2);
    if(check_result.code != 0)
      return check_result;
    gender = readField("gender", info);                         //Controllo genere
    check_result = checkField(gender, Account::nameValid, 0x3, 0x3);
    if(check_result.code != 0)
      return check_result;
    address = readField("addr", info);                          //Controllo indirizzo
    check_result = checkField(address, Account::nameValid, 0x3, 0x4);
    if(check_result.code != 0)
      return check_result;
    subscription = readField("sub", info);                      //Controllo data di iscrizione
    check_result = checkField(subscription, Date::CheckDate, 0x3, 0x5);
    if(check_result.code != 0)
      return check_result;
    birth = readField("birth", info);                           //Controllo data di nascita
    check_result = checkField(birth, Date::CheckDate, 0x3, 0x6);
    if(check_result.code != 0)
      return check_result;
  }
  else if (type == Account::group_type) {
    //L'account è un gruppo
    std::string name, location, activity;
    std::string inception, subscription;
    
    name = readField("name", info);                             //Controllo nome
    check_result = checkField(name, Account::nameValid, 0x3, 0x1);
    if(check_result.code != 0)
      return check_result;
    location = readField("location", info);                     //Controllo posizione legale
    check_result = checkField(location, Account::nameValid, 0x3, 0x4);
    if(check_result.code != 0)
      return check_result;
    activity = readField("activity", info);                     //Controllo attività
    check_result = checkField(activity, Account::nameValid, 0x3, 0x7);
    if(check_result.code != 0)
      return check_result;
    inception = readField("inception", info);                   //Controllo data di creazione del gruppo
    check_result = checkField(inception, Date::CheckDate, 0x3, 0x8);
    if(check_result.code != 0)
      return check_result;
    subscription = readField("sub", info);                     //Controllo data di iscrizione
    check_result = checkField(subscription, Date::CheckDate, 0x3, 0x5);
    if(check_result.code != 0)
      return check_result;
  }
  else if (type == Account::company_type) {
    //L'account è una compagnia
    std::string name, f_location, op_location, prod;
    std::string inception, subscription;
    
    name = readField("name", info);                                     //Controllo nome
    check_result = checkField(name, Account::nameValid, 0x3, 0x1);
    if(check_result.code != 0)
      return check_result;
    prod = readField("prod", info);                                     //Controllo prodotto
    check_result = checkField(prod, Account::nameValid, 0x3, 0xA);
    if(check_result.code != 0)
      return check_result;
    f_location = readField("financial_loc", info);                      //Controllo della finantial location
    check_result = checkField(f_location, Account::nameValid, 0x3, 0x4);
    if(check_result.code != 0)
      return check_result;
    op_location = readField("operative_loc", info);                     //Controllo della operative location
    check_result = checkField(f_location, Account::nameValid, 0x3, 0x4);
    if(check_result.code != 0)
      return check_result;
    inception = readField("inception", info);              //Controllo data di creazione della compagnia
    check_result = checkField(inception, Date::CheckDate, 0x3, 0x8);
    if(check_result.code != 0)
      return check_result;
    subscription = readField("sub", info);                 //Controllo data di iscrizione
    check_result = checkField(subscription, Date::CheckDate, 0x3, 0x5);
    if(check_result.code != 0)
      return check_result;
  }
  return {0, 0};
}

FH::Error FH::relationsFile(std::stringstream &line) {
  std::string id1, id2, relation;
  
  std::getline(line, id1, ',');                                         //Controllo del primo ID
  if (!line.good()) return {0x11000000, 0};
  if (!Account::IDValid(id1)) return {0x21000000, 0};
  
  std::getline(line, id2, ',');                                         //Controllo del secondo ID
  if (!line.good()) return {0x11000000, 0};
  if (!Account::IDValid(id2)) return {0x21000000, 0};
  
  std::getline(line, relation);                                         //Controllo della relazione
  if (!relation::belong(relation)) return {0x24000000, 0};
  return {0, 0};
}

FH::Error FH::postsFile(std::stringstream &line) {
  std::string id, message;
  std::string like, dislike;
  std::string date;
  FH::Error check_result;
  
  std::getline(line, id, ',');                                        //Acquisici l'ID
  check_result = checkField(id, Account::IDValid, 0x1, 0x1);
  if(check_result.code != 0)
    return check_result;
  
  std::getline(line, message, ',');
  while (!isFormatChar(line.str(), message.size() + id.size() + 1)) {
    std::string temp;
    std::getline(line, temp, ',');
    message += "," + temp;
  }
  if (!line.good()) { return {0x13000009, 0}; }                       //Errore di formattazione del messaggio
  
  std::getline(line, date, ',');
  check_result = checkField(date, Date::CheckDate, 0x3, 0xB);
  if(check_result.code != 0)
    return check_result;
  
  std::string reactions;
  std::getline(line, reactions);                                      //Acquisici il resto della riga
  std::stringstream likes_ss(readField("likes", reactions));          //Metti i likes in uno stringstream
  while (likes_ss.good() && likes_ss.gcount() != 0) {
    std::getline(likes_ss, like, ',');
    check_result = checkField(like, Account::IDValid, 0x1, 0x1);
    if(check_result.code != 0)
      return check_result;
  }
  
  std::stringstream dislikes_ss(readField("dislikes", reactions));   //Metti i dislikes in uno stringstream
  while (dislikes_ss.good() && dislikes_ss.gcount() != 0) {
    std::getline(dislikes_ss, dislike);
    check_result = checkField(dislike, Account::IDValid, 0x1, 0x1);
    if(check_result.code != 0)
      return check_result;
  }
  return {0, 0};
}

/**#################
 * ## Acquisition ##
 * #################
 */
FH::Error FH::accountsFile(std::stringstream &line, IOBuffer &buff) {
  std::string id;
  char type;
  
  std::getline(line, id, ',');                                  //Acquisici l'ID
  line.get(type);                                               //Acquisisci il tipo
  line.ignore(1);                                               //Salta la virgola
  
  if (buff.replicatedID(id))                                     //Controlla i duplicati
    return {0x31000000, 0};
  
  //Acquisici il resto delle informazioni (che dipendono dal tipo di account)
  std::string info;
  std::getline(line, info);                                     //Acquisisci il resto delle informazioni
  info = info.substr(1, info.size() - 2);                       //Liberati delle parentesi graffe
  if (type == Account::user_type) {
    //L'account è un utente
    std::string name, surname, address, gender;
    Date birth, subscription;
    
    name = readField("name", info);                             //Acquisici il nome
    surname = readField("surname", info);                       //Acquisisci il cognome
    address = readField("addr", info);                          //Acquisisci l'indirizzo
    gender = readField("gender", info);                         //Acquisisci il genere
    birth.scanDateByStr(readField("birth", info));              //Acquisisci la data di nascita
    subscription.scanDateByStr(readField("sub", info));         //Acquisisci la data di iscrizione
    
    User new_user(name, surname, id, address, subscription, birth, gender[0]);
    buff << new_user;
  }
  else if (type == Account::group_type) {
    //L'account è un gruppo
    std::string name, location, activity;
    Date inception, subscription;
    
    name = readField("name", info);                                     //Acquisizione nome
    location = readField("location", info);                             //Acquisizione posizione legale
    activity = readField("activity", info);
    inception.scanDateByStr(readField("inception", info));              //Acquisizione data di creazione del gruppo
    subscription.scanDateByStr(readField("sub", info));                 //Acquisizione data di iscrizione
    
    Group new_group(name, id, location, activity, subscription, inception);
    buff << new_group;
    
  }
  else if (type == Account::company_type) {
    //L'account è una compagnia
    std::string name, f_location, op_location, prod;
    Date inception, subscription;
    
    name = readField("name", info);                                     //Acquisizione nome
    prod = readField("prod", info);                                     //Acquisizione prodotto
    f_location = readField("financial_loc", info);                      //Acquisizione della finantial location
    op_location = readField("operative_loc", info);                     //Acquisizione della operative location
    inception.scanDateByStr(readField("inception", info));              //Acquisizione data di creazione della compagnia
    subscription.scanDateByStr(readField("sub", info));                 //Acquisizione data di iscrizione
    
    Company new_company(name, id, f_location, op_location, prod, subscription, inception);
    buff << new_company;
  }
  return {0, 0};
}

FH::Error FH::relationsFile(std::stringstream &line, IOBuffer &buff) {
  std::string id1, id2, relation;
  
  std::getline(line, id1, ',');                                         //Acquisizione del primo ID
  std::getline(line, id2, ',');                                         //Acquisizione del secondo ID
  std::getline(line, relation);                                         //Acquisizione della relazione
  
  IOBuffer::Relation new_rel = {{id1, id2}, relation};                  //Prevenzione delle sovrascritture
  if (buff.overwritingRelation(new_rel))
    return {0x34000001, 0};
  buff << new_rel;
  return {0, 0};
}

FH::Error FH::postsFile(std::stringstream &line, IOBuffer &buff) {
  std::string id, message;
  std::string like, dislike;
  std::string date_str;
  Post new_post;
  
  std::getline(line, id, ',');                                          //Acquisici l'ID
  
  std::getline(line, message, ',');                                     //Acquisisci il messaggio
  while (!isFormatChar(line.str(), message.size() + id.size() + 1)) {
    std::string temp;
    std::getline(line, temp, ',');
    message += "," + temp;
  }
  message = unformatString(message);                                    //De-formatta il messaggio
  new_post.setNews(message);
  
  std::getline(line, date_str, ',');                                    //Acquisici la data
  new_post.setDate_Time(date_str);
  
  std::string reactions;
  std::getline(line, reactions);                                      //Acquisici il resto della riga
  std::stringstream likes_ss(readField("likes", reactions));          //Metti i likes in uno stringstream
  while (likes_ss.good() && likes_ss.gcount() != 0) {
    std::getline(likes_ss, like, ',');
    new_post.AddLike(like);
  }
  
  std::stringstream dislikes_ss(readField("dislikes", reactions));   //Metti i dislikes in uno stringstream
  while (dislikes_ss.good() && dislikes_ss.gcount() != 0) {
    std::getline(dislikes_ss, dislike);
    new_post.AddDislike(dislike);
  }
  buff << std::pair<std::string, Post>(id, new_post);
  return {0, 0};
}

/**##############
 * ## Printers ##
 * ##############
 */
std::string FH::accountsFile(IOBuffer &buff) {
  std::stringstream out;                      //Prepara uno stringstream per poi trasformarlo in stringa
  while (!buff.usersEmpty()) {
    User usr;
    buff >> usr;                              //Acquisisci l'utente dal buffer
    out << formatOutput(usr) << std::endl;    //Aggiungilo all'output
  }
  while (!buff.groupsEmpty()) {
    Group group;
    buff >> group;                            //Acquisisci il gruppo
    out << formatOutput(group) << std::endl;
  }
  while (!buff.companiesEmpty()) {
    Company company;
    buff >> company;                          //Acquisisci la compagnia
    out << formatOutput(company) << std::endl;
  }
  return out.str();                           //Converti lo stringstream in string e ritorna
}

std::string FH::relationsFile(IOBuffer &buff) {
  std::stringstream out;
  while (!buff.relationsEmpty()) {
    IOBuffer::Relation rel;
    buff >> rel;
    out << formatOutput(rel);
  }
  return out.str();
}

std::string FH::postsFile(IOBuffer &buff) {
  std::stringstream out;
  while (!buff.postsEmpty()) {
    IOBuffer::m_Post post;
    buff >> post;
    out << formatOutput(post);
  }
  return out.str();
}