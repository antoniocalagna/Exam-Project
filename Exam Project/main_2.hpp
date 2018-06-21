//
// Created by Alex on 21/06/2018.
//

#ifndef SOCIAL_NETWORK_MAIN_2_HPP
#define SOCIAL_NETWORK_MAIN_2_HPP

#include "Manager.h"
#include "FileHandler.h"

std::string promptInput();

int main_2(int argc, char *argv[]) {
  Manager network;
  FH::FileHandler accountsFH,
          relationsFH,
          postsFH;
  std::set<std::string> commands = {"help", "get", "set", "list", "exit"};
  
  std::cout << "Welcome. This is program is console based, so commands must be typed in console-style."
            << "\nType \"help\" for a list of useful commands.";
  
  bool exit = false;
  do {
    std::stringstream user_command;
    std::string user_input;
    do {
      std::getline(std::cin, user_input);
    } while(user_input.empty());
    
    std::string command;
    user_command >> command;
    if(command == "help") {
      //Stampa una lista dei comandi possibili ed una breve spiegazione
      
    }
    else if (command == "get") {
      //GET: Interfaccia per ottenere informazioni e dati
      std::string what_to_get;
      user_command >> what_to_get;
      if(what_to_get == "info") {
        std::cout << "Insert ID:" << std::endl;
        std::string requested_id = promptInput();
        char account_type = network.getAccountType(requested_id);
        if(account_type == 0) {
          std::cout << "Requested ID not found" << std::endl;
        }
        else if (account_type == Account::user_type) {
          User user = network.getUser(requested_id);
          std::cout << "Name:" << user.getName() << "\n"
                  << "Surname:" << user.getSurname() << "\n"
                  << "Name:" << user.getName() << "\n"
                  << "Name:" << user.getName() << "\n"
                  << "Name:" << user.getName() << "\n"
        }
      }
      else if (what_to_get == "relation") {
      
      }
      else if (what_to_get == "post") {
      
      }
      else {
        std::cout << "Cannot get \"" << what_to_get << "\"" << std::endl;
        std::cout << "Possible parameters:"
      }
    }
    
    else {
      std::cout << "Command \""<< command << "\" unknown." << std::endl;
    }
    user_command.clear();
  } while(!exit);
}

std::string promptInput() {
  std::string input;
  std::cout << ">";
  std::cin >> input;
  return input;
}

#endif //SOCIAL_NETWORK_MAIN_2_HPP
