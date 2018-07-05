//
// Created by Alex on 24/06/2018.
//

#include "Shell.h"

void Shell::help(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  std::cout << "help" << "\n\tPrints this page of help." << std::endl;
  std::cout << "list" << "\n\tPrints a list of all registered accounts" << std::endl;
  std::cout << "get <data_to_get> <id1> [<id2>]"
            << "\n\tPrints the required data.\n"
               "\tPossible data to get:\n"
               "\tinfo, relation (between two IDs), posts, <type_of_relation>.\n"
               "\tIn case of \"get relation\", the two IDs need to be inserted.\n"
               "\tget relation id1 1d2 returns the relation between two IDs,\n"
               "\tget <type_of_relation> <id> returns a list of all accounts related to that ID through the required relation."
            << std::endl;
  std::cout << "set <field_to_set> <id>\n\tSets the field as required. Possible fields: \n"
               "\tname, surname, gender, address, birth, subscription (USERS)\n"
               "\tname, type_activity, inception, subscription (GROUPS)\n"
               "\tname, products, operative_location, financial_location (COMPANIES)"
            << std::endl;
  std::cout << "add <data_to_add>"
               "\n\tCreates new data for the program.\n"
               "\tPossible data to add:\n"
               "\tuser, group, company,\n"
               "\trelation (in format \"add relation <id1> <relation> <id2>\"),\n"
               "\tpost, like, dislike."
            << std::endl;
  std::cout << "delete <data_to_delete>\n\tDeletes the data as required.\n\tPossible data to delete:\n"
               "\taccount <ID_to_delete>,\n"
               "\trelation <id1> <id2>,\n"
               "\tpost <owner_ID>,\n"
               "\tlike, dislike."
            << std::endl;
  std::cout << "stats <type_of_statistic> [<type_of_data>]"
               "\n\tPrints statistic info.\n"
               "\tPossible type of statistic:\n"
               "\tnumber(<type_of_data>), most(<type_of_data>), average_age, best_ratio_post, worst_ratio_post.\n"
               "\tIn case of \"number\", the possible types of data are:\n"
               "\taccounts, users, groups, companies, friends <id>, relatives <id>, workers <id>, subsidiaries <id>, members <id>, born_after.\n"
               "\tIn case of \"most\", the possible types of data are:\n"
               "\temploying_company, employing_partnership, user_friends, user_acquaintances, liked_post, disliked_post, liked_account, disliked_account."
            << std::endl;
  std::cout << "search <data_to_search>"
               "\n\tPossible data to search:\n"
               "\ttrees (all trees), tree <id>, loner_people, friendliest_companies.\n"
            << std::endl;
  std::cout << "save\n\tSaves current status on file." << std::endl;
  std::cout << "autosave on/off\n\tTurns autosave on/off." << std::endl;
  std::cout << "exit\n\tSaves and quits the program." << std::endl;
}

void Shell::list(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  //Stampa una lista di account
  std::vector<Account> accounts(manager.getAllAccountsSorted());
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
  //Preleva informazioni su un certo ID
  if (what_to_get == "info") {
    std::string requested_id;
    command >> requested_id;
    char account_type = manager.getAccountType(
            requested_id);   //Controlla il tipo di account di cui sono richieste le informazioni
    if (account_type == 0) {                                    //Account non trovato
      std::cout << "Requested ID not found" << std::endl;
      return;
    }
    
    else if (account_type == Account::user_type) {
      User user = manager.getUser(
              requested_id);                //Prendi una copia dell'utente e stampane le informazioni
      std::cout << "Name: " << user.getName() << "\n"
                << "Surname: " << user.getSurname() << "\n"
                << "Gender: " << user.getGender() << "\n"
                << "Address: " << user.getAddress() << "\n"
                << "Birth Date: " << user.getBirth() << "\n"
                << "Subscription Date: " << user.getSubscription() << std::endl;
      
    }
    
    else if (account_type == Account::group_type) {
      Group group = manager.getGroup(requested_id);             //Prendi una copia del gruppo e stampane le informazioni
      std::cout << "Name: " << group.getName() << "\n"
                << "Legal location: " << group.getLegalLocation() << "\n"
                << "Activity: " << group.getTypeOfActivity() << "\n"
                << "Inception: " << group.getInception() << "\n"
                << "Birth Date: " << group.getSubscription() << std::endl;
    }
    
    else if (account_type == Account::company_type) {
      Company company = manager.getCompany(
              requested_id);       //Prendi una copia della compagnia e stampane le informazioni
      std::cout << "Name: " << company.getName() << "\n"
                << "Financial location: " << company.getFinancialLocation() << "\n"
                << "Operative location: " << company.getOperativeLocation() << "\n"
                << "Product: " << company.getTypeOfProduct() << "\n"
                << "Inception: " << company.getInception() << "\n"
                << "Birth Date: " << company.getSubscription() << std::endl;
    }
  }
    //Preleva una relazione tra due ID
  else if (what_to_get == "relation") {
    std::string id1, id2;                                                 //Prendi dalla linea di comando anche i due ID
    command >> id1 >> id2;
    
    if (!manager.accountExists(id1) || (!manager.accountExists(id2))) {
      cout << "Error! One the two IDs does not exist." << endl;
      return;
    }
    
    std::string relation = manager.getRelationship(id1, id2);             //Richiedi la relazione
    if (relation.empty()) {                                               //Non è presente alcuna relazione
      std::cout << "No relation found between " << id1 << " and " << id2;
      return;
    }
    std::cout << "Relation: " << relation << std::endl;                   //Stampala
  }
    
    //Preleva tutti i post di un certo ID
  else if (what_to_get == "posts") {
    std::string id;
    command >> id;                                                       //Leggi l'ID dalla linea di comando
    
    if (!manager.accountExists(id)) {
      cout << "Error! The ID: " << id << " does not exist." << endl;
      return;
    }
    
    std::vector<Post> posts = manager.getPosts(id);                      //Richiedi tutti i post dell'account
    if (posts.empty()) {
      std::cout << "No posts from " << id << " found" << std::endl;
      return;
    }
    else {
      for (int i = 0; i < posts.size(); i++) {                            //Stampa tutti i post
        std::cout << "\n" << i + 1 << ":" << std::endl                    //i = Indice del post
                  << "\n" << posts[i].getNews() << "\n" << std::endl
                  << "[" << posts[i].getDate()
                  << " " << posts[i].getTime()
                  << "]\n" << std::endl;
        
        std::vector<std::string> likes, dislikes;                       //Preparati a leggere i like e i dislike
        likes = posts[i].getLikes();
        dislikes = posts[i].getDislikes();
        
        if (!likes.empty()) {                                           //Stampa i like (se ce ne sono)
          std::cout << "Likes: ";
          for (int j = 0; j < likes.size(); j++) {
            std::cout << likes[j];
            if (j != likes.size() - 1)              //Aggiungi una virgola nella lista tranne che all'ultimo like
              std::cout << ",";
          }
          std::cout << std::endl;
        }
        
        likes.clear();                              //Il vettore non serve più
        
        if (!dislikes.empty()) {                    //Stampa i dislike se ce ne sono
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
    //Preleva gli ID oggetto della relazione. L'ID fornito è il soggetto della suddetta.
  else if (relation::isValid(what_to_get)) {
    std::string id;
    command >> id;
    
    if (!manager.accountExists(id)) {
      cout << "Error! The ID: " << id << " does not exist." << endl;
      return;
    }
    
    std::vector<std::string> relations = manager.getRelated(id, what_to_get); //Prendi una lista di tutte le relzioni del tipo richiesto
    if (relations.empty()) {
      if (what_to_get == relation::parent || what_to_get == relation::born)
        std::cout << id << " is " << what_to_get << " of no one." << std::endl;
      else
        std::cout << id << " has no " << what_to_get << "." << std::endl;
      return;
    }
    
    if (what_to_get == relation::born)
      std::cout << id << "'s parents are: " << std::endl;
    for (int i = 0; i < relations.size(); i++) {                              //Stampale
      std::cout << relations[i] << std::endl;
    }
    std::cout << std::endl;
  }
  
  else {
    std::cout << "Cannot get \"" << what_to_get << "\"" << std::endl;
    std::cout << "Possible parameters: info <id>, relation <id1> <id2>, posts <id>, <relation> <id>";
    return;
  }
}

//Imposta dati specifici
void Shell::set(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  
  string what_to_set, ID_to_set;
  command >> what_to_set >> ID_to_set;      //Acquisisci i parametri dalla linea di comando
  
  if (what_to_set.empty() || ID_to_set.empty()) {   //Controlla che i parametri siano stati effettivamente inseriti
    cout << "Error! Please specify what field to set." << endl;
    return;
  }
  
  if (!manager.accountExists(ID_to_set)) {
    cout << "Error! The ID: " << ID_to_set << " does not exist." << endl;
    return;
  }
  
  char type = manager.getAccountType(ID_to_set);    //Reperisci il tipo di account
  if (type == Account::user_type) {
    User user_old;                                  //User a cui cambiare le info
    User user_new;                                  //Utente modificato
    
    user_old = manager.getUser(ID_to_set);          //Se l'id non esiste, user_old è un default constructor
    user_new = user_old;                            //Fai momentaneamente una copia
    
    if (user_old == User()) {                       //Controlla che l'ID esista
      cout << "Error! This ID is not valid." << endl;
      return;
    }
    
    if (what_to_set == "name") {
      string new_name;
      cout << "Please insert the new " << what_to_set << "." << endl;
      getline(cin, new_name);
      if (!Account::nameValid(new_name)) {           //Controlla che sia valido
        cout << "Error! This name does not respect the rules." << endl;
      }
      user_new.setName(new_name);                   //Modifica il dato nel nuovo utente
      
      if (!manager.replaceAccount(ID_to_set, user_new)) { //Il manager non è riuscuto a rimpiazzare l'account
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "surname") {
      string new_surname;
      cout << "Please insert the new" << what_to_set << "." << endl;
      getline(cin, new_surname);
      user_new.setSurname(new_surname);                 //Acquisisci il nuovo cognome
      if (!Account::nameValid(new_surname)) {           //Controlla che sia valido
        cout << "Error! This surname does not respect the rules." << endl;
      }
      if (!manager.replaceAccount(ID_to_set, user_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "gender") {
      char new_gender;
      cout << "Please insert the new gender." << endl;
      cin >> new_gender;
      new_gender = char(toupper(new_gender));         //Converti sempre il genere in maiuscolo
      if (!gender::isValid(new_gender)) {              //Controlla che sia valido
        cout << "Error! Gender not valid." << endl;
      }
      user_new.setGender(new_gender);
      if (!manager.replaceAccount(ID_to_set, user_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "birth") {
      string new_birth;
      Date birth;
      cout << "Please insert the new birth date." << endl;
      getline(cin, new_birth);
      if (Date::CheckDate(new_birth)) {                       //Controlla che la data inserita sia valida
        birth.scanDateByStr(new_birth);                       //In quel caso assegnala
        user_new.setBirth(birth);                             //E impostala come data di nascita dell'utente
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
      getline(cin, new_addr);                         //Acquisisci l'indirizzo
      if (!Account::nameValid(new_addr)) {            //Controlla che sia valido
        cout << "Error! This address does not respect the rules." << endl;
      }
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
      getline(cin, new_sub);
      if (Date::CheckDate(new_sub)) {   //Controlla che la data sia valida
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
    data_to_delete << user_old;         //Carica la versione vecchia dell'utente nel buffer dei dati da eliminare
    new_data << user_new;               //Carica invece la versione nuova tra i dati da aggiungere
    cout << "Done!" << endl;
  }
  else if (type == Account::group_type) {
    Group group_old;
    Group group_new;
    group_old = manager.getGroup(ID_to_set);
    group_new = group_old;              //Fai momentaneamente una copia che verrà modificata in seguito
    
    if (group_new == Group()) {         //Il manager non ha trovato il gruppo
      cout << "Error! This ID is not valid." << endl;
      return;
    }
    
    if (what_to_set == "name") {
      string new_name;
      cout << "Please insert the new name." << endl;
      getline(cin, new_name);
      if (!Account::nameValid(new_name)) {           //Controlla che sia valido
        cout << "Error! This name does not respect the rules." << endl;
      }
      group_new.setName(new_name);
      if (!manager.replaceAccount(ID_to_set, group_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "subscription") {
      string new_sub;
      Date sub;
      cout << "Please insert the new date of subscription (in format dd/mm/yyyy)." << endl;
      getline(cin, new_sub);
      if (Date::CheckDate(new_sub)) {
        sub.scanDateByStr(new_sub);
        group_new.setSubscription(new_sub);
        if (!manager.replaceAccount(ID_to_set, group_new)) {
          cout << "Error! Could not modify this parameter." << endl;
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
      cout << "Please insert the new legal location." << endl;
      getline(cin, new_loc);
      if (!Account::nameValid(new_loc)) {
        std::cout << "Location inserted is not valid." << std::endl;
        return;
      }
      group_new.setLegalLocation(new_loc);
      if (!manager.replaceAccount(ID_to_set, group_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "type_activity") {
      string new_act;
      cout << "Please insert the new type of activity." << endl;
      getline(cin, new_act);
      if (!Account::nameValid(new_act)) {
        std::cout << "Error! Could not add the activity in this format." << std::endl;
      }
      group_new.setTypeOfActivity(new_act);
      if (!manager.replaceAccount(ID_to_set, group_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "inception") {
      string new_inc;
      Date inc;
      cout << "Please insert the new date of inception (in format dd/mm/yyyy)." << endl;
      getline(cin, new_inc);
      if (Date::CheckDate(new_inc)) {
        inc.scanDateByStr(new_inc);
        group_new.setInception(inc);
        if (!manager.replaceAccount(ID_to_set, group_new)) {
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
      getline(cin, new_name);
      if (!Account::nameValid(new_name)) {
        std::cout << "Name not valid." << std::endl;
        return;
      }
      company_new.setName(new_name);
      if (!manager.replaceAccount(ID_to_set, company_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "subscription") {
      string new_sub;
      Date sub;
      cout << "Please insert the new date of subscription (in format dd/mm/yyyy)." << endl;
      getline(cin, new_sub);
      if (Date::CheckDate(new_sub)) {
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
      getline(cin, new_inc);
      if (Date::CheckDate(new_inc)) {
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
      getline(cin, new_loc);
      if (!Account::nameValid(new_loc)) {
        std::cout << "Location not valid." << std::endl;
        return;
      }
      company_new.setFinancialLocation(new_loc);
      if (!manager.replaceAccount(ID_to_set, company_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "operative_location") {
      string new_loc;
      cout << "Please insert the new operative location." << endl;
      getline(cin, new_loc);
      if (!Account::nameValid(new_loc)) {
        std::cout << "Location not valid." << std::endl;
        return;
      }
      company_new.setOperativeLocation(new_loc);
      if (!manager.replaceAccount(ID_to_set, company_new)) {
        cout << "Error! " << what_to_set << " could not be modified." << endl;
        return;
      }
    }
    else if (what_to_set == "products") {
      string new_prod;
      cout << "Please insert the new type of products." << endl;
      getline(cin, new_prod);
      if (!Account::nameValid(new_prod)) {
        std::cout << "Product name not valid." << std::endl;
        return;
      }
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
  //Aggiungi un nuovo Account
  if (what_to_add == "user") {
    string tmp_n, tmp_s, tmp_id, tmp_a, d1, d2;
    Date tmp_sub, tmp_b;
    char tmp_g;
    cout << "ID:\n>";
    getline(cin, tmp_id);
    if (!Account::IDValid(tmp_id)) {
      std::cout << "ID not valid." << std::endl;
      return;
    }
    cout << "Name:\n>";
    getline(cin, tmp_n);
    if (!Account::nameValid(tmp_n)) {
      std::cout << "Name not valid." << std::endl;
      return;
    }
    cout << "Surname:\n>";
    getline(cin, tmp_s);
    if (!Account::nameValid(tmp_s)) {
      std::cout << "Surname not valid." << std::endl;
    }
    cout << "Address:\n>";
    getline(cin, tmp_a);
    if (!Account::nameValid(tmp_a)) {
      std::cout << "Address not valid." << std::endl;
      return;
    }
    cout << "Subscription:\n>";
    getline(cin, d1);
    if (!Date::CheckDate(d1)) {
      std::cout << "Date not valid." << std::endl;
      return;
    }
    tmp_sub.scanDateByStr(d1);
    cout << "Birth:\n>";
    getline(cin, d2);
    if (!Date::CheckDate(d2)) {
      std::cout << "Date not valid." << std::endl;
      return;
    }
    tmp_b.scanDateByStr(d2);
    cout << "Gender:\n"
            "Possible gender:\n"
            "'M' MALE\n"
            "'F' FEMALE\n"
            "'N' NON-BINARY\n"
            "'A' AGENDER\n"
            "'B' BIGENDER\n"
            "'O' OTHERS\n>";
    tmp_g = char(toupper(cin.get()));                                   //Converti sempre il genere in maiuscolo
    if (!gender::isValid(tmp_g)) {
      std::cout << "Gender not valid." << std::endl;
      return;
    }
    
    User new_u(tmp_n, tmp_s, tmp_id, tmp_a, tmp_sub, tmp_b, tmp_g);
    
    if (!manager.addAccount(new_u)) {
      cout << "Error! Account could not be created." << endl;
      return;
    }
    new_data << new_u;                                                  //Aggiungi il nuovo utente al buffer
  }
  else if (what_to_add == "group") {
    string tmp_n, tmp_id, tmp_loc, tmp_act, d1, d2;
    Date tmp_sub, tmp_inc;
    cout << "ID:\n>";
    getline(cin, tmp_id);
    if (!Account::IDValid(tmp_id)) {
      std::cout << "ID not valid." << std::endl;
      return;
    }
    cout << "Name:\n>";
    getline(cin, tmp_n);
    if (!Account::nameValid(tmp_n)) {
      std::cout << "Name not valid." << std::endl;
      return;
    }
    cout << "Legal location:\n>";
    getline(cin, tmp_loc);
    if (!Account::nameValid(tmp_loc)) {
      std::cout << "Location not valid." << std::endl;
      return;
    }
    cout << "Type of activity:\n>";
    getline(cin, tmp_act);
    if (!Account::nameValid(tmp_act)) {
      std::cout << "Activity not valid." << std::endl;
      return;
    }
    cout << "Subscription:\n>";
    getline(cin, d1);
    if (!Date::CheckDate(d1)) {
      std::cout << "Date not valid." << std::endl;
      return;
    }
    tmp_sub.scanDateByStr(d1);
    cout << "Inception:\n>";
    getline(cin, d2);
    if (!Date::CheckDate(d2)) {
      std::cout << "Date not valid." << std::endl;
      return;
    }
    tmp_inc.scanDateByStr(d2);
    
    Group new_g(tmp_n, tmp_id, tmp_loc, tmp_act, tmp_sub, tmp_inc);
    if (!manager.addAccount(new_g)) {
      cout << "Error! Account could not be created." << endl;
      return;
    }
    new_data << new_g;                                                  //aggiungi il nuovo gruppo al buffer
  }
  else if (what_to_add == "company") {
    string tmp_n, tmp_id, tmp_finloc, tmp_oploc, tmp_p, d1, d2;
    Date tmp_sub, tmp_inc;
    cout << "ID:\n>";
    getline(cin, tmp_id);
    if (!Account::IDValid(tmp_id)) {
      std::cout << "ID not valid." << std::endl;
      return;
    }
    cout << "Name:\n>";
    getline(cin, tmp_n);
    if (!Account::nameValid(tmp_n)) {
      std::cout << "Name not valid." << std::endl;
      return;
    }
    cout << "Financial location:\n>";
    getline(cin, tmp_finloc);
    if (!Account::nameValid(tmp_finloc)) {
      std::cout << "Financial location not valid." << std::endl;
      return;
    }
    cout << "Operative location:\n>";
    getline(cin, tmp_oploc);
    if (!Account::nameValid(tmp_oploc)) {
      std::cout << "Operative location not valid." << std::endl;
      return;
    }
    cout << "Type of product:\n>";
    getline(cin, tmp_p);
    if (!Account::nameValid(tmp_p)) {
      std::cout << "Product not valid." << std::endl;
      return;
    }
    cout << "Subscription:\n>";
    getline(cin, d1);
    if (!Date::CheckDate(d1)) {
      std::cout << "Date not valid." << std::endl;
      return;
    }
    tmp_sub.scanDateByStr(d1);
    cout << "Inception:\n>";
    getline(cin, d2);
    if (!Date::CheckDate(d2)) {
      std::cout << "Date not valid." << std::endl;
      return;
    }
    tmp_inc.scanDateByStr(d2);
    
    Company new_c(tmp_n, tmp_id, tmp_finloc, tmp_oploc, tmp_p, tmp_sub, tmp_inc);
    if (!manager.addAccount(new_c)) {
      cout << "Error! Account could not be created." << endl;
      return;
    }
    new_data << new_c;  //Metti la nuova compagnia nel buffer
  }                                           //Add Relationship
  else if (what_to_add == "relation") {
    int error;
    string who1, who2, type_rel;
    command >> who1 >> type_rel >> who2;
    if (who1.empty() || who2.empty() || type_rel.empty()) {
      cout << "Error! Please insert the relation in form <id1> <relation_to> <id2>." << endl;
      return;
    }
    
    string existing_relation = manager.getRelationship(who1, who2);
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
              "\tworking_with (C - C)\n"
              "\tworker (U -> C)\n"
              "\temployer (C -> U)\n" << endl;
      return;
    }
    else if (error == -4) {
      cout << "Error! The relationship and the Users' ages are not compatible!" << endl;
      return;
    }
    else if (error == -5) {
      cout << "Error! The relationship is incoherent with the Accounts' types!" << endl;
      return;
    }
    new_data
            << std::make_pair(std::make_pair(who1, who2), type_rel);                 //In caso di successo, salva la relazione
    data_to_delete << std::make_pair(std::make_pair(who1, who2), existing_relation);  //E sovrascrivi quella vecchia
  }
    //Aggiungi un nuovo Post
  else if (what_to_add == "post") {
    string news, d_t, whose_ID;
    std::string input;
    std::set<std::string> likes, dislikes;
    Post post_tmp;
    
    cout << "News:\n>";
    getline(cin, news);
    post_tmp.setNews(news);
    
    cout << "Date and time:\n>";
    getline(cin, d_t);
    if (!post_tmp.setDate_Time(d_t)) {          //Controllo che data e ora siano valide
      cout << "\nError! Date or time not valid." << endl;
      return;
    }
    
    cout << "Likes (insert one user per line. Use '-' to end the list):\n";
    while (input != "-") {
      cout << ">";
      getline(cin, input);
      
      if (input != "-" && !input.empty()) {
        std::string id;
        std::stringstream(input) >> id;       //Se vengono inseriti spazi o altre robe, ignorale
        if(manager.getAccountType(id) != Account::user_type) {
          std::cout << "Error! Account " << id << " is not a User or doesn't exist." << std::endl;
          return;
        }
        likes.insert(id);
      }
    }
    post_tmp.setLikes(likes);
    
    input.clear();                                //Altrimenti non entra nel ciclo dei dislikes
    
    cout << "Dislikes (insert one user per line. Use '-' to end the list):\n";
    while (input != "-") {
      cout << ">";
      getline(cin, input);
      if (input != "-" && !input.empty()) {
        std::string id;
        std::stringstream(input) >> id;
        if(manager.getAccountType(id) != Account::user_type) {
          std::cout << "Error! Account " << id << " is not a User or doesn't exist." << std::endl;
          return;
        }
        dislikes.insert(id);
      }
    }
    post_tmp.setDislikes(dislikes);
    
    cout << "Who wrote the post?:\n>";
    getline(cin, whose_ID);
    
    if (!manager.accountExists(whose_ID)) {
      cout << "Error! Your ID does not exist." << endl;
      return;
    }
    
    if (!manager.addPost(post_tmp, whose_ID)) { //addPost controlla che l'aggiunzione sia possibile
      cout << "Could not create post." << endl;
      return;
    }
    new_data << std::make_pair(whose_ID, post_tmp);
  }                                          //Aggiungi una nuova interazione ad un certo post
  else if (what_to_add == "like" || what_to_add == "dislike") {
    string post_owner, reaction_id, post_num_str;
    unsigned int post_num;
    
    cout << "Please insert the post owner's ID:" << endl;
    cout << ">";
    getline(cin, post_owner);
    
    if (!manager.accountExists(post_owner)) {
      cout << "Error! The ID: " << post_owner << " does not exist." << endl;
      return;
    }
    
    cout << "Insert the account reacting to the post's ID:" << endl;
    cout << ">";
    getline(cin, reaction_id);
    
    if (!manager.accountExists(reaction_id)) {
      cout << "Error! The ID: " << reaction_id << " does not exist." << endl;
      return;
    }
    
    cout << "Please insert post number:" << endl;
    cout << ">";
    getline(cin, post_num_str);
    post_num = unsigned(stoi(post_num_str));
    
    post_num--;
    
    vector<Post> posts = manager.getPosts(post_owner);
    if (posts.size() <= post_num) {   //Controlla se il proprietario dell'account ha il post
      cout << "Error! Could find post " << post_num + 1 << " from " << post_owner << "." << endl;
      return;
    }
    
    Post &old_post = posts[post_num];
    
    bool like = what_to_add == "like";      //Decidi se mettere like o dislike
    if (!manager.setReaction(like, true, post_owner, post_num, reaction_id)) {
      cout << "Error! Could find post " << post_num + 1 << " from " << post_owner << "." << endl;
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
  }                                          //Elimina un certo Account
  if (what_to_delete == "account") {
    string ID_to_delete;
    command >> ID_to_delete;
    if (ID_to_delete.empty()) {
      cout << "Error! Missing parameter." << endl;
      return;
    }
    if (!manager.accountExists(ID_to_delete)) {
      cout << "Error! The ID: " << ID_to_delete << " does not exist." << endl;
      return;
    }
    
    std::vector<Post> posts;
    std::vector<IOBuffer::Relation> relations;
    std::vector<pair<pair<string, string>, string>> all_relationships;
    std::vector<pair<string, vector<Post>>> post_without_react, post_with_react;
    
    posts = manager.getPosts(ID_to_delete);
    for (auto it = posts.begin(); it != posts.end(); it++) {
      data_to_delete << make_pair(ID_to_delete, *it); //Elimino i post redatti dall'account da eliminare
    }
    
    all_relationships = manager.getAllRelationships(ID_to_delete);
    for (auto it = all_relationships.begin(); it != all_relationships.end(); it++) {
      data_to_delete << *it; //Elimino le relazioni in cui l'account occorre
    }
    
    post_with_react = manager.getPostsReactedBy(ID_to_delete);             //vettore di post pre-eliminazione
    post_without_react = manager.getPostsWithoutReactionsOf(ID_to_delete); //vettore di post post-eliminazione
    
    //Elimino dai post generali le occorrenze nelle interazioni dell'account da eliminare
    for (auto it_new1 = post_without_react.begin(); it_new1 != post_without_react.end(); it_new1++) {
      for (auto it_new2 = it_new1->second.begin();
           it_new2 != it_new1->second.end(); it_new2++) {
        new_data << make_pair(it_new1->first, *it_new2);
      }
    }
    
    for (auto it_del1 = post_with_react.begin(); it_del1 != post_with_react.end(); it_del1++) {
      for (auto it_del2 = it_del1->second.begin();
           it_del2 != it_del1->second.end(); it_del2++) {
        data_to_delete << make_pair(it_del1->first, *it_del2);
      }
    }
    
    //Ricavo il tipo di account così da poterlo passare al buffer di eliminazione opportunamente
    char type = manager.getAccountType(ID_to_delete);
    if (type == Account::user_type) {
      User temp = manager.getUser(ID_to_delete);
      if (manager.deleteAccount(ID_to_delete)) {
        data_to_delete << temp;
      }
    }
    else if (type == Account::group_type) {
      Group temp = manager.getGroup(ID_to_delete);
      if (manager.deleteAccount(ID_to_delete)) {
        data_to_delete << temp;
      }
    }
    else if (type == Account::company_type) {
      Company temp = manager.getCompany(ID_to_delete);
      if (manager.deleteAccount(ID_to_delete)) {
        data_to_delete << temp;
      }
    }
    else {
      std::cout << "Account not valid." << std::endl;
      return;
    }
  }                                          //Elimina una relazione tra due ID
  else if (what_to_delete == "relation") {
    string id_start, id_target;
    command >> id_start >> id_target;
    std::string relation = manager.getRelationship(id_start, id_target);
    if (!manager.deleteRelationship(id_start, id_target)) {
      cout << "Error! IDs not found." << endl;
      return;
    }
    data_to_delete << std::make_pair(std::make_pair(id_start, id_target), relation);
  }                                          //Elimina un post in una determinata posizione
  else if (what_to_delete == "post") {
    std::string post_owner;
    string post_num_str;
    unsigned int post_num;
    command >> post_owner;
    
    if (!manager.accountExists(post_owner)) {
      cout << "Error! The ID: " << post_owner << " does not exist." << endl;
      return;
    }
    
    cout << "Please insert post number:" << endl;
    cout << ">";
    getline(cin, post_num_str);
    post_num = unsigned(stoi(post_num_str));
    
    post_num--;
    
    std::vector<Post> posts = manager.getPosts(post_owner);
    if (posts.size() <= post_num) {
      std::cout << "Error! " << post_owner << " does not have any post at pos " << post_num + 1 << std::endl;
      return;
    }
    if (!manager.deletePost(post_owner, post_num)) {
      cout << "Error! Post " << post_num + 1 << " from " << post_owner << " could not be deleted." << endl;
      return;
    }
    data_to_delete << std::make_pair(post_owner, posts[post_num]);
  }                                          //Elimina una interazione in un certo post
  else if (what_to_delete == "like" || what_to_delete == "dislike") {
    string post_owner, reaction_id, post_num_str;
    unsigned int post_num;
    vector<Post> posts;
    cout << "Please insert the post owner's ID:" << endl;
    cout << ">";
    getline(cin, post_owner);
    
    if (!manager.accountExists(post_owner)) {
      cout << "Error! The ID: " << post_owner << " does not exist." << endl;
      return;
    }
    
    cout << "Insert the account reacting to the post's ID:" << endl;
    cout << ">";
    getline(cin, reaction_id);
    
    if (!manager.accountExists(reaction_id)) {
      cout << "Error! The ID: " << reaction_id << " does not exist." << endl;
      return;
    }
    
    cout << "Please insert post number:" << endl;
    cout << ">";
    getline(cin, post_num_str);
    post_num = unsigned(stoi(post_num_str));
    
    post_num--;
    posts = manager.getPosts(post_owner);   //Acquisisco il vettore di post per controlli e per estrarre quello interessato
    if (posts.size() <= post_num) {   //Controlla se il proprietario dell'account ha il post
      cout << "Error! Could find post " << post_num + 1 << " from " << post_owner << "." << endl;
      return;
    }
    
    Post &old_post = posts[post_num];   //Salvo il post pre-eliminazione per passarlo al buffer delle eliminazioni
    
    bool like = what_to_delete == "like";      //Decidi se mettere like o dislike
    if (!manager.setReaction(like, false, post_owner, post_num, reaction_id)) {
      cout << "Error! Could find this reaction in post " << post_num + 1 << " from " << post_owner << "." << endl;
      return;
    }
    data_to_delete << std::make_pair(post_owner, old_post);
    new_data << std::make_pair(post_owner, manager.getPosts(post_owner)[post_num]);
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
  if (param1 == "number") {             //Statistics -- Number of...
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
      
      if (!manager.accountExists(id)) {
        cout << "Error! The ID: " << id << " does not exist." << endl;
        return;
      }
      if (num == 0) {
        cout << id << " has no friends." << endl;
        return;
      }
    }
    else if (param2 == "relatives") {
      command >> id;
      num = manager.NumRelatives(id);
      
      if (!manager.accountExists(id)) {
        cout << "Error! The ID: " << id << " does not exist." << endl;
        return;
      }
      
      if (num == 0) {
        cout << id << " has no relatives." << endl;
        return;
      }
    }
    else if (param2 == "workers") {
      command >> id;
      num = manager.NumEmployees(id);
      
      if (!manager.accountExists(id)) {
        cout << "Error! The ID: " << id << " does not exist." << endl;
        return;
      }
      
      if (num == 0) {
        cout << id << " has no workers." << endl;
        return;
      }
    }
    else if (param2 == "subsidiaries") {
      command >> id;
      num = manager.NumSubsidiaries(id);
      
      if (!manager.accountExists(id)) {
        cout << "Error! The ID: " << id << " does not exist." << endl;
        return;
      }
      
      if (num == 0) {
        cout << id << " has no subsidiaries." << endl;
        return;
      }
    }
    else if (param2 == "members") {
      command >> id;
      num = manager.NumMembers(id);
      
      if (!manager.accountExists(id)) {
        cout << "Error! The ID: " << id << " does not exist." << endl;
        return;
      }
      
      if (num == 0) {
        cout << id << " has no members." << endl;
        return;
      }
    }
    else if (param2 == "born_after") {
      string date;
      Date born_d;
      cout << "Please insert the starting date (in format dd/mm/yyyy):\n>";
      cin >> date;
      if (!Date::CheckDate(date)) {
        cout << "Error! The date is not valid." << endl;
        return;
      }
      born_d.scanDateByStr(date);
      num = manager.NumBornAfter(born_d);
      cout << "The number of people born after this date is: " << num << endl;
      return;
    }
    else {
      cout << "Error! Cannot get number of " << param2 << "." << endl;
      return;
    }
    cout << "The number of " << param2 << " is: " << num << endl;
  }
  else if (param1 == "most") {                //Statistics -- Most...
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
      cout << "The Most Liked Account is:\n" << manager.MostLiked_DislikedAccount(true);
    }
    else if (param2 == "disliked_account") {
      cout << "The Most Disliked Account is:\n" << manager.MostLiked_DislikedAccount(false);
    }
    else {
      cout << "Error! Cannot get statistscs of \"" << param2 << "\"." << endl;
      return;
    }
  }
  else if (param1 == "average_age") {           //Statistics -- Average Age
    cout << "The Average of Users' ages is:\n"
         << manager.UsersAverageAge() << endl;
  }
  else if (param1 == "best_ratio_post") {       //Statistics -- Best and Worst Ratio
    pair<string, Post> best_post;
    best_post = manager.RatioReactionPost(true);
    cout << "The Post with the best Like/Dislike ratio is:\n" << best_post.second << endl
         << "By: " << best_post.first;
  }
  else if (param1 == "worst_ratio_post") {
    pair<string, Post> worst_post;
    worst_post = manager.RatioReactionPost(false);
    cout << "The Post with the worst Like/Dislike ratio is:\n" << worst_post.second << endl
         << "By: " << worst_post.first;
  }
  else {
    std::cout << "Cannot get statistics of " << param1 << std::endl;
  }
}

void Shell::search(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete) {
  string what_to_search;
  command >> what_to_search;
  if (what_to_search == "trees") {                //Search all genealogical trees
    std::cout << "Searching trees..." << std::endl;
    vector<string> trees = manager.PrintAllTrees();               //Genera il vettore degli alberi
    if (trees.empty()) {                                          //Controlla che siano stati trovati degli alberi
      std::cout << "No trees found." << std::endl;
      return;
    }
    
    for (auto it = trees.begin(); it != trees.end(); it++) {
      cout << *it << endl;
    }
    
    std::cout << "Saving trees..." << std::endl;
    std::ofstream f;
    for (int i = 0;
         i < trees.size(); i++) {                      //Per ogni elemento del vettore stampa in un file diverso
      std::string name = "tree" + std::to_string(i + 1) + ".txt";
      f.open(name, std::ios::out | std::ios::trunc);
      f << trees[i];
      f.flush();
      f.close();
    }
  }                                             //Search a specific genealogical tree
  else if (what_to_search == "tree") {
    string id;
    command >> id;
    
    if (!manager.accountExists(id)) {
      cout << "Error! The ID: " << id << " does not exist." << endl;
      return;
    }
    
    if (id.empty() || manager.getAccountType(id) != Account::user_type) {
      std::cout << "Please insert some User's ID" << std::endl;
      return;
    }
    
    std::string tree = manager.PrintTree(id);
    
    if (tree.empty()) {
      cout << "This tree is not valid because it is not long enough!" << endl;
      return;
    }
    cout << tree;
    std::cout << "Save this tree? (yes/no)" << std::endl << ">"; //Richiesta di salvataggio su file
    std::string answer;
    std::getline(std::cin, answer);
    if (answer == "yes") {
      std::cout << "Filename: " << std::endl << ">";
      std::string filename;
      std::getline(std::cin, filename);
      std::ofstream file;
      file.open(filename + ".txt", std::ios::out | std::ios::trunc);
      file << tree;
      file.flush();
      file.close();
    }
    else {
      return;
    }
  }
  else if (what_to_search == "loner_people") {
    unsigned int relations, memberships, reactions;
    string ans; //answer
    bool unemployed;
    vector<string> lon_people;
    
    cout << "Please insert the parameters that define a loner person:\n"
            "Minimum number of relations: ";
    cin >> relations;
    cout << "Minimum number of groups: ";
    cin >> memberships;
    cout << "Does the user need to be unemployed? (yes/no): ";
    cin >> ans;
    if (ans == "yes") {
      unemployed = true;
    }
    else if (ans == "no") {
      unemployed = false;
    }
    else {
      std::cout << "Invalid answer." << std::endl;
      return;
    }
    cout << "Minimum number of added reactions: ";
    cin >> reactions;
    cin.ignore();
    
    lon_people = manager.LonerPeople(relations, memberships, unemployed, reactions);
    for (auto it = lon_people.begin(); it != lon_people.end(); it++) {
      cout << *it << endl;
    }
  }
  else if (what_to_search == "friendliest_companies") {
    float threshold;
    bool with_partners;
    std::string ans;
    std::cout << "Please insert the minimum ratio of likes/total_reactions: " << std::endl << ">";
    std::cin >> threshold;
    std::cout << "Should partners be considered? (yes/no)" << std::endl << ">";
    std::cin >> ans;
    std::cin.ignore();
    if (ans == "yes") {
      with_partners = true;
    }
    else if (ans == "no") {
      with_partners = false;
    }
    else {
      std::cout << "Invalid answer." << std::endl;
      return;
    }
    std::vector<std::pair<std::string, float>> companies = manager.SortedNewsRatioCompanies(with_partners, threshold);
    if (companies.empty()) {
      std::cout << "No company respected the threshold." << std::endl;
      return;
    }
    std::cout << "List:" << std::endl;
    for (int i = 0; i < companies.size(); i++) {
      std::cout << companies[i].first << "\t\t" << companies[i].second << std::endl;
    }
  }
  else {
    cout << "Error! Cannot search for " << what_to_search << "." << endl;
    return;
  }
  std::cout << "Done!" << std::endl;
}
