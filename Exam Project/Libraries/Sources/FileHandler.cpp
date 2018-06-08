//
// Created by Alex on 06/06/2018.
//

#include "FileHandler.h"
#include <iostream>

std::string fh::error(int code) {
  std::string err;
  switch (code) {
    //Format or integrity problems
    case 0:
      err = "File formattato correttamente.";
      break;
    case 0xFFFFFFFF:
      err = "File non aperto o danneggiato.";
      break;
    case 0xF0000001:
      err = "Errore di formato.";
      break;
      
      //Data problems
    case 0x10000001:
      err = "ID non valido.";
      break;
    
    default:
      err = "Errore " + std::to_string(code) + " sconosciuto.";
      break;
  }
  return err;
}

std::string fh::readField(const std::string &field_name, const std::string &raw_str) {
  size_t field_pos = raw_str.find(field_name);  //Ricerca del campo
  if (field_pos == std::string::npos) {          //Il campo non è presente
    return "";
  }
  if (field_pos != 0) {                          //La stringa contiene altri dati. Per scelta non devono esserci.
    return "";
  }
  
  //Non considerare il nome del campo, i : e le parentesi graffe
  return raw_str.substr(field_name.size() + 2, raw_str.size() - (field_name.size() + 3));
}

int fh::checkAccountsFile(std::ifstream &f) {
  //Controlla se il file è aperto
  if (!f.is_open()) {
    return 0xFFFFFFFF;
  }
  
  //Riporta il cursore di get all'inizio e controlla che il file sia leggibile
  f.seekg(0);
  if (!f.good()) {
    return 0xFFFFFFFF;                                          //Errore nel file
  }
  
  while (f.good()) {
    //Acquisisci l'ID
    std::string id;
    std::getline(f, id, ',');                                   //L'ID non contiene virgole
    if (!f.good()) { return 0xF0000001; }
    if (!Account::IDValid(id)) { return 0x10000001; }
    
    //Acquisici il tipo di account
    std::string type;
    std::getline(f, type, ',');
    if (!f.good()) { return 0xF0000001; }                       //Problema di formato
    if (type.size() != 1 || !Account::typeValid(type[0])) {
      return 0x10000001;                                        //ID non valido
    }
    
    //Raccogli tutte le informazioni sull'utente
    std::string info;
    std::getline(f, info, '}');     //Qui bisogna fermarsi alla graffa
    if (!f.good()) { return 0xF0000001; }
  }
}