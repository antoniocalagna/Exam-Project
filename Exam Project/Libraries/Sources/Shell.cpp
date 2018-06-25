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
  cout << "stats most employing_company\n" //DA UNIRE
          "stats most employing_partnership\n"
          "stats most user_friends\n"
          "stats most user_acquaintances\n"
          "stats most liked_post\n"
          "stats most disliked_post\n"
          "stats most liked_account\n"
          "stats most disliked_account\n"
       << endl;
  cout << "stats average_age\n"
          "stats best_post\n"
          "stats worse_post\n"
       << endl;
  cout << "search genealogical_trees\n"
          "search genealogical_tree\n"
          "search loner_people\n"
          "search friendly_companies\n"
          << endl;
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
    char account_type = manager.getAccountType(
            requested_id);   //Controlla il tipo di account di cui sono richieste le informazioni
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
    } else {
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

  string what_to_set, ID_to_set; //non so se questi nomi sono messi per scherzo, a me sembrano carini
  command >> what_to_set >> ID_to_set;

  if (what_to_set.empty() || ID_to_set.empty()) {
    cout << "Error! I do not understand what information you'd like to set." << endl;
    return;
  }

  char type = manager.getAccountType(ID_to_set);
  if (type == Account::user_type) {
    User user_old;                         //User a cui cambiare le info
    User user_new;                         //Utente modificato

    user_old = manager.getUser(ID_to_set); //se l'id non esiste, user_old è un default constructor
    user_new = user_old;

    if (user_old == User()) {
      cout << "Error! This ID is not valid." << endl;
      return;
    }

    if (what_to_set == "name") {
      string new_name;
      cout << "Please insert the new " << what_to_set << "." << endl;
      //cin.ignore();                           //Salta il carattere \n rimasto dall'input precedente
      getline(cin, new_name);
      user_new.setName(new_name);             //Modifica il dato nel nuovo utente

      if (!manager.replaceAccount(ID_to_set, user_new)) { //Il manager non è riuscuto a rimpiazzare l'account
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    } else if (what_to_set == "surname") {
      string new_surname;
      cout << "Please insert the new" << what_to_set << "." << endl;
      //cin.ignore();
      getline(cin, new_surname);
      user_new.setSurname(new_surname);
      if (!manager.replaceAccount(ID_to_set, user_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    } else if (what_to_set == "gender") {
      char new_gender;
      cout << "Please insert the new gender." << endl;
      cin >> new_gender;
      user_new.setGender(new_gender);
      if (!manager.replaceAccount(ID_to_set, user_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    } else if (what_to_set == "birth") {
      string new_birth;
      Date birth;
      cout << "Please insert the new" << what_to_set << "." << endl;
      //cin.ignore();
      getline(cin, new_birth);
      if (birth.CheckDate(new_birth)) {
        birth.scanDateByStr(new_birth);
        user_new.setBirth(birth);
        if (!manager.replaceAccount(ID_to_set, user_new)) {
          cout << "Error! " << what_to_set << " could not be modified." << endl;
          return;
        }
      } else {
        cout << "Error! Date is not valid." << endl;
        return;
      }
    } else if (what_to_set == "address") {
      string new_addr;
      cout << "Please insert the new address." << endl;
      //cin.ignore();
      getline(cin, new_addr);
      user_new.setAddress(new_addr);
      if (!manager.replaceAccount(ID_to_set, user_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    } else if (what_to_set == "subscription") {
      string new_sub;
      Date sub;
      cout << "Please insert the new date of subscription (in format dd/mm/yyyy)." << endl;
      //cin.ignore();
      getline(cin, new_sub);
      if (sub.CheckDate(new_sub)) {   //Controlla che la data sia valida
        sub.scanDateByStr(new_sub);
        user_new.setSubscription(new_sub);
        if (!manager.replaceAccount(ID_to_set, user_new)) {
          cout << "Error! " << what_to_set << " could not be modified." << endl;
          return;
        }
      } else {  //Data non valida
        cout << "Error! This date is not valid." << endl;
        return;
      }
    } else {
      std::cout << "Cannot set \"" << what_to_set << "\" on a User." << std::endl;
    }
    data_to_delete << user_old;
    new_data << user_new;
    cout << "Done!" << endl;
  } else if (type == Account::group_type) {
    Group group_old;
    Group group_new;
    group_old = manager.getGroup(ID_to_set);
    group_new = group_old;

    if (group_new == Group()) {                                  //Il manager non ha trovato il gruppo
      cout << "Error! This ID is not valid." << endl;
      return;
    }
    if (what_to_set == "name") {
      string new_name;
      cout << "Please insert the new name." << endl;
      //cin.ignore();
      getline(cin, new_name);
      group_new.setName(new_name);
      if (!manager.replaceAccount(ID_to_set, group_new)) { //da provare
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    } else if (what_to_set == "subscription") {
      string new_sub;
      Date sub;
      cout << "Please insert the new date of subscription (in format dd/mm/yyyy)." << endl;
      //cin.ignore();
      getline(cin, new_sub);
      if (sub.CheckDate(new_sub)) {
        sub.scanDateByStr(new_sub);
        group_new.setSubscription(new_sub);
        if (!manager.replaceAccount(ID_to_set, group_new)) {
          cout << "Error! This ID already exists." << endl;
          return;
        }
      } else {
        cout << "Error! This date is not valid." << endl;
        return;
      }
    } else if (what_to_set == "location") {
      string new_loc;
      cout << "Please insert the new location." << endl;
      //cin.ignore();
      getline(cin, new_loc);
      group_new.setLegalLocation(new_loc);
      if (!manager.replaceAccount(ID_to_set, group_new)) { //da provare
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    } else if (what_to_set == "type_activity") {
      string new_act;
      cout << "Please insert the new type of activity." << endl;
      //cin.ignore();
      getline(cin, new_act);
      group_new.setTypeOfActivity(new_act);
      if (!manager.replaceAccount(ID_to_set, group_new)) { //da provare
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
      cout << "Done!" << endl;
    } else if (what_to_set == "inception") {
      string new_inc;
      Date inc;
      cout << "Please insert the new date of inception (in format dd/mm/yyyy)." << endl;
      //cin.ignore(); da provare se ci vada o no
      getline(cin, new_inc);
      if (inc.CheckDate(new_inc)) {
        inc.scanDateByStr(new_inc);
        group_new.setInception(inc);
        if (!manager.replaceAccount(ID_to_set, group_new)) { //da provare
          cout << "Error! " << what_to_set << " could not be modified." << endl;
          return;
        }
      } else {
        cout << "Error! The date is not valid." << endl;
        return;
      }
    } else {
      cout << "Error! Cannot set \"" << what_to_set << " on a Group." << endl;
      return;
    }

    new_data << group_new;
    data_to_delete << group_old;
    cout << "Done!" << endl;
  } else if (type == Account::company_type) {
    Company company_old;
    Company company_new;
    company_old = manager.getCompany(ID_to_set);
    company_new = company_old;

    if (company_old == Company()) {
      cout << "Error! This ID is not valid." << endl;
    }

    if (what_to_set == "name") {
      string new_name;
      cout << "Please insert the new name." << endl;
      ////cin.ignore(); da provare se ci vada o no
      getline(cin, new_name);
      company_new.setName(new_name);
      if (!manager.replaceAccount(ID_to_set, company_new)) { //da provare
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    } else if (what_to_set == "subscription") {
      string new_sub;
      Date sub;
      cout << "Please insert the new date of subscription (in format dd/mm/yyyy)." << endl;
      ////cin.ignore();
      getline(cin, new_sub);
      if (sub.CheckDate(new_sub)) {
        sub.scanDateByStr(new_sub);
        company_new.setSubscription(new_sub);
        if (!manager.replaceAccount(ID_to_set, company_new)) {
          cout << "Error! This ID already exist." << endl;
          return;
        }
      } else {
        cout << "Error! This date is not valid." << endl;
        return;
      }
    } else if (what_to_set == "inception") {
      string new_inc;
      Date inc;
      cout << "Please insert the new date of inception (in format dd/mm/yyyy)." << endl;
      ////cin.ignore(); da provare se ci vada o no
      getline(cin, new_inc);
      if (inc.CheckDate(new_inc)) {
        inc.scanDateByStr(new_inc);
        company_new.setInception(inc);
        if (!manager.replaceAccount(ID_to_set, company_new)) {
          cout << "Error! " << what_to_set << " could not be modified." << endl;
          return;
        }
      } else {
        cout << "Error! This date is not valid." << endl;
        return;
      }

    } else if (what_to_set == "financial_location") {
      string new_loc;
      cout << "Please insert the new financial location." << endl;
      ////cin.ignore(); da provare se ci vada o no
      getline(cin, new_loc);
      company_new.setFinancialLocation(new_loc);
      if (!manager.replaceAccount(ID_to_set, company_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    } else if (what_to_set == "operative_location") {
      string new_loc;
      cout << "Please insert the new operative location." << endl;
      ////cin.ignore(); da provare se ci vada o no
      getline(cin, new_loc);
      company_new.setOperativeLocation(new_loc);
      if (!manager.replaceAccount(ID_to_set, company_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    } else if (what_to_set == "products") {
      string new_prod;
      cout << "Please insert the new type of products." << endl;
      ////cin.ignore(); da provare se ci vada o no
      getline(cin, new_prod);
      company_new.setTypeOfProduct(new_prod);
      if (!manager.replaceAccount(ID_to_set, company_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }

    new_data << company_new;
    data_to_delete << company_old;
    std::cout << "Done!" << std::endl;
  }
}

void Shell::del(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  string what_to_delete;
  command >> what_to_delete;
  if (what_to_delete.empty()) {
    cout << "Error! Missing parameter." << endl;
  }
  if (what_to_delete == "account") {
    string ID_to_delete;
    command >> ID_to_delete;
    if (ID_to_delete.empty()) {
      cout << "Error! Missing parameter." << endl;
      return;
    }
    manager.deleteAccount(ID_to_delete);
  } else if (what_to_delete == "relationship") {
    string id_start, id_target;
    cout << "Please insert: <id_subject> <id_target>:\n";
    command >> id_start >> id_target;
    if (id_start.empty() || id_target.empty()) {
      cout << "Error! I do not understand which IDs I have to work with." << endl;
    }
    if (!manager.deleteRelationship(id_start, id_target)) {
      cout << "Error! One or both of your IDs don't exist." << endl;
    }
  } else if (what_to_delete == "post") {
    string who, tmp_news, d_t;
    vector<Post> post;
    int find = 0;
    cout << "Please insert the ID whose user wrote this post:\n>";
    cin >> who;
    if (who.empty()) {
      cout << "Error! You did not write any ID." << endl;
    }
    post = manager.getPosts(who);
    cout << "Please insert the news of the target post:\n>";
    cin.ignore();
    getline(cin, tmp_news);
    cout << "Please insert date and the time (in format dd/mm/yyyy hh:mm) of the target post:\n>";
    cin.ignore();
    getline(cin, d_t);

    Post cmp_post(tmp_news, d_t);

    for (auto it = post.begin(); it != post.end(); it++) {
      if (*it == cmp_post) {
        manager.deletePost(*it, who);
        find = 1;
      }
    }
    if (find != 1) {
      cout << "Post not found!" << endl;
    }
  } else if (what_to_delete == "like") {
    string who, tmp_news, d_t;
    pair<string, vector<Post>> post;

    cout << "Please insert the news of the target post:\n>";
    cin.ignore();
    getline(cin, tmp_news);
    cout << "Please insert the date and the time (in format dd/mm/yyyy hh:mm) of the target post:\n>";
    cin.ignore();
    getline(cin, d_t);
    cout << "Please insert the ID whose User did not like this post anymore:\n>";
    cin >> who;

    Post cmp_post(tmp_news, d_t);

    if (manager.setReaction(1, 0, cmp_post, who)) {
      cout << "Done!" << endl;
    } else {
      cout << "Error! I could not remove this like" << endl; //1-NO ID, 2-NO AUTOLIKES, 3-NO POST
    }

  } else if (what_to_delete == "dislike") {
    string who, tmp_news, d_t;
    pair<string, vector<Post>> post;

    cout << "Please insert the news of the target post:\n>";
    cin.ignore();
    getline(cin, tmp_news);
    cout << "Please insert the date and the time (in format dd/mm/yyyy hh:mm) of the target post:\n>";
    cin.ignore();
    getline(cin, d_t);
    cout << "Please insert the ID whose User did not dislike this post anymore:\n>";
    cin >> who;

    Post cmp_post(tmp_news, d_t);

    if (manager.setReaction(0, 0, cmp_post, who)) {
      cout << "Done!" << endl;
    } else {
      cout << "Error! I could not remove this dislike" << endl; //1-NO ID, 2-NO AUTOLIKES, 3-NO POST
    }
  } else {
    cout << "I do not understand what you'd like to delete." << endl;
  }
}

void Shell::stats(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete){
  string what1, what2;
  command >> what1 >> what2;
  if (what1 == "number") {
    size_t num;
    string id;
    if (what2 == "accounts")
      num = manager.NumAccounts();
    else if (what2 == "users")
      num = manager.NumUsers();
    else if (what2 == "groups")
      num = manager.NumGroups();
    else if (what2 == "companies")
      num = manager.NumCompanies();
    else if (what2 == "friends") {
      cout << "Please insert the target ID:\n>";
      cin >> id;
      num = manager.NumFriends(id);
      if (num == 0) {
        cout << "Error! " << id << " is not a User or it does not exist." << endl;
      } else {
        cout << "The number of " << what2 << " is: " << num << endl;
      }
    } else if (what2 == "relatives") {
      cout << "Please insert the ID:\n>";
      cin >> id;
      num = manager.NumRelatives(id);
      if (num == 0) {
        cout << "Error! " << id << " is not a User or it does not exist." << endl;
      } else {
        cout << "The number of " << what2 << " is: " << num << endl;
      }
    } else if (what2 == "employees") {
      cout << "Please insert the ID of the Company employer:\n>";
      cin >> id;
      num = manager.NumEmployees(id);
      if (num == 0) {
        cout << "Error! " << id <<  "is not a Company or it does not exist." << endl;
      } else {
        cout << "The number of " << what2 << " is: " << num << endl;
      }
    } else if (what2 == "subsidiaries") {
      cout << "Please insert the target ID:\n>";
      cin >> id;
      num = manager.NumSubsidiaries(id);
      if (num == 0) {
        cout << "Error! " << id << " is not a Company or it does not exist." << endl;
      } else {
        cout << "The number of " << what2 << " is: " << num << endl;
      }
    } else if (what2 == "members") {
      cout << "Please insert the target ID:\n>";
      cin >> id;
      num = manager.NumMembers(id);
      if (num == 0) {
        cout << "Error! " << id << " is not a Group or it does not exist." << endl;
      } else {
        cout << "The number of " << what2 << " is: " << num << endl;
      }
    } else if (what2 == "born_after") {
      string date;
      Date born_d;
      //int not_valid;
      cout << "Please insert the starting date (in format dd/mm/yyyy):\n>";
      cin >> date;
      if (born_d.CheckDate(date)) {
        born_d.scanDateByStr(date);
        num = manager.NumBornAfter(born_d);
        cout << "The number of people born after your date is: " << num << endl;
      } else {
        cout << "Error! This date is not valid." << endl;
        num = 0;
      }
    } else {
      cout << "Error! I do not understand what statistic you'd like to retreive." << endl;
      num = 0;
    }
    cout << num ;

  } else if (what1 == "most") {
    if (what2 == "employing_company") {
      Company empl_comp;
      empl_comp = manager.MostEmployingCompany();
      cout << "Name: " << empl_comp.getName() << "\n"
           << "Financial location: " << empl_comp.getFinancialLocation() << "\n"
           << "Operative location: " << empl_comp.getOperativeLocation() << "\n"
           << "Product: " << empl_comp.getTypeOfProduct() << "\n"
           << "Inception: " << empl_comp.getInception() << "\n"
           << "Birth Date: " << empl_comp.getSubscription() << std::endl;

    } else if (what2 == "employing_partnership") {
      vector<string> part;
      part = manager.MostEmployingPartnership();
      cout << "The members of the Most Employing Partnership are:\n";
      for (auto it = part.begin(); it != part.end(); it++) {
        cout << *it << endl;
      }
    } else if (what2 == "user_friends") {
      User mostfr_user;
      mostfr_user = manager.UserWithMostFriends();
      cout << "Name:" << mostfr_user.getName() << "\n"
           << "Surname: " << mostfr_user.getSurname() << "\n"
           << "Gender: " << mostfr_user.getGender() << "\n"
           << "Address: " << mostfr_user.getAddress() << "\n"
           << "Birth Date :" << mostfr_user.getBirth() << "\n"
           << "Subscription Date: " << mostfr_user.getSubscription() << std::endl;

    } else if (what2 == "user_acquaintances") {
      User mostacq_user;
      mostacq_user = manager.UserWithMostAcquaintances();
      cout << "Name:" << mostacq_user.getName() << "\n"
           << "Surname: " << mostacq_user.getSurname() << "\n"
           << "Gender: " << mostacq_user.getGender() << "\n"
           << "Address: " << mostacq_user.getAddress() << "\n"
           << "Birth Date :" << mostacq_user.getBirth() << "\n"
           << "Subscription Date: " << mostacq_user.getSubscription() << std::endl;

    } else if (what2 == "liked_post") {
      pair<string, Post> l_post;
      l_post = manager.MostLikedPost();
      cout << "The Most Liked Post is:\n" << l_post.second << endl
           << "Wrote by: " << l_post.first;

    } else if (what2 == "disliked_post") {
      pair<string, Post> d_post;
      d_post = manager.MostDislikedPost();
      cout << "The Most Disliked Post is:\n" << d_post.second << endl
           << "Wrote by: " << d_post.first;

    } else if (what2 == "liked_account") {
      cout << "The Most Liked Account is:\n" << manager.MostLiked_DislikedAccount(1);

    } else if (what2 == "disliked_account") {
      cout << "The Most Disliked Account is:\n" << manager.MostLiked_DislikedAccount(0);

    } else {
      cout << "Error! I do not understand what statistic you'd like to retreive." << endl;
    }

  } else if (what1 == "average_age") {
    cout << "The Average of Users' ages is:\n"
         << manager.UsersAverageAge() << endl;

  } else if (what1 == "best_post") {
    pair<string, Post> best_post;
    best_post = manager.RatioReactionPost(1);
    cout << "The Post with the best Like/Dislike ratio is:\n" << best_post.second << endl
         << "Wrote by: " << best_post.first;
  } else if (what1 == "worse_post") {
    pair<string, Post> worse_post;
    worse_post = manager.RatioReactionPost(0);
    cout << "The Post with the worse Like/Dislike ratio is:\n" << worse_post.second << endl
         << "Wrote by: " << worse_post.first;
  }
}