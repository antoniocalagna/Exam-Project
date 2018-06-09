//
// Created by Alex on 06/06/2018.
//

#include "FileHandler.h"
#include <iostream>

unsigned char fh::getHexDigit(unsigned int number, unsigned int digit_pos) {
  digit_pos = 7 - digit_pos + 1;              //La cifra 1 è la cifra più a sinistra
  if (digit_pos > 7) {                    //Un int ha massimo 8 cifre
    return 16;
  }
  number >>= digit_pos * 4;               //Shifta 4 bit per cifra
  number &= 0x0000000F;                   //Azzera tutti i bit antecedenti
  return (unsigned char)(number);
}

std::string fh::error(unsigned int code) {
  std::string err;
  //Controlli di base:
  switch (code) {
    case 0:
      err = "File formattato correttamente.";
      break;
    case 0xFFFFFFFF:
      err = "File non apribile o danneggiato";
      break;
    default:
      err = "Errore sconosciuto";
      break;
  }
  return err;
}

std::string fh::readField(const std::string &field_name, const std::string &raw_str) {
  size_t field_pos = raw_str.find(field_name);      //Ricerca del campo
  if (field_pos == std::string::npos) {             //Il campo non è presente
    return "";
  }
  if (field_pos != 0) {                            //La stringa contiene altri dati. Per scelta non devono esserci.
    return "";
  }
  
  //Non considerare il nome del campo, i : e le parentesi graffe
  return raw_str.substr(field_name.size() + 2, raw_str.size() - (field_name.size() + 3));
}

fh::Error fh::checkAccountsFile(std::ifstream &f) {
  //Controlla se il file è aperto
  if (!f.is_open()) {
    return {0xFFFFFFFF, 0};
  }
  
  //Riporta il cursore di get all'inizio e controlla che il file sia leggibile
  f.seekg(0);
  if (!f.good()) {
    return {0xFFFFFFFF, 0};                                          //Errore nel file
  }
  
  unsigned int line = 0;
  while (f.good()) {
    //Acquisisci l'ID
    std::string id;
    std::getline(f, id, ',');                                   //L'ID non contiene virgole
    if (!f.good()) { return {0x10000000, line}; }
    if (!Account::IDValid(id)) { return {0x21000000, line}; }
    
    //Acquisici il tipo di account
    std::string type;
    std::getline(f, type, ',');
    if (!f.good()) { return {0xF0000001, line}; }                       //Problema di formato
    if (type.size() != 1 || !Account::typeValid(type[0])) {
      return {0x10000001, line};                                        //Tipo non valido
    }
    
    //Raccogli tutte le informazioni sull'utente
    std::string info;
    std::getline(f, info, '}');     //Qui bisogna fermarsi alla graffa
    if (!f.good()) { return {0xF0000001, line}; }
    
    line++;
  }
  return {0, 0};
}
