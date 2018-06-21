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
    } while (user_input.empty());
    
    std::string command;
    user_command >> command;
    if (command == "help") {
      //Stampa una lista dei comandi possibili ed una breve spiegazione
      
    }
    else if (command == "get") {
      //GET: Interfaccia per ottenere informazioni e dati
      std::string what_to_get;
      user_command >> what_to_get;
      //Get info
      if (what_to_get == "info") {
        std::cout << "Insert ID:" << std::endl;
        std::string requested_id = promptInput();
        char account_type = network.getAccountType(requested_id);   //Controlla il tipo di account di cui sono richieste le informazioni
        if (account_type == 0) {
          std::cout << "Requested ID not found" << std::endl;
        }
          //Get info - user
        else if (account_type == Account::user_type) {
          User user = network.getUser(requested_id);
          std::cout << "Name:" << user.getName() << "\n"
                    << "Surname: " << user.getSurname() << "\n"
                    << "Gender: " << user.getGender() << "\n"
                    << "Address: " << user.getAddress() << "\n"
                    << "Birth Date :" << user.getBirth() << "\n"
                    << "Subscription Date: " << user.getSubscription() << std::endl;
          
        }
          //Get info - group
        else if (account_type == Account::group_type) {
          Group group = network.getGroup(requested_id);
          std::cout << "Name: " << group.getName() << "\n"
                    << "Legal location: " << group.getLegalLocation() << "\n"
                    << "Activity: " << group.getTypeOfActivity() << "\n"
                    << "Inception: " << group.getInception() << "\n"
                    << "Birth Date: " << group.getSubscription() << std::endl;
        }
          //Get info - company
        else if (account_type == Account::company_type) {
          Company company = network.getCompany(requested_id);
          std::cout << "Name: " << company.getName() << "\n"
                    << "Financial location: " << company.getFinancialLocation() << "\n"
                    << "Operative location: " << company.getOperativeLocation() << "\n"
                    << "Product: " << company.getTypeOfProduct() << "\n"
                    << "Inception: " << company.getInception() << "\n"
                    << "Birth Date: " << company.getSubscription() << std::endl;
        }
      }
      else if (what_to_get == "relation") {
        std::cout << "Insert the two IDs: " << std::endl;
      }
      else if (what_to_get == "posts") {
      
      }
      else {
        std::cout << "Cannot get \"" << what_to_get << "\"" << std::endl;
        std::cout << "Possible parameters:";
      }
    }
    
    else {
      std::cout << "Command \"" << command << "\" unknown." << std::endl;
      std::cout << "Type \"help\" for a list of useful commands." << std::endl;
    }
    user_command.clear();
  } while (!exit);
}

std::string promptInput() {
  std::string input;
  std::cout << ">";
  std::cin >> input;
  return input;
}

#endif //SOCIAL_NETWORK_MAIN_2_HPP
