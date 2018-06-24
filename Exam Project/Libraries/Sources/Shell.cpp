//
// Created by Alex on 24/06/2018.
//

#include "Shell.h"

void Shell::get(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  //GET: Interfaccia per ottenere informazioni e dati
  std::string what_to_get;
  command >> what_to_get;
  //Get info
  if (what_to_get == "info") {
    std::string requested_id;
    command >> requested_id;
    char account_type = manager.getAccountType(requested_id);   //Controlla il tipo di account di cui sono richieste le informazioni
    if (account_type == 0) {
      std::cout << "Requested ID not found" << std::endl;
      return;
    }
      //Get info - user
    else if (account_type == Account::user_type) {
      User user = manager.getUser(requested_id);
      std::cout << "Name:" << user.getName() << "\n"
                << "Surname: " << user.getSurname() << "\n"
                << "Gender: " << user.getGender() << "\n"
                << "Address: " << user.getAddress() << "\n"
                << "Birth Date :" << user.getBirth() << "\n"
                << "Subscription Date: " << user.getSubscription() << std::endl;
      
    }
      //Get info - group
    else if (account_type == Account::group_type) {
      Group group = manager.getGroup(requested_id);
      std::cout << "Name: " << group.getName() << "\n"
                << "Legal location: " << group.getLegalLocation() << "\n"
                << "Activity: " << group.getTypeOfActivity() << "\n"
                << "Inception: " << group.getInception() << "\n"
                << "Birth Date: " << group.getSubscription() << std::endl;
    }
      //Get info - company
    else if (account_type == Account::company_type) {
      Company company = manager.getCompany(requested_id);
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
    command >> id1 >> id2;
    std::string relation = manager.getRelationship(id1, id2);
    if (relation.empty()) {                                               //Non è presente alcuna relazione
      std::cout << "No relation found between " << id1 << " and  " << id2;
      return;
    }
    std::cout << "Relation: " << relation << std::endl;
  }
    
    //Get posts
  else if (what_to_get == "posts") {
    std::string id;
    command >> id;
    std::vector<Post> posts = manager.getPosts(id);
    if (posts.empty()) {
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
        if (!likes.empty()) {
          std::cout << "Likes: ";
          for (int j = 0; j < likes.size(); j++) {
            std::cout << likes[i];
            if (j != likes.size() - 1)
              std::cout << ",";
          }
          std::cout << std::endl;
        }
        if (!dislikes.empty()) {
          std::cout << "Disikes: ";
          for (int j = 0; j < dislikes.size(); j++) {
            std::cout << dislikes[j];
            if (j != dislikes.size() - 1)
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
    std::cout << "Possible parameters: info <id>, relation <id1> <id2>, posts <id>";
    return;
  }
}
