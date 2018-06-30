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
  
  //Da qui l'help sistemato
  std::cout << "---------- DA CANCELLARE --------------" << std::endl;
  std::cout << "help" << "\n\tPrints this page of help." << std::endl;
  std::cout << "list" << "\n\tPrints a list of all registered accounts" << std::endl;
  std::cout << "get <data_to_get> <id1> [<id2>]"
            << "\n\tPrints the required data. In case of \"get relation\", the two IDs need to be inserted.\n"
               "\tPossible data to get:\n"
               "\tinfo, relation (between two IDs), posts, <type_of_relation>.\n"
               "\tget relation id1 1d2 returns the relation between two IDs,\n"
               "\tget <type_of_relation> id returns a list of all accounts related to that ID through the required relation."
            << std::endl;
  std::cout << "set <field_to_set> <id>\n\tSets the field as required. Possible fields: \n"
               "\tname, surname, gender, address, birth, subscription (USERS)\n"
               "\tname, type_activity, inception, subscription (GROUPS)\n"
               "\tname, products, operative_location, financial_location (COMPANIES)" << std::endl;
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
    std::cout << accounts[i].getType() << "\t" << accounts[i].getID() << std::endl;
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
      std::cout << "No relation found between " << id1 << " and " << id2;
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
        std::cout << "\n" << i + 1 << ":" << std::endl                 //Indice del post
                  << "\n" << posts[i].getNews() << "\n" << std::endl
                  << "[" << posts[i].getDate() << "]" << std::endl;
        std::vector<std::string> likes, dislikes;
        likes = posts[i].getLikes();
        dislikes = posts[i].getDislikes();
        if (!likes.empty()) {
          std::cout << "Likes: ";
          for (int j = 0; j < likes.size(); j++) {
            std::cout << likes[j];
            if (j != likes.size() - 1)
              std::cout << ",";
          }
          std::cout << std::endl;
        }
        if (!dislikes.empty()) {
          std::cout << "Dislikes: ";
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
    //Get <relation_type> <id>
  else if (relation::isValid(what_to_get)) {
    std::string id;
    command >> id;
    std::vector<std::string> relations = manager.getRelated(id, what_to_get);
    if(relations.empty()) {
      std::cout << id << " has no " << what_to_get << "." << std::endl;
      return;
    }
    for(int i = 0; i < relations.size(); i++) {
      std::cout << relations[i] << std::endl;
    }
    std::cout << std::endl;
  }
    //Get <something_not_valid>
  else {
    std::cout << "Cannot get \"" << what_to_get << "\"" << std::endl;
    std::cout << "Possible parameters: info <id>, relation <id1> <id2>, posts <id>, <relation> <id>";
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
    }
    else if (what_to_set == "surname") {
      string new_surname;
      cout << "Please insert the new" << what_to_set << "." << endl;
      //cin.ignore();
      getline(cin, new_surname);
      user_new.setSurname(new_surname);
      if (!manager.replaceAccount(ID_to_set, user_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "gender") {
      char new_gender;
      cout << "Please insert the new gender." << endl;
      cin >> new_gender;
      user_new.setGender(new_gender);
      if (!manager.replaceAccount(ID_to_set, user_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
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
        user_new.setBirth(birth);
        if (!manager.replaceAccount(ID_to_set, user_new)) {
          cout << "Error! " << what_to_set << " could not be modified." << endl;
          return;
        }
      }
      else {
        cout << "Error! Date is not valid." << endl;
        return;
      }
    }
    else if (what_to_set == "address") {
      string new_addr;
      cout << "Please insert the new address." << endl;
      //cin.ignore();
      getline(cin, new_addr);
      user_new.setAddress(new_addr);
      if (!manager.replaceAccount(ID_to_set, user_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "subscription") {
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
      }
      else {  //Data non valida
        cout << "Error! This date is not valid." << endl;
        return;
      }
    }
    else {
      std::cout << "Cannot set \"" << what_to_set << "\" on a User." << std::endl;
    }
    data_to_delete << user_old;
    new_data << user_new;
    cout << "Done!" << endl;
  }
  else if (type == Account::group_type) {
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
    }
    else if (what_to_set == "subscription") {
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
      }
      else {
        cout << "Error! This date is not valid." << endl;
        return;
      }
    }
    else if (what_to_set == "location") {
      string new_loc;
      cout << "Please insert the new location." << endl;
      //cin.ignore();
      getline(cin, new_loc);
      group_new.setLegalLocation(new_loc);
      if (!manager.replaceAccount(ID_to_set, group_new)) { //da provare
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "type_activity") {
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
    }
    else if (what_to_set == "inception") {
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
      }
      else {
        cout << "Error! The date is not valid." << endl;
        return;
      }
    }
    else {
      cout << "Error! Cannot set \"" << what_to_set << " on a Group." << endl;
      return;
    }
    
    new_data << group_new;
    data_to_delete << group_old;
    cout << "Done!" << endl;
  }
  else if (type == Account::company_type) {
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
    }
    else if (what_to_set == "subscription") {
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
      }
      else {
        cout << "Error! This date is not valid." << endl;
        return;
      }
    }
    else if (what_to_set == "inception") {
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
      }
      else {
        cout << "Error! This date is not valid." << endl;
        return;
      }
      
    }
    else if (what_to_set == "financial_location") {
      string new_loc;
      cout << "Please insert the new financial location." << endl;
      ////cin.ignore(); da provare se ci vada o no
      getline(cin, new_loc);
      company_new.setFinancialLocation(new_loc);
      if (!manager.replaceAccount(ID_to_set, company_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "operative_location") {
      string new_loc;
      cout << "Please insert the new operative location." << endl;
      ////cin.ignore(); da provare se ci vada o no
      getline(cin, new_loc);
      company_new.setOperativeLocation(new_loc);
      if (!manager.replaceAccount(ID_to_set, company_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "products") {
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

void Shell::add(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  string what_to_add;
  command >> what_to_add;
  
  if (what_to_add == "user") {
    string tmp_n, tmp_s, tmp_id, tmp_a, d1, d2;
    Date tmp_sub, tmp_b;
    char tmp_g;
    cout << "Name:\n>";
    getline(cin, tmp_n);
    cout << "Surname:\n>";
    getline(cin, tmp_s);
    cout << "Id:\n>";
    getline(cin, tmp_id);
    cout << "Address:\n>";
    getline(cin, tmp_a);
    cout << "Subscription:\n>";
    getline(cin, d1);
    tmp_sub.scanDateByStr(d1);
    cout << "Birth:\n>";
    getline(cin, d2);
    tmp_b.scanDateByStr(d2);
    cout << "Gender:\n"
            "Possible gender:\n"
            "'M' MALE\n"
            "'F' FEMALE\n"
            "'N' NON-BINARY\n"
            "'A' AGENDER\n"
            "'B' BIGENDER\n"
            "'O' OTHERS\n>";
    tmp_g = char(cin.get());
    User new_u(tmp_n, tmp_s, tmp_id, tmp_a, tmp_sub, tmp_b, tmp_g);
    if (!manager.addAccount(new_u)) {
      cout << "Error! Account could not be created." << endl;
      return;
    }
    new_data << new_u;
  }
  else if (what_to_add == "group") {
    string tmp_n, tmp_id, tmp_loc, tmp_act, d1, d2;
    Date tmp_sub, tmp_inc;
    cout << "Name:\n>";
    getline(cin, tmp_n);
    cout << "Id:\n>";
    getline(cin, tmp_id);
    cout << "Legal location:\n>";
    getline(cin, tmp_loc);
    cout << "Type of activity:\n>";
    getline(cin, tmp_act);
    cout << "Subscription:\n>";
    getline(cin, d1);
    tmp_sub.scanDateByStr(d1);
    cout << "Inception:\n>";
    getline(cin, d2);
    tmp_inc.scanDateByStr(d2);
    
    Group new_g(tmp_n, tmp_id, tmp_loc, tmp_act, tmp_sub, tmp_inc);
    if (!manager.addAccount(new_g)) {
      cout << "Error! This ID already exists!";
      return;
    }
    new_data << new_g;
  }
  else if (what_to_add == "company") {
    string tmp_n, tmp_id, tmp_finloc, tmp_oploc, tmp_p, d1, d2;
    Date tmp_sub, tmp_inc;
    //cin.ignore();
    cout << "Name:\n>";
    getline(cin, tmp_n);
    cout << "Id:\n>";
    getline(cin, tmp_id);
    cout << "Finantial location:\n>";
    getline(cin, tmp_finloc);
    cout << "Operative location:\n>";
    getline(cin, tmp_oploc);
    cout << "Type of product:\n>";
    getline(cin, tmp_p);
    cout << "Subscription:\n>";
    getline(cin, d1);
    tmp_sub.scanDateByStr(d1);
    cout << "Inception:\n>";
    getline(cin, d2);
    tmp_inc.scanDateByStr(d2);
    
    Company new_c(tmp_n, tmp_id, tmp_finloc, tmp_oploc, tmp_p, tmp_sub, tmp_inc);
    if (!manager.addAccount(new_c)) {
      cout << "Error! This ID already exists." << endl;
      return;
    }
    new_data << new_c;  //Metti la nuova compagnia nel buffer
  }
  
  else if (what_to_add == "relation") {
   int error;
    string who1, who2, type_rel;
      command >> who1 >> type_rel >> who2;
      if (who1.empty() || who2.empty() || type_rel.empty()) {
        cout << "Error! Your data were not inserted properly, I got some of them empty. Please retry!" << endl;
        return;
      }
      error = manager.addRelationship(who1, who2, type_rel);
      //Analizzo il codice errore restituito da addRelationship.
      if (error == -1) {
        cout << "Error! The first ID does not exist!" << endl;
        return;
      }
      else if (error == -2) {
        cout << "Error! The second ID does not exist!" << endl;
        return;
      }
      else if (error == -3) {
        cout << "Error! This relationship does not exist!" << endl;
        cout << "Possible relations:\n"
                "\tfriend (U - U)\n"
                "\tacquaintance (U - U)\n"
                "\tparent (U - U)\n"
                "\tson (U - U)\n"
                "\tpartner (U - U)\n"
                "\tmember (U - G)\n"
                "\tpartner\n"
                "\tworker (U -> C)\n"
                "\temployer (C -> U)\n" << endl;
  
        return;
      }
      else if (error == -4) {
        cout << "Error! The relationship and the Users' ages are not compatible!" << endl;
        return;
      }
      new_data << std::make_pair(std::make_pair(who1, who2), type_rel);   //In caso di successo
  }
  else if (what_to_add == "post") {
    string news, d_t, whose_ID;
    std::string input;
    std::set<std::string> likes, dislikes;
    bool isValid = false;
    Post post_tmp;
    
    cout << "News:\n>";
    getline(cin, news);
    post_tmp.setNews(news);
    
    do {
      cout << "Date and time:\n>"; //a me sta cosa non funziona, sto impazzendo, non esce dal ciclo
      cin.ignore();
      getline(cin, d_t);
      isValid = post_tmp.setDate_Time(d_t);
      
    } while (!isValid);
    
    cout << "Likes (insert '-' at the end of the list):\n";
    while (input != "-") {
      cout << ">";
      getline(cin, input);
      if (input != "-")
        likes.insert(input);
    }
    post_tmp.setLikes(likes);
    
    input.clear(); //Altrimenti non entra nel ciclo dei dislike
    cout << "Dislikes (insert '-' at the end of the list):\n";
    while (input != "-") {
      cout << ">";
      getline(cin, input);
      if (input != "-")
        dislikes.insert(input);
    }
    post_tmp.setDislikes(dislikes);
    
    /*cout << news << " " << d_t << endl;
    
    if (!likes.empty()) {
      cout << "\nLikes:" << endl;
      
      for (auto it = likes.begin(); it != likes.end(); it++) {
        cout << *it << endl;
      }
    }
    
    if (!dislikes.empty()) {
      cout << "\nDislikes:" << endl;
      for (auto it = dislikes.begin(); it != dislikes.end(); it++) {
        cout << *it << endl;
      }
    }*/
    cout << "Who wrote the post?:\n>";
    getline(cin, whose_ID);
    if (!manager.addPost(post_tmp, whose_ID)) {
      cout << "Could not create post." << endl;
      return;
    }
    new_data << std::make_pair(whose_ID, post_tmp); //MANCAVA QUESTO, TESTO.
  }
  else if (what_to_add == "like" || what_to_add == "dislike") {
    string post_owner, reaction_id, post_num_str;
    unsigned int post_num;
    vector<Post> posts;
    
    cout << "Please insert the post owner's ID:" <<endl;
    cout << ">";
    getline(cin, post_owner);
    cout << "Insert the account reacting to the post's ID:" << endl;
    cout << ">";
    getline(cin, reaction_id);
    cout << "Please insert post number:" << endl;
    cout << ">";
    getline(cin, post_num_str);
    post_num = stoi(post_num_str);

    post_num--;
    posts = manager.getPosts(post_owner);   //Acquisisci i post per passarli al manager
    if(posts.size() <= post_num) {   //Controlla se il proprietario dell'account ha il post
      cout << "Error! Could find post " << post_num + 1 << " from " << post_owner << "." <<endl;
      return;
    }
    
    Post &old_post = posts[post_num];
    
    bool like = what_to_add == "like";      //Decidi se mettere like o dislike
    if(!manager.setReaction(like, true, post_owner, post_num, reaction_id)) {
      cout << "Error! Could find post " << post_num + 1<< " from " << post_owner << "." <<endl;
      return;
    }
    data_to_delete << std::make_pair(post_owner, old_post);
    new_data << std::make_pair(post_owner, manager.getPosts(post_owner)[post_num]);
  }
  
  else {
    cout << "Could not add \"" << what_to_add << "\"." << endl;
    return;
  }
  cout << "Done!" << endl;
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
    
    if(!manager.accountExists(ID_to_delete)) {
      std::cout << "Account not valid" << std::endl;
    }
    
    std::vector<Post> posts;
    std::vector<IOBuffer::Relation> relations;
    
    posts = manager.getPosts(ID_to_delete);
    
    char type = manager.getAccountType(ID_to_delete);
    if(type == Account::user_type) {
      User temp = manager.getUser(ID_to_delete);
      if (manager.deleteAccount(ID_to_delete)) {
        data_to_delete << temp;
      }
    }
    else if(type == Account::group_type) {
      Group temp = manager.getGroup(ID_to_delete);
      if (manager.deleteAccount(ID_to_delete)) {
        data_to_delete << temp;
      }
    }
    else if(type == Account::company_type) {
      Company temp = manager.getCompany(ID_to_delete);
      if (manager.deleteAccount(ID_to_delete)) {
        data_to_delete << temp;
      }
    }
    else {
      std::cout << "Account not valid." << std::endl;
      return;
    }
  }
  
  else if (what_to_delete == "relation") {
    string id_start, id_target;
    command >> id_start >> id_target;
    std::string relation = manager.getRelationship(id_start, id_target);
    if (!manager.deleteRelationship(id_start, id_target)) {
      cout << "Error! IDs not found." << endl;
      return;
    }
    data_to_delete << std::make_pair(std::make_pair(id_start, id_target), relation);
  }
  
  else if (what_to_delete == "post") {
    std::string post_owner;
    unsigned int post_num;
    command >> post_owner >> post_num;
    post_num --;
    
    std:vector<Post> posts = manager.getPosts(post_owner);
    if(posts.size() <= post_num) {
      std::cout << "Error! " << post_owner << " does not have post " << post_num + 1 << std::endl;
      return;
    }
    if(!manager.deletePost(post_owner, post_num)) {
      cout << "Error! Post " << post_num + 1 << " from " << post_owner << " could not be deleted." << endl;
      return;
    }
    data_to_delete << std::make_pair(post_owner, posts[post_num]);
  }
  
  else if (what_to_delete == "like") {
    string who, tmp_news, d_t;
    pair<string, vector<Post>> post;
    
    cout << "Please insert the news of the target post:\n>";
    getline(cin, tmp_news);
    cout << "Please insert the date and the time (in format dd/mm/yyyy hh:mm) of the target post:\n>";
    getline(cin, d_t);
    cout << "Please insert the ID whose User did not like this post anymore:\n>";
    getline(cin, who);
    
    Post cmp_post(tmp_news, d_t);
    
    if (!manager.setReaction(1, 0, cmp_post, who)) {
      cout << "Error! Operation not successful." << endl; //1-NO ID, 2-NO AUTOLIKES, 3-NO POST
      return;
    }
    
  }
  else if (what_to_delete == "dislike") {
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
    
    if (!manager.setReaction(0, 0, cmp_post, who)) {
      cout << "Error! I could not remove this dislike" << endl; //1-NO ID, 2-NO AUTOLIKES, 3-NO POST
      return;
    }
  }
  else {
    cout << "Cannot delete \"" << what_to_delete << "\"." << endl;
    return;
  }
  std::cout << "Done!" << std::endl;
}

void Shell::stats(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  string param1, param2;
  command >> param1 >> param2;
  if (param1 == "number") {
    size_t num;
    string id;
    if (param2 == "accounts")
      num = manager.NumAccounts();
    else if (param2 == "users")
      num = manager.NumUsers();
    else if (param2 == "groups")
      num = manager.NumGroups();
    else if (param2 == "companies")
      num = manager.NumCompanies();
    else if (param2 == "friends") {
      command >> id;
      num = manager.NumFriends(id);
      if (num == 0) {
        cout << "Error! " << id << " is not a User or it does not exist." << endl;
        return;
      }
    }
    else if (param2 == "relatives") {
      command >> id;
      num = manager.NumRelatives(id);
      if (num == 0) {
        cout << id << " has no relatives." << endl;
        return;
      }
    }
    else if (param2 == "workers") {
      command >> id;
      num = manager.NumEmployees(id);
      if (num == 0) {
        cout << id << " has no workers." << endl;
        return;
      }
    }
    else if (param2 == "subsidiaries") {
      command >> id;
      num = manager.NumSubsidiaries(id);
      if (num == 0) {
        cout << id << " has no subsidiaries." << endl;
        return;
      }
    }
    else if (param2 == "members") {
      command >> id;
      num = manager.NumMembers(id);
      if (num == 0) {
        cout << id << " has no members." << endl;
        return;
      }
    }
    else if (param2 == "born_after") {
      string date;
      Date born_d;
      //int not_valid;
      cout << "Please insert the starting date (in format dd/mm/yyyy):\n>";
      cin >> date;
      if (!Date::CheckDate(date)) {
        cout << "Error! The date is not valid." << endl;
        return;
      }
      born_d.scanDateByStr(date);
      num = manager.NumBornAfter(born_d);
      cout << "The number of people born after your date is: " << num << endl;
      return;
    }
    else {
      cout << "Error! Cannot get number of " << param2 << "." << endl;
      return;
    }
    cout << "The number of " << param2 << " is: " << num << endl;
  }
  else if (param1 == "most") {
    if (param2 == "employing_company") {
      Company empl_comp;
      empl_comp = manager.MostEmployingCompany();
      cout << "Name: " << empl_comp.getName() << "\n"
           << "Financial location: " << empl_comp.getFinancialLocation() << "\n"
           << "Operative location: " << empl_comp.getOperativeLocation() << "\n"
           << "Product: " << empl_comp.getTypeOfProduct() << "\n"
           << "Inception: " << empl_comp.getInception() << "\n"
           << "Birth Date: " << empl_comp.getSubscription() << std::endl;
      
    }
    else if (param2 == "employing_partnership") {
      vector<string> part;
      part = manager.MostEmployingPartnership();
      cout << "The members of the Most Employing Partnership are:\n";
      for (auto it = part.begin(); it != part.end(); it++) {
        cout << *it << endl;
      }
    }
    else if (param2 == "user_friends") {
      User mostfr_user;
      mostfr_user = manager.UserWithMostFriends();
      cout << "Name:" << mostfr_user.getName() << "\n"
           << "Surname: " << mostfr_user.getSurname() << "\n"
           << "Gender: " << mostfr_user.getGender() << "\n"
           << "Address: " << mostfr_user.getAddress() << "\n"
           << "Birth Date :" << mostfr_user.getBirth() << "\n"
           << "Subscription Date: " << mostfr_user.getSubscription() << std::endl;
      
    }
    else if (param2 == "user_acquaintances") {
      User mostacq_user;
      mostacq_user = manager.UserWithMostAcquaintances();
      cout << "Name:" << mostacq_user.getName() << "\n"
           << "Surname: " << mostacq_user.getSurname() << "\n"
           << "Gender: " << mostacq_user.getGender() << "\n"
           << "Address: " << mostacq_user.getAddress() << "\n"
           << "Birth Date :" << mostacq_user.getBirth() << "\n"
           << "Subscription Date: " << mostacq_user.getSubscription() << std::endl;
    }
    
    else if (param2 == "liked_post") {
      pair<string, Post> l_post;
      l_post = manager.MostLikedPost();
      cout << "The most liked post is:\n" << l_post.second << endl
           << "From: " << l_post.first;
      
    }
    else if (param2 == "disliked_post") {
      pair<string, Post> d_post;
      d_post = manager.MostDislikedPost();
      cout << "The most disliked post is:\n" << d_post.second << endl
           << "From: " << d_post.first;
    }
    else if (param2 == "liked_account") {
      cout << "The Most Liked Account is:\n" << manager.MostLiked_DislikedAccount(1);
    }
    else if (param2 == "disliked_account") {
      cout << "The Most Disliked Account is:\n" << manager.MostLiked_DislikedAccount(0);
    }
    else {
      cout << "Error! I do not understand what statistic you'd like to retreive." << endl;
    }
  }
  else if (param1 == "average_age") {
    cout << "The Average of Users' ages is:\n"
         << manager.UsersAverageAge() << endl;
  }
  else if (param1 == "best_post") {
    pair<string, Post> best_post;
    best_post = manager.RatioReactionPost(true);
    cout << "The Post with the best Like/Dislike ratio is:\n" << best_post.second << endl
         << "Wrote by: " << best_post.first;
  }
  else if (param1 == "worse_post") {
    pair<string, Post> worse_post;
    worse_post = manager.RatioReactionPost(false);
    cout << "The Post with the worse Like/Dislike ratio is:\n" << worse_post.second << endl
         << "Wrote by: " << worse_post.first;
  }
  else {
    std::cout << "Cannot get satystics of " << param1 << std::endl;
  }
}

void Shell::search(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  string what_to_search;
  command >> what_to_search;
  if (what_to_search == "genealogical_trees") {
    /*vector <string> trees_to_print = manager.PrintAllTrees();
    for (auto it = trees_to_print.begin(); it!=trees_to_print.end(); it++){
      new_data_buffer << *it;
    }*/
    // devo poterli stampare su files diversi
    
  }
  else if (what_to_search == "genealogical_tree") {
    string id;
    cout << "Please insert the id of the User you want to build the genealogical tree:\n";
    cin >> id;
    cout << manager.PrintTree(id); //dentro la funzione non c'è il controllo dell'esistenza dell'id
    //per ora ho messo cout anche se vanno stampati su file
    
  }
  else if (what_to_search == "loner_people") {
    unsigned int relations, memberships, reactions;
    string ans; //answer
    bool unemployed;
    vector<string> lon_people;
    
    cout << "Please insert the parameters that define a loner person:\n"
            "Minimum number of relation: ";
    cin >> relations;
    cout << "Minimum number of groups: ";
    cin >> memberships;
    cout
            << "Does the user need to be unemployed? (yes/no): "; //non ho capito se logicamente sia giusto per come è fatta la funzione
    cin >> ans;
    if (ans == "yes") {
      unemployed = true;
    }
    else {
      unemployed = false;
    }
    cout << "Minimum number of added reactions: ";
    cin >> reactions;
    
    lon_people = manager.LonerPeople(relations, memberships, unemployed, reactions);
    for (auto it = lon_people.begin(); it != lon_people.end(); it++) {
      cout << *it << endl;
    }
    
  }
  else if (what_to_search == "friendliest_company") {
    //function
  }
  else {
    cout << "Error! Cannot search for" << what_to_search << "." << endl;
  }
}