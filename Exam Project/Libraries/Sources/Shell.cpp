//
// Created by Alex on 24/06/2018.
//

#include "Shell.h"

void Shell::help(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  //LISTA DEI POSSIBILI COMANDI
  cout << "list - Prints a list of all accounts" << endl;
  cout << "Users:" << endl;
  cout
          << "\nset user name\nset user surname\nset user gender\nset user birth\nset user address\nset user id\nset user subscription\n"
          << endl
          << "get user info\n"
          << endl;
  cout << "add account user\n"
       << endl;
  
  cout << "Groups:" << endl;
  cout
          << "\nset name <group_id>\nset n_members <group_id>\nset location <group_id>\nset type_activity <group_id>\nset inception <group_id>\nset id <group_id>\nset subscription <group_id>\n"
          << endl
          << "get group info\n"
          << endl;
  cout << "add account group\n"
       << endl;
  
  cout << "Companies:" << endl;
  cout
          << "\nset name <company_id>\nset finantial_location <company_id>\nset operative_location <company_id>\nset products <company_id>\nset inception <company_id>\nset id <company_id>\nset subscription <company_id>\n"
          << endl
          << "get company info\n"
          << endl;
  cout << "add account company\n"
       << endl;
  
  cout << "delete account <id>\n"
       << endl;
  
  cout << "add relationship\n"
          "delete relationship\n"
       << endl;
  cout << "add post\n"
          "delete post\n"
       << endl;
  cout << "add like\n"
          "delete like\n"
          "add dislike\n"
          "delete dislike\n"
       << endl;
  cout << "stats number accounts\n" //DA UNIRE
          "stats number users\n"
          "stats number groups\n"
          "stats number companies\n"
          "stats number friends\n"
          "stats number relatives\n"
          "stats number employees\n"
          "stats number subsidiaries\n"
          "stats number members\n"
          "stats number born_after\n"
       << endl;
  cout << "most employing_company\n" //DA UNIRE
          "most employing_partnership\n"
          "most user_friends\n"
          "most user_acquaintances\n"
          "most liked_post\n"
          "most disliked_post\n"
          "most liked_account\n"
          "most disliked_account\n"
       << endl;
  cout << "average_age\n"
          "best_post\n"
          "worse_post\n";
}

void Shell::list(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  //Stampa una lista di account
  std::vector<Account> accounts(manager.getAllAccounts());
  if (accounts.empty()) {
    std::cout << "No accounts found." << std::endl;
    return;
  }
  
  //Ordina il vettore per stampare gli account in base al loro tipo
  std::sort(accounts.begin(),
            accounts.end(),
            [](const Account &account1, const Account &account2) {
              return account1.getType() > account2.getType();
            });
  for (int i = 0; i < accounts.size(); i++) {
    std::cout << accounts[i].getID() << "\t" << accounts[i].getType() << std::endl;
  }
}

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

void Shell::set(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  
  string what_to_set, who_to_set; //non so se questi nomi sono messi per scherzo, a me sembrano carini
  command >> who_to_set >> what_to_set;
  if (what_to_set.empty()) {
    cout << "Error! I do not understand what information you'd like to set." << endl;
    return;
  }
  if (who_to_set == "user") {
    string id_to_set;
    User user_to_set; //user a cui vuoi cambiare info
    User user_new;
    cout << "Please insert the ID whose User you'd like to set info about:\n>" << endl;
    cin >> id_to_set; //id "vecchio" account
    user_to_set = manager.getUser(id_to_set); //se l'id non esiste, user_to_set è un default constructor
    if (user_to_set == User()) {
      cout << "Error! This ID is not valid." << endl;
    }
    else {
      if (what_to_set == "name") {
        string new_name;
        user_new = user_to_set;
        cout << "Please insert the new" << what_to_set << "." << endl;
        //cin.ignore(); da provare se ci vada o no
        getline(cin, new_name);
        user_new.setName(new_name);
        
        if (!manager.replaceAccount(id_to_set, user_new)) {             //Il manager non è riuscuto a rimpiazzare l'account
          cout << "Error! " << what_to_set << " could not be modified." << endl;
          return;
        }
        
        data_to_delete << user_to_set;
        new_data << user_new;
        cout << "Done!" << endl;
      }
      
      else if (what_to_set == "surname") {
        string new_surname;
        cout << "Please insert the new" << what_to_set << "." << endl;
        //cin.ignore();
        getline(cin, new_surname);
        user_to_set.setSurname(new_surname);
        if (!manager.replaceAccount(id_to_set, user_to_set)) {
          cout << "Error! " << what_to_set << " could not be modified." << endl;
        }
        else {
          cout << "Done!" << endl;
        }
        
      }
      else if (what_to_set == "gender") {
        char new_gender;
        cout << "Please insert the new gender." << endl;
        //cin.ignore();
        cin >> new_gender;
        user_to_set.setGender(new_gender);
        if (!manager.replaceAccount(id_to_set, user_to_set)) {
          cout << "Error! " << what_to_set << " could not be modified." << endl;
        }
        else {
          cout << "Done!" << endl;
        }
        
      }
      else if (what_to_set == "birth") {
        string new_birth;
        Date birth;
        cout << "Please insert the new" << what_to_set << "." << endl;
        //cin.ignore();
        getline(cin, new_birth);
        if (birth.CheckDate(new_birth)) {
          birth.scanDateByStr(new_birth);
          user_to_set.setBirth(birth);
          if (!manager.replaceAccount(id_to_set, user_to_set)) {
            cout << "Error! " << what_to_set << " could not be modified." << endl;
          }
          else {
            cout << "Done!" << endl;
          }
        }
        else {
          cout << "Error! Date is not valid." << endl;
        }
        
      }
      else if (what_to_set == "address") {
        string new_addr;
        cout << "Please insert the new address." << endl;
        //cin.ignore();
        getline(cin, new_addr);
        user_to_set.setAddress(new_addr);
        if (!manager.replaceAccount(id_to_set, user_to_set)) {
          cout << "Error! " << what_to_set << " could not be modified." << endl;
        }
        else {
          cout << "Done!" << endl;
        }
      }
      
      else if (what_to_set == "subscription") {
        string new_sub;
        Date sub;
        cout << "Please insert the new date of subscription (in format dd/mm/yyyy)." << endl;
        //cin.ignore();
        getline(cin, new_sub);
        if (sub.CheckDate(new_sub)) {
          sub.scanDateByStr(new_sub);
          user_to_set.setSubscription(new_sub);
          if (!manager.replaceAccount(id_to_set, user_to_set)) {
            cout << "Error! " << what_to_set << " could not be modified." << endl;
          }
          else {
            cout << "Done!" << endl;
          }
          
        }
        else {
          cout << "Error! This date is not valid." << endl;
        }
      }
    }
  }
  else if (who_to_set == "group") {
    string id_to_set;
    Group group_to_set;
    cout << "Please insert the ID whose Group you'd like to set info about:\n>" << endl;
    cin >> id_to_set; //id "vecchio" account
    group_to_set = manager.getGroup(id_to_set);
    if (group_to_set == Group()) {                                  //Il manager non ha trovato il gruppo
      cout << "Error! This ID is not valid." << endl;
      return;
    }
    if (what_to_set == "name") {
      string new_name;
      cout << "Please insert the new name." << endl;
      //cin.ignore(); da provare se ci vada o no
      getline(cin, new_name);
      group_to_set.setName(new_name);
      if (!manager.replaceAccount(id_to_set, group_to_set)) { //da provare
        cout << "Error! " << what_to_set << " could not be modified." << endl;
      }
      else {
        cout << "Done!" << endl;
      }
    }
    
    else if (what_to_set == "subscription") {
      string new_sub;
      Date sub;
      cout << "Please insert the new date of subscription (in format dd/mm/yyyy)." << endl;
      //cin.ignore();
      getline(cin, new_sub);
      if (sub.CheckDate(new_sub)) {
        sub.scanDateByStr(new_sub);
        group_to_set.setSubscription(new_sub);
        if (!manager.replaceAccount(id_to_set, group_to_set)) {
          cout << "Error! This ID already exists." << endl;
        }
        else {
          cout << "Done!" << endl;
        }
      }
      else {
        cout << "Error! This date is not valid." << endl;
      }
      
    }
    else if (what_to_set == "location") {
      string new_loc;
      cout << "Please insert the new location." << endl;
      //cin.ignore(); da provare se ci vada o no
      getline(cin, new_loc);
      group_to_set.setLegalLocation(new_loc);
      if (!manager.replaceAccount(id_to_set, group_to_set)) { //da provare
        cout << "Error! " << what_to_set << " could not be modified." << endl;
      }
      else {
        cout << "Done!" << endl;
      }
      
    }
    else if (what_to_set == "type_activity") {
      string new_act;
      cout << "Please insert the new type of activity." << endl;
      //cin.ignore(); da provare se ci vada o no
      getline(cin, new_act);
      group_to_set.setTypeOfActivity(new_act);
      
      if (!manager.replaceAccount(id_to_set, group_to_set)) { //da provare
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
      cout << "Done!" << endl;
    }
    else if (what_to_set == "inception") {
      string new_inc;
      Date inc;
      cout << "Please insert the new date of inception (in format dd/mm/yyyy)." << endl;
      //cin.ignore(); da provare se ci vada o no
      getline(cin, new_inc);
      if (inc.CheckDate(new_inc)) {
        inc.scanDateByStr(new_inc);
        group_to_set.setInception(inc);
        if (!manager.replaceAccount(id_to_set, group_to_set)) { //da provare
          cout << "Error! " << what_to_set << " could not be modified." << endl;
        }
        else {
          cout << "Done!" << endl;
        }
      }
      else {
        cout << "Error! The date is not valid." << endl;
      }
      
    }
  }
  else if (who_to_set == "company") {
    string id_to_set;
    Company company_to_set;
    cout << "Please insert the ID whose Company you'd like to set info about:\n>" << endl;
    cin >> id_to_set; //id "vecchio" account
    company_to_set = manager.getCompany(id_to_set);
    if (company_to_set == Company()) {
      cout << "Error! This ID is not valid." << endl;
    }
    
    if (what_to_set == "name") {
      string new_name;
      cout << "Please insert the new name." << endl;
      //cin.ignore(); da provare se ci vada o no
      getline(cin, new_name);
      company_to_set.setName(new_name);
      if (!manager.replaceAccount(id_to_set, company_to_set)) { //da provare
        cout << "Error! " << what_to_set << " could not be modified." << endl;
      }
      else {
        cout << "Done!" << endl;
      }
      
    }
    
    else if (what_to_set == "subscription") {
      string new_sub;
      Date sub;
      cout << "Please insert the new date of subscription (in format dd/mm/yyyy)." << endl;
      //cin.ignore();
      getline(cin, new_sub);
      if (sub.CheckDate(new_sub)) {
        sub.scanDateByStr(new_sub);
        company_to_set.setSubscription(new_sub);
        if (!manager.replaceAccount(id_to_set, company_to_set)) {
          cout << "Error! This ID already exist." << endl;
        }
        else {
          cout << "Done!" << endl;
        }
      }
      else {
        cout << "Error! This date is not valid." << endl;
      }
    }
    else if (what_to_set == "inception") {
      string new_inc;
      Date inc;
      cout << "Please insert the new date of inception (in format dd/mm/yyyy)." << endl;
      //cin.ignore(); da provare se ci vada o no
      getline(cin, new_inc);
      if (inc.CheckDate(new_inc)) {
        inc.scanDateByStr(new_inc);
        company_to_set.setInception(inc);
        if (!manager.replaceAccount(id_to_set, company_to_set)) {
          cout << "Error! " << what_to_set << " could not be modified." << endl;
          return;
        }
        cout << "Done!" << endl;
      }
      else {
        cout << "Error! This date is not valid." << endl;
      }
      
    }
    else if (what_to_set == "financial_location") {
      string new_loc;
      cout << "Please insert the new financial location." << endl;
      //cin.ignore(); da provare se ci vada o no
      getline(cin, new_loc);
      company_to_set.setFinancialLocation(new_loc);
      if (!manager.replaceAccount(id_to_set, company_to_set)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
      cout << "Done!" << endl;
    }
    
    else if (what_to_set == "operative_location") {
      string new_loc;
      cout << "Please insert the new operative location." << endl;
      //cin.ignore(); da provare se ci vada o no
      getline(cin, new_loc);
      company_to_set.setOperativeLocation(new_loc);
      if (!manager.replaceAccount(id_to_set, company_to_set)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
      cout << "Done!" << endl;
    }
    
    else if (what_to_set == "products") {
      string new_prod;
      cout << "Please insert the new type of products." << endl;
      //cin.ignore(); da provare se ci vada o no
      getline(cin, new_prod);
      company_to_set.setTypeOfProduct(new_prod);
      if (!manager.replaceAccount(id_to_set, company_to_set)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
      cout << "Done!" << endl;
    }
  }
}