//
// Created by Alex on 06/06/2018.
//

#ifndef SOCIAL_NETWORK_FILEHANDLER_H
#define SOCIAL_NETWORK_FILEHANDLER_H

#include <string>
#include <fstream>

namespace fh { //File handler

//Ritorna il contenuto di una stringa
std::string getScopeContent(const std::string &scope);

int checkAccountsFile(std::ifstream &f);

} //Namespace fh

#endif //SOCIAL_NETWORK_FILEHANDLER_H
