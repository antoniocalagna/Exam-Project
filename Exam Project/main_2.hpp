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
  FH::FileHandler accountsFH("Accounts_TEST.dat"),
          relationsFH("Relations_TEST.dat"),
          postsFH("Posts_TEST.dat");
  std::set<std::string> commands = {"help", "get", "set", "list", "exit"};
  
  std::cout << "Beginning checks:" << std::endl;
  //Controllo del file degli account
  FH::Error check_results = accountsFH.checkFile(FH::accountsFile);
  if (check_results.code == 0) {
    std::cout << "Accounts file correctly formatted and ready to be read." << std::endl;
  }
  else {
    std::cerr << "Accounts file returned error code " << check_results.code << " at line " << check_results.data << "."
              << "File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  //Controllo del file delle relazioni
  check_results = relationsFH.checkFile(FH::relationsFile);
  if (check_results.code == 0) {
    std::cout << "Relations file correctly formatted and ready to be read." << std::endl;
  }
  else {
    std::cerr << "Relations file returned error code " << check_results.code << " at line " << check_results.data << "."
              << "File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  //Controllo del file dei post
  check_results = postsFH.checkFile(FH::postsFile);
  if (check_results.code == 0) {
    std::cout << "Posts file correctly formatted and ready to be read." << std::endl;
  }
  else {
    std::cerr << "Posts file returned error code " << check_results.code << " at line " << check_results.data << "."
              << "File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  //Controlli completati
  std::cout << "\n\n" << std::endl;
  
  //Messaggio di avvio
  std::cout << "Welcome. This is program is console based, so commands must be typed in console-style."
            << "\nType \"help\" for a list of useful commands.";
  
  bool exit = false;
  do {
    //Acquisici il comando dell'utente
    std::stringstream user_command;
    std::string user_input;
    do {
      std::getline(std::cin, user_input);
    } while (user_input.empty());         //Salta le righe vuote
    
    std::string command;
    user_command >> command;              //Nome del comando da eseguire
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
        //Get relation
      else if (what_to_get == "relation") {
        std::string id1, id2;
        std::cout << "Insert the two IDs: " << std::endl;
        std::cout << ">";
        std::cin >> id1 >> id2;
        std::string relation = network.getRelationship(id1, id2);
        if (relation.empty()) {
          std::cout << "No relation found between " << id1 << " and  " << id2;
        }
        else {
          std::cout << "Relation: " << relation << std::endl;
        }
      }
        
        //Get posts
      else if (what_to_get == "posts") {
        std::cout << "Insert the ID from which posts should be retrieved: " << std::endl;
        std::string id;
        user_command >> id;
        std::vector<Post> posts = network.getPosts(id);
        if (posts.size() == 0) {
          std::cout << "No posts from " << id << " found" << std::endl;
        }
        else {
          for (int i = 0; i < posts.size(); i++) {
            std::cout << i + 1 << ":" << std::endl                 //Indice del post
                      << "\n" << posts[i].getNews() << "\n" << std::endl
                      << "[" << posts[i].getDate() << "]" << std::endl;
            std::vector<std::string> likes, dislikes;
            likes = posts[i].getLikes();
            dislikes = posts[i].getDislikes();
            if (likes.size() != 0) {
              std::cout << "Likes: ";
              for (int i = 0; i < likes.size(); i++) {
                std::cout << likes[i];
                if (i != likes.size() - 1)
                  std::cout << ",";
              }
              std::cout << std::endl;
            }
            if (dislikes.size() != 0) {
              std::cout << "Disikes: ";
              for (int i = 0; i < dislikes.size(); i++) {
                std::cout << dislikes[i];
                if (i != dislikes.size() - 1)
                  std::cout << ",";
              }
              std::cout << std::endl;
            }
          }
        }
      }
        
        //Get <something_not_valid>
      else {
        std::cout << "Cannot get \"" << what_to_get << "\"" << std::endl;
        std::cout << "Possible parameters:";
      }
    }
    else if (command == "exit") {
      exit = true;
    }
    else {
      std::cout << "Command \"" << command << "\" unknown." << std::endl;
      std::cout << "Type \"help\" for a list of useful commands." << std::endl;
    }
    user_command.clear();
  } while (!exit);
  return 0;
}

std::string promptInput() {
  std::string input;
  std::cout << ">";
  std::cin >> input;
  return input;
}

#endif //SOCIAL_NETWORK_MAIN_2_HPP
