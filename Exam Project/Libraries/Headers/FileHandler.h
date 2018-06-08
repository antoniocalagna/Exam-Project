//
// Created by Alex on 06/06/2018.
//

#ifndef SOCIAL_NETWORK_FILEHANDLER_H
#define SOCIAL_NETWORK_FILEHANDLER_H

#include <string>
#include <fstream>
#include "Account.h"

namespace fh { //File handler;
//Traduce il codice errore nella stringa corrispondente
std::string error(unsigned int code);

std::string readField(const std::string &field_name, const std::string &raw_str);

unsigned int checkAccountsFile(std::ifstream &f);

} //Namespace fh

#endif //SOCIAL_NETWORK_FILEHANDLER_H
