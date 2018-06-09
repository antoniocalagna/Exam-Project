//
// Created by Alex on 06/06/2018.
//

#ifndef SOCIAL_NETWORK_FILEHANDLER_H
#define SOCIAL_NETWORK_FILEHANDLER_H

#include <string>
#include <fstream>
#include "Account.h"

namespace fh { //File handler;

//Srtruttura utilizzata come ritorno delle funzioni per fornire informazioni più complete
struct Error {
  unsigned int code;
  int info;
};

//Ritorna la cifra richiesta in un numero esadecimale (La cifra 1 è quella più a sinistra
unsigned char getHexDigit(unsigned int number, unsigned int digit_pos);

//Traduce il codice errore nella stringa corrispondente
std::string error(unsigned int code);

//Legge un campo formattato come "nome_campo:{......}" ignorando le parentesi graffe
std::string readField(const std::string &field_name, const std::string &raw_str);

Error checkAccountsFile(std::ifstream &f);
} //Namespace fh

#endif //SOCIAL_NETWORK_FILEHANDLER_H
