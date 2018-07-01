//
// Created by Alex on 01/07/2018.
//

#include "Social_Handlers.h"

/////////////////////////////////////                ACCOUNTS                     //////////////////////////////////////
FH::Error FH::AccountsHandler::_checkLine(const std::string &line) const {            //Controllo del file
  std::string id, type_s;
  std::stringstream line_ss(line);
  FH::Error check_result {0,0};
  //Acquisisci e controlla l'ID
  std::getline(line_ss, id, ',');                                  //Acquisisci l'ID
  check_result = check_result = checkField(id, Account::IDValid, 0x1, 0x1);
  if (check_result.code != 0)
    return check_result;                //Controllalo
  
  //Acquisisci e controlla il tipo di account
  std::getline(line_ss, type_s, ',');                              //Acquisisci il tipo
  check_result = check_result = checkField(type_s, Account::typeValid, 0x2, 0x1);
  if (check_result.code != 0)
    return check_result;
  char type = type_s[0];
  type_s.clear();                                               //Libera la stringa
  
  //Acquisici e controlla il resto delle informazioni (che dipendono dal tipo di account)
  std::string info;
  std::getline(line_ss, info);                                     //Acquisisci il resto delle informazioni
  info = info.substr(1, info.size() - 2);                       //Liberati delle parentesi graffe
  if (type == Account::user_type) {
    //L'account è un utente
    std::string name, surname, address, gender;
    std::string birth, subscription;
    
    name = readField("name", info);                             //Controllo nome
    check_result = checkField(name, Account::nameValid, 0x3, 0x1);
    if (check_result.code != 0)
      return check_result;
    surname = readField("surname", info);                       //Controllo cognome
    check_result = checkField(surname, Account::nameValid, 0x3, 0x2);
    if (check_result.code != 0)
      return check_result;
    gender = readField("gender", info);                         //Controllo genere
    check_result = checkField(gender, Account::nameValid, 0x3, 0x3);
    if (check_result.code != 0)
      return check_result;
    address = readField("addr", info);                          //Controllo indirizzo
    check_result = checkField(address, Account::nameValid, 0x3, 0x4);
    if (check_result.code != 0)
      return check_result;
    subscription = readField("sub", info);                      //Controllo data di iscrizione
    check_result = checkField(subscription, Date::CheckDate, 0x3, 0x5);
    if (check_result.code != 0)
      return check_result;
    birth = readField("birth", info);                           //Controllo data di nascita
    check_result = checkField(birth, Date::CheckDate, 0x3, 0x6);
    if (check_result.code != 0)
      return check_result;
  }
  else if (type == Account::group_type) {
    //L'account è un gruppo
    std::string name, location, activity;
    std::string inception, subscription;
    
    name = readField("name", info);                             //Controllo nome
    check_result = checkField(name, Account::nameValid, 0x3, 0x1);
    if (check_result.code != 0)
      return check_result;
    location = readField("location", info);                     //Controllo posizione legale
    check_result = checkField(location, Account::nameValid, 0x3, 0x4);
    if (check_result.code != 0)
      return check_result;
    activity = readField("activity", info);                     //Controllo attività
    check_result = checkField(activity, Account::nameValid, 0x3, 0x7);
    if (check_result.code != 0)
      return check_result;
    inception = readField("inception", info);                   //Controllo data di creazione del gruppo
    check_result = checkField(inception, Date::CheckDate, 0x3, 0x8);
    if (check_result.code != 0)
      return check_result;
    subscription = readField("sub", info);                     //Controllo data di iscrizione
    check_result = checkField(subscription, Date::CheckDate, 0x3, 0x5);
    if (check_result.code != 0)
      return check_result;
  }
  else if (type == Account::company_type) {
    //L'account è una compagnia
    std::string name, f_location, op_location, prod;
    std::string inception, subscription;
    
    name = readField("name", info);                                     //Controllo nome
    check_result = checkField(name, Account::nameValid, 0x3, 0x1);
    if (check_result.code != 0)
      return check_result;
    prod = readField("prod", info);                                     //Controllo prodotto
    check_result = checkField(prod, Account::nameValid, 0x3, 0xA);
    if (check_result.code != 0)
      return check_result;
    f_location = readField("financial_loc", info);                      //Controllo della finantial location
    check_result = checkField(f_location, Account::nameValid, 0x3, 0x4);
    if (check_result.code != 0)
      return check_result;
    op_location = readField("operative_loc", info);                     //Controllo della operative location
    check_result = checkField(f_location, Account::nameValid, 0x3, 0x4);
    if (check_result.code != 0)
      return check_result;
    inception = readField("inception", info);              //Controllo data di creazione della compagnia
    check_result = checkField(inception, Date::CheckDate, 0x3, 0x8);
    if (check_result.code != 0)
      return check_result;
    subscription = readField("sub", info);                 //Controllo data di iscrizione
    check_result = checkField(subscription, Date::CheckDate, 0x3, 0x5);
    if (check_result.code != 0)
      return check_result;
  }
  return {0, 0};
}

std::unordered_set<std::string> FH::AccountsHandler::_emptyBuffer(IOBuffer &buffer) const {
  std::unordered_set<std::string> out;          //Prepara uno stringstream per poi trasformarlo in stringa
  while (!buffer.usersEmpty()) {
    User usr;
    buffer >> usr;                              //Acquisisci l'utente dal buffer
    out.insert(formatOutput(usr));              //Aggiungilo all'output
  }
  while (!buffer.groupsEmpty()) {
    Group group;
    buffer >> group;                            //Acquisisci il gruppo
    out.insert(formatOutput(group));
  }
  while (!buffer.companiesEmpty()) {
    Company company;
    buffer >> company;                          //Acquisisci la compagnia
    out.insert(formatOutput(company));
  }
  return out;                                   //Converti lo stringstream in string e ritorna
}

void FH::AccountsHandler::_fillBuffer(IOBuffer &buffer, const std::string &data) const {
  std::string id;
  std::stringstream line(data);
  char type;
  
  std::getline(line, id, ',');                                  //Acquisici l'ID
  line.get(type);                                               //Acquisisci il tipo
  line.ignore(1);                                               //Salta la virgola
  
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
    buffer << new_user;
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
    buffer << new_group;
    
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
    buffer << new_company;
  }
}


/////////////////////////////////////               RELATIONS                    //////////////////////////////////////
FH::Error FH::RelationsHandler::_checkLine(const std::string &line) const {
  std::stringstream line_ss(line);
  std::string id1, id2, relation;
  FH::Error status{0, 0};
  
  std::getline(line_ss, id1, ',');                                         //Controllo del primo ID
  status = checkField(id1, Account::IDValid, 0x1, 0x0);
  if(status.code != 0) {
    return status;
  }
  
  std::getline(line_ss, id2, ',');                                         //Controllo del secondo ID
  status = checkField(id2, Account::IDValid, 0x1, 0x0);
  if(status.code != 0) {
    return status;
  }
  
  std::getline(line_ss, relation);                                         //Controllo della relazione
  status = checkField(relation, relation::isValid, 0x4, 0x0);
  if(status.code != 0) {
    return status;
  }
  
  return {0, 0};
}

std::unordered_set<std::string> FH::RelationsHandler::_emptyBuffer(IOBuffer &buffer) const {
  std::unordered_set<std::string> out;
  while (!buffer.relationsEmpty()) {
    IOBuffer::Relation rel;
    buffer >> rel;
    out.insert(formatOutput(rel));
  }
  return out;
}

void FH::RelationsHandler::_fillBuffer(IOBuffer &buffer, const std::string &data) const {
  std::string id1, id2, relation;
  std::stringstream line(data);
  
  std::getline(line, id1, ',');                                         //Acquisizione del primo ID
  std::getline(line, id2, ',');                                         //Acquisizione del secondo ID
  std::getline(line, relation);                                         //Acquisizione della relazione
  
  IOBuffer::Relation new_rel = {{id1, id2}, relation};                  //Prevenzione delle sovrascritture
  buffer << new_rel;
}

/////////////////////////////////////                  POSTS                      //////////////////////////////////////
FH::Error FH::PostsHandler::_checkLine(const std::string &line) const {
  std::stringstream line_ss(line);
  std::string id, message;
  std::string like, dislike;
  std::string date;
  FH::Error check_result {0, 0};
  
  std::getline(line_ss, id, ',');                                        //Acquisici l'ID
  check_result = checkField(id, Account::IDValid, 0x1, 0x1);
  if (check_result.code != 0)
    return check_result;
  
  std::getline(line_ss, message, ',');
  while (!isFormatChar(line_ss.str(), message.size() + id.size() + 1)) {
    std::string temp;
    std::getline(line_ss, temp, ',');
    message += "," + temp;
  }
  if (!line_ss.good()) { return {0x13000009, 0}; }                       //Errore di formattazione del messaggio
  
  std::getline(line_ss, date, ',');
  check_result = checkField(date, Date::CheckDate, 0x3, 0xB);
  if (check_result.code != 0)
    return check_result;
  
  std::string reactions;
  std::getline(line_ss, reactions);                                      //Acquisici il resto della riga
  std::stringstream likes_ss(readField("likes", reactions));          //Metti i likes in uno stringstream
  while (likes_ss.good() && likes_ss.gcount() != 0) {
    std::getline(likes_ss, like, ',');
    check_result = checkField(like, Account::IDValid, 0x1, 0x1);
    if (check_result.code != 0)
      return check_result;
  }
  
  std::stringstream dislikes_ss(readField("dislikes", reactions));   //Metti i dislikes in uno stringstream
  while (dislikes_ss.good() && dislikes_ss.gcount() != 0) {
    std::getline(dislikes_ss, dislike);
    check_result = checkField(dislike, Account::IDValid, 0x1, 0x1);
    if (check_result.code != 0)
      return check_result;
  }
  return {0, 0};
}

std::unordered_set<std::string> FH::PostsHandler::_emptyBuffer(IOBuffer &buffer) const {
  std::unordered_set<std::string> out;
  while (!buffer.postsEmpty()) {
    IOBuffer::m_Post post;
    buffer >> post;
    out.insert(formatOutput(post));
  }
  return out;
}

void FH::PostsHandler::_fillBuffer(IOBuffer &buffer, const std::string &data) const {
  std::stringstream line(data);
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
  size_t ch_count = likes_ss.str().size();
  while (likes_ss.good() && ch_count != 0) {
    std::getline(likes_ss, like, ',');
    new_post.AddLike(like);
  }
  
  std::stringstream dislikes_ss(readField("dislikes", reactions));   //Metti i dislikes in uno stringstream
  ch_count = dislikes_ss.str().size();
  while (dislikes_ss.good() /*&& dislikes_ss.gcount() != 0*/&& ch_count != 0) {
    std::getline(dislikes_ss, dislike, ',');
    new_post.AddDislike(dislike);
  }
  buffer << std::pair<std::string, Post>(id, new_post);
}


/////////////////////////////////////            LIBRARY FUNCTIONS                //////////////////////////////////////

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