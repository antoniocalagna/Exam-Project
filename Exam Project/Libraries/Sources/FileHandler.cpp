//
// Created by Alex on 06/06/2018.
//

#include "FileHandler.h"
#include <iostream>

std::string fh::getScopeContent(const std::string &scope) {
  std::string out = "";
  if (scope[0] != '{' || scope[scope.size() - 1] != '}')
    return out;
  return scope.substr(1, scope.size() - 2);
}

int fh::checkAccountsFile(std::ifstream &f) {
  //Controlla se il file è aperto
  if (!f.is_open()) {
    return 0xFFFFFFFF;
  }
  
  //Riporta il cursore di get all'inizio e controlla che il file sia leggibile
  f.seekg(0);
  if (!f.good()) {
    return 0xFFFFFFFF;
  }
  
  while (f.good()) {
    //Acquisisci l'ID
    std::string id;
    std::getline(f, id, ',');         //L'ID non contiene virgole
    if (!f.good()) {
      //Problema di formato
      return 0x00000001;
    }
    
    //Acquisici il tipo di account
    std::string type;
    std::getline(f, type, ',');
    if (!f.good()) {
      //Problema di formato
      return 0x00000001;
    }
  }
}