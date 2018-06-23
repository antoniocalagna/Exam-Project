#include "Post.h"
#include "User.h"
#include "Account.h"
#include "Manager.h"
#include "FileHandler.h"

using namespace std;

int main_di_clara(/*int argc, char *argv[]*/) {
  FH::FileHandler accountsFH("Accounts_TEST.dat"),
          relationsFH("Relations_TEST.dat"),
          postsFH("Posts_TEST.dat");
  IOBuffer buffer;                      //Buffer necessario per l'acquisizione dei dati
  
  /* //Controllo dei parametri passati da linea di comando
   if (argc != 0 && argc != 4) {
     cerr << "Parameters error. Plese insert input file names as follows: <accounts_file> <relations_file> <posts_file>.\n"
          << "You can also execute the program without specifying the files to open, and choose them later." << endl;
     return -1;
   }
   
   if(argc == 3) {
     //Apri e controlla i file richiesti.
     accounts_fh.open(argv[1]);
     relations_fh.open(argv[2]);
     accounts_fh.open(argv[3]);
   }*/
  
  std::cout << "Beginning checks:" << std::endl;
  //Controllo del file degli account
  FH::Error check_results = accountsFH.checkFile(FH::accountsFile);
  if (check_results.code == 0) {
    std::cout << "* Accounts file correctly formatted and ready to be read." << std::endl;
  }
  else {
    std::cerr << "** Accounts file returned error code " << check_results.code << " at line " << check_results.data << "."
              << "File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  //Controllo del file delle relazioni
  check_results = relationsFH.checkFile(FH::relationsFile);
  if (check_results.code == 0) {
    std::cout << "* Relations file correctly formatted and ready to be read." << std::endl;
  }
  else {
    std::cerr << "** Relations file returned error code " << check_results.code << " at line " << check_results.data << "."
              << "File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  //Controllo del file dei post
  check_results = postsFH.checkFile(FH::postsFile);
  if (check_results.code == 0) {
    std::cout << "* Posts file correctly formatted and ready to be read." << std::endl;
  }
  else {
    std::cerr << "** Posts file returned error code " << check_results.code << " at line " << check_results.data << "."
              << "File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  //Controlli completati
  std::cout << "Checks completed.\n" << std::endl;
  
  std::cout << "Beginning acquisition: " << std::endl;
  
  accountsFH.fetchData(FH::accountsFile, buffer);
  relationsFH.fetchData(FH::relationsFile, buffer);
  postsFH.fetchData(FH::postsFile, buffer);
  
  Manager manager;
  while (!buffer.usersEmpty()) {
    User user_tmp;
    buffer >> user_tmp;
    if (!manager.addAccount(user_tmp)) {
      //sto cercando di inserire un id già presente
    }
  }
  while (!buffer.groupsEmpty()) {
    Group group_tmp;
    buffer >> group_tmp;
    if (!manager.addAccount(group_tmp)) {
      //sto cercando di inserire un id già presente
    }
  }
  while (!buffer.companiesEmpty()) {
    Company comp_tmp;
    buffer >> comp_tmp;
    if (!manager.addAccount(comp_tmp)) {
      //sto cercando di inserire un id già presente
    }
  }
  while (!buffer.relationsEmpty()) {
    IOBuffer::Relation relation_tmp;
    buffer >> relation_tmp;
    string id_start, id_target, relationship;
    id_start = relation_tmp.first.first;
    id_target = relation_tmp.first.second;
    relationship = relation_tmp.second;
    if (!manager.addRelationship(id_start, id_target, relationship)) {
    
    }
  }
  while (!buffer.postsEmpty()) {
    IOBuffer::m_Post post_tmp;
    buffer >> post_tmp;
    if (!manager.addPost(post_tmp.second, post_tmp.first)) {
    
    }
    
  }
  std::cout << "Acquisition completed.\n" << std::endl;
  
  string input;
  string cmd;
  bool exit = false;
  do {
    cout << "\n>";
    getline(cin, input);
    
    stringstream command(input);
    command >> cmd;
    if (cmd == "help") {
      //LISTA DEI POSSIBILI COMANDI
      cout << "Users:" << endl;
      cout
              << "\nset user name\nset user surname\nset user gender\nset user birth\nset user address\nset user id\nset user subscription\n"
              << endl
              << "get info <id>\n"
              << "get posts <id>\n"
              << "get relation <id1> <id2>\n"
              << endl;
      cout << "add account user\n"
           << endl;
      
      cout << "Groups:" << endl;
      cout
              << "\nset name <group_id>\nset n_members <group_id>\nset location <group_id>\nset type_activity <group_id>\nset inception <group_id>\nset id <group_id>\nset subscription <group_id>\n"
              << endl
              << "get info <id>\n"
              << "get posts <id>\n"
              << "get relation <id1> <id2>\n"
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
    else if (cmd == "set") {
      string what, who; //non so se questi nomi sono messi per scherzo, a me sembrano carini
      command >> who >> what;
      if (what.empty()) {
        cout << "Error! I do not understand what information you'd like to set." << endl;
      }
      else {
        if (who == "user") {
          string id_to_set;
          User user_to_set; //user a cui vuoi cambiare info
          cout << "Please insert the ID whose User you'd like to set info about:\n>" << endl;
          cin >> id_to_set; //id "vecchio" account
          user_to_set = manager.getUser(id_to_set); //se l'id non esiste, user_to_set è un default constructor
          if (user_to_set == User()) {
            cout << "Error! This ID is not valid." << endl;
          }
          else {
            if (what == "name") {
              string new_name;
              cout << "Please insert the new" << what << "." << endl;
              //cin.ignore(); da provare se ci vada o no
              getline(cin, new_name);
              user_to_set.setName(new_name);
              if (!manager.replaceAccount(id_to_set, user_to_set)) { //da provare
                cout << "Error! I could not modify your information." << endl;
              }
              else {
                cout << "Done!" << endl;
              }
            }
            else if (what == "surname") {
              string new_surname;
              cout << "Please insert the new" << what << "." << endl;
              //cin.ignore();
              getline(cin, new_surname);
              user_to_set.setSurname(new_surname);
              if (!manager.replaceAccount(id_to_set, user_to_set)) {
                cout << "Error! I could not modify your information." << endl;
              }
              else {
                cout << "Done!" << endl;
              }
              
            }
            else if (what == "gender") {
              char new_gender;
              cout << "Please insert the new gender." << endl;
              //cin.ignore();
              cin >> new_gender;
              user_to_set.setGender(new_gender);
              if (!manager.replaceAccount(id_to_set, user_to_set)) {
                cout << "Error! I could not modify your information." << endl;
              }
              else {
                cout << "Done!" << endl;
              }
              
            }
            else if (what == "birth") {
              string new_birth;
              Date birth;
              cout << "Please insert the new" << what << "." << endl;
              //cin.ignore();
              getline(cin, new_birth);
              if (birth.CheckDate(new_birth)) {
                birth.scanDateByStr(new_birth);
                user_to_set.setBirth(birth);
                if (!manager.replaceAccount(id_to_set, user_to_set)) {
                  cout << "Error! I could not modify your information." << endl;
                }
                else {
                  cout << "Done!" << endl;
                }
              }
              else {
                cout << "Error! Date is not valid." << endl;
              }
              
            }
            else if (what == "address") {
              string new_addr;
              cout << "Please insert the new address." << endl;
              //cin.ignore();
              getline(cin, new_addr);
              user_to_set.setAddress(new_addr);
              if (!manager.replaceAccount(id_to_set, user_to_set)) {
                cout << "Error! I could not modify your information." << endl;
              }
              else {
                cout << "Done!" << endl;
              }
              
            }
            else if (what == "id") {
              string new_id;
              cout << "Please insert the new ID." << endl;
              //cin.ignore();
              getline(cin, new_id);
              user_to_set.setID(new_id);
              if (manager.deleteAccount(
                      new_id)) { //questo l'ho fatto "a mano" perchè non penso che con la funzione si possa fare
                if (!manager.addAccount(user_to_set)) {
                  cout << "Error! I could not modify your information." << endl;
                }
                else {
                  cout << "Done!" << endl;
                }
              }
              else {
                cout << "Error! The ID does not exist." << endl;
              }
              
            }
            else if (what == "subscription") {
              string new_sub;
              Date sub;
              cout << "Please insert the new date of subscription (in format dd/mm/yyyy)." << endl;
              //cin.ignore();
              getline(cin, new_sub);
              if (sub.CheckDate(new_sub)) {
                sub.scanDateByStr(new_sub);
                user_to_set.setSubscription(new_sub);
                if (!manager.replaceAccount(id_to_set, user_to_set)) {
                  cout << "Error! I could not modify your information." << endl;
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
        else if (who == "group") {
          string id_to_set;
          Group group_to_set;
          cout << "Please insert the ID whose Group you'd like to set info about:\n>" << endl;
          cin >> id_to_set; //id "vecchio" account
          group_to_set = manager.getGroup(id_to_set);
          if (group_to_set == Group()) {
            cout << "Error! This ID is not valid." << endl;
          }
          if (what == "name") {
            string new_name;
            cout << "Please insert the new name." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_name);
            group_to_set.setName(new_name);
            if (!manager.replaceAccount(id_to_set, group_to_set)) { //da provare
              cout << "Error! I could not modify your information." << endl;
            }
            else {
              cout << "Done!" << endl;
            }
            
          }
          else if (what == "id") {
            string new_id;
            cout << "Please insert the new ID." << endl;
            //cin.ignore();
            getline(cin, new_id);
            group_to_set.setID(new_id);
            if (manager.deleteAccount(
                    new_id)) { //questo l'ho fatto "a mano" perchè non penso che con la funzione si possa fare
              if (!manager.addAccount(group_to_set)) {
                cout << "Error! This ID already exists." << endl;
              }
              else {
                cout << "Done!" << endl;
              }
            }
            else {
              cout << "Error! This ID does not exist." << endl;
            }
            
          }
          else if (what == "subscription") {
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
          else if (what == "location") {
            string new_loc;
            cout << "Please insert the new location." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_loc);
            group_to_set.setLegalLocation(new_loc);
            if (!manager.replaceAccount(id_to_set, group_to_set)) { //da provare
              cout << "Error! I could not modify your information." << endl;
            }
            else {
              cout << "Done!" << endl;
            }
            
          }
          else if (what == "type_activity") {
            string new_act;
            cout << "Please insert the new type of activity." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_act);
            group_to_set.setTypeOfActivity(new_act);
            if (!manager.replaceAccount(id_to_set, group_to_set)) { //da provare
              cout << "Error! I could not modify your information." << endl;
            }
            else {
              cout << "Done!" << endl;
            }
            
          }
          else if (what == "inception") {
            string new_inc;
            Date inc;
            cout << "Please insert the new date of inception (in format dd/mm/yyyy)." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_inc);
            if (inc.CheckDate(new_inc)) {
              inc.scanDateByStr(new_inc);
              group_to_set.setInception(inc);
              if (!manager.replaceAccount(id_to_set, group_to_set)) { //da provare
                cout << "Error! I could not modify your information." << endl;
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
        else if (who == "company") {
          string id_to_set;
          Company company_to_set;
          cout << "Please insert the ID whose Company you'd like to set info about:\n>" << endl;
          cin >> id_to_set; //id "vecchio" account
          company_to_set = manager.getCompany(id_to_set);
          if (company_to_set == Company()) {
            cout << "Error! This ID is not valid." << endl;
          }
          
          if (what == "name") {
            string new_name;
            cout << "Please insert the new name." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_name);
            company_to_set.setName(new_name);
            if (!manager.replaceAccount(id_to_set, company_to_set)) { //da provare
              cout << "Error! I could not modify your information." << endl;
            }
            else {
              cout << "Done!" << endl;
            }
            
          }
          else if (what == "id") {
            string new_id;
            cout << "Please insert the new ID." << endl;
            //cin.ignore();
            getline(cin, new_id);
            company_to_set.setID(new_id);
            if (manager.deleteAccount(new_id)) {
              if (!manager.addAccount(company_to_set)) {
                cout << "Error! This ID already exists." << endl;
              }
              else {
                cout << "Done!" << endl;
              }
              
            }
            else if (what == "subscription") {
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
            
          }
          else if (what == "inception") {
            string new_inc;
            Date inc;
            cout << "Please insert the new date of inception (in format dd/mm/yyyy)." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_inc);
            if (inc.CheckDate(new_inc)) {
              inc.scanDateByStr(new_inc);
              company_to_set.setInception(inc);
              if (!manager.replaceAccount(id_to_set, company_to_set)) {
                cout << "Error! I could not modify your information." << endl;
              }
              else {
                cout << "Done!" << endl;
              }
            }
            else {
              cout << "Error! This date is not valid." << endl;
            }
            
          }
          else if (what == "financial_location") {
            string new_loc;
            cout << "Please insert the new financial location." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_loc);
            company_to_set.setFinancialLocation(new_loc);
            if (!manager.replaceAccount(id_to_set, company_to_set)) {
              cout << "Error! I could not modify your information." << endl;
            }
            else {
              cout << "Done!" << endl;
            }
            
          }
          else if (what == "operative_location") {
            string new_loc;
            cout << "Please insert the new operative location." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_loc);
            company_to_set.setOperativeLocation(new_loc);
            if (!manager.replaceAccount(id_to_set, company_to_set)) {
              cout << "Error! I could not modify your information." << endl;
            }
            else {
              cout << "Done!" << endl;
            }
            
          }
          else if (what == "products") {
            string new_prod;
            cout << "Please insert the new type of products." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_prod);
            company_to_set.setTypeOfProduct(new_prod);
            if (!manager.replaceAccount(id_to_set, company_to_set)) {
              cout << "Error! I could not modify your information." << endl;
            }
            else {
              cout << "Done!" << endl;
            }
            
          }
        }
      }
    }
    
    //Get
    else if (cmd == "get") {
      string what_to_get;
      command >> what_to_get;
      if (what_to_get == "info") {
        std::string requested_id;
        command >> requested_id;
        
        char account_type = manager.getAccountType(requested_id);   //Controlla il tipo di account di cui sono richieste le informazioni
        if (account_type == 0) {
          std::cout << "Requested ID not found" << std::endl;
        }
          //Get info - user
        else if (account_type == Account::user_type) {
          User user = manager.getUser(requested_id);
          std::cout << "Name: " << user.getName() << "\n"
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
      else if (what_to_get == "relation") {
        std::string id1, id2;
        command >> id1 >> id2;
        std::string relation = manager.getRelationship(id1, id2);
        if (relation.empty()) {
          std::cout << "No relation found between " << id1 << " and  " << id2;
        }
        else {
          std::cout << "Relation: " << relation << std::endl;
        }
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
      }   //End of get posts
      //Get <something_not_valid>
      else {
        std::cout << "Cannot get \"" << what_to_get << "\"" << std::endl;
        std::cout << "Possible parameters:\ninfo, relation, posts";
      }
    }
    else if (cmd == "delete") {
      string what;
      command >> what;
      if (what.empty()) {
        cout << "Error! I do not understand what you would like to delete." << endl;
      }
      if (what == "account") {
        string who;
        command >> who;
        if (who.empty()) {
          cout << "Error! I do not understand the ID whose User you'd like to delete." << endl;
        }
        else {
          manager.deleteAccount(who);
        }
        
      }
      else if (what == "relationship") {
        string id_start, id_target;
        cout << "Please insert: <id_subject> <id_target>:\n";
        cin >> id_start >> id_target;
        if (id_start.empty() || id_target.empty()) {
          cout << "Error! I do not understand which IDs I have to work with." << endl;
        }
        if (!manager.deleteRelationship(id_start, id_target)) {
          cout << "Error! One or both of your IDs don't exist." << endl;
        }
      }
      else if (what == "post") {
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
      }
      else if (what == "like") {
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
        }
        else {
          cout << "Error! I could not remove this like" << endl; //1-NO ID, 2-NO AUTOLIKES, 3-NO POST
        }
        
      }
      else if (what == "dislike") {
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
        }
        else {
          cout << "Error! I could not remove this dislike" << endl; //1-NO ID, 2-NO AUTOLIKES, 3-NO POST
        }
      }
      else {
        cout << "I do not understand what you'd like to delete." << endl;
      }
    }
    else if (cmd == "add") {
      string what1, what2;
      command >> what1 >> what2;
      if (what1.empty()) {
        cout << "Error! I do not understand what you would like to add. Please retry!" << endl;
      }
      else {
        
        if (what1 == "account") {
          if (what2.empty()) {
            cout << "Error! I do not understand what you would like to add. Please retry!" << endl;
          }
          else if (what2 == "user") {
            string tmp_n, tmp_s, tmp_id, tmp_a, d1, d2;
            Date tmp_sub, tmp_b;
            char tmp_g;
            cout << "Name:\n>";
            cin >> tmp_n;
            cout << "Surname:\n>";
            cin >> tmp_s;
            cout << "Id:\n>";
            cin >> tmp_id;
            cout << "Address:\n>";
            cin.ignore();
            getline(cin, tmp_a);
            cout << "Subscription:\n>";
            cin >> d1;
            tmp_sub.scanDateByStr(d1);
            cout << "Birth:\n>";
            cin >> d2;
            tmp_b.scanDateByStr(d2);
            cout << "Gender:\n"
                    "Possible gender:\n"
                    "'M' MALE\n"
                    "'F' FEMALE\n"
                    "'N' NON-BINARY\n"
                    "'A' AGENDER\n"
                    "'B' BIGENDER\n"
                    "'O' OTHERS\n>";
            cin >> tmp_g;
            
            User new_u(tmp_n, tmp_s, tmp_id, tmp_a, tmp_sub, tmp_b, tmp_g);
            if (!manager.addAccount(new_u)) {
              cout << "Error! Your ID already exists!";
            }
            else cout << "Done!";
            
            getline(cin, input);
          }
          else if (what2 == "group") {
            string tmp_n, tmp_id, tmp_loc, tmp_act, d1, d2;
            Date tmp_sub, tmp_inc;
            cout << "Name:\n>";
            cin >> tmp_n;
            cout << "Id:\n>";
            cin >> tmp_id;
            cout << "Legal location:\n>";
            cin.ignore();
            getline(cin, tmp_loc);
            cout << "Type of activity:\n>";
            cin.ignore();
            getline(cin, tmp_act);
            cout << "Subscription:\n>";
            cin >> d1;
            tmp_sub.scanDateByStr(d1);
            cout << "Inception:\n>";
            cin >> d2;
            tmp_inc.scanDateByStr(d2);
            
            Group new_g(tmp_n, tmp_id, tmp_loc, tmp_act, tmp_sub, tmp_inc);
            if (!manager.addAccount(new_g)) {
              cout << "Error! Your ID already exists!";
            }
            else cout << "Done!";
            getline(cin, input);
          }
          else if (what2 == "company") {
            string tmp_n, tmp_id, tmp_finloc, tmp_oploc, tmp_p, d1, d2;
            Date tmp_sub, tmp_inc;
            cout << "Name:\n>";
            cin >> tmp_n;
            cout << "Id:\n>";
            cin >> tmp_id;
            cout << "Finantial location:\n>";
            cin.ignore();
            getline(cin, tmp_finloc);
            cout << "Operative location:\n>";
            cin.ignore();
            getline(cin, tmp_oploc);
            cout << "Type of product:\n>";
            cin.ignore();
            getline(cin, tmp_p);
            cout << "Subscription:\n>";
            cin >> d1;
            tmp_sub.scanDateByStr(d1);
            cout << "Inception:\n>";
            cin >> d2;
            tmp_inc.scanDateByStr(d2);
            
            Company new_c(tmp_n, tmp_id, tmp_finloc, tmp_oploc, tmp_p, tmp_sub, tmp_inc);
            if (!manager.addAccount(new_c)) {
              cout << "Error! Your ID already exists!";
            }
            else
              cout << "Done!";
          }
        }
        else if (what1 == "relationship") {
          int error = 0;
          while (error != 1) {
            cout << "Please insert: <id_subject> <type_of_relation> <id_target>\n"
                    "Your available relationships are:\n"
                    "Friendship\n"
                    "Knowings\n"
                    "Parent\n"
                    "Born\n"
                    "Partner\n"
                    "Membership\n"
                    "Partnership\n"
                    "Employee\n"
                    "Employer\n" << endl;
            
            string who1, who2, type_rel;
            getline(cin, input);
            
            command.clear();
            command.str(input);
            command >> who1 >> type_rel >> who2;
            
            if (who1.empty() || who2.empty() || type_rel.empty()) {
              cout << "Error! Your data were not inserted properly, I got some of them empty. Please retry!" << endl;
            }
            error = manager.addRelationship(who1, who2, type_rel);
            //Analizzo il codice errore restituito da addRelationship.
            if (error == -1) {
              cout << "Error! The first ID does not exist!" << endl;
            }
            else if (error == -2) {
              cout << "Error! The second ID does not exist!" << endl;
            }
            else if (error == -3) {
              cout << "Error! This relationship does not exist!" << endl;
            }
            else if (error == 1) {
              cout << "Done!";
            }
          }
        }
        else if (what1 == "post") {
          string news, d_t, whose_ID;
          set<string> likes, dislikes;
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
          
          cout << "Dislikes (insert '-' at the end of the list):\n";
          input.clear();
          while (input != "-") {
            cout << ">";
            getline(cin, input);
            if (input != "-")
              dislikes.insert(input);
          }
          post_tmp.setDislikes(dislikes);
          
          cout << news << " " << d_t << endl;
          
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
          }
          cout << "Who wrote the post?:\n>";
          getline(cin, whose_ID);
          if (!manager.addPost(post_tmp, whose_ID))
            cout << "Something went wrong.." << endl;
          
        }
        else if (what1 == "like") {
          string who, tmp_news, d_t;
          pair<string, vector<Post>> post;
          
          cout << "Please insert the news of the target post:\n>";
          cin.ignore();
          getline(cin, tmp_news);
          cout << "Please insert the date and the time (in format dd/mm/yyyy hh:mm) of the target post:\n>";
          cin.ignore();
          getline(cin, d_t);
          cout << "Please insert the id whose user would react to this post:\n>";
          cin >> who;
          
          Post cmp_post(tmp_news, d_t);
          
          if (manager.setReaction(1, 1, cmp_post, who)) {
            cout << "Done!" << endl;
          }
          else {
            cout << "Error!" << endl; //1-NO ID, 2-NO AUTOLIKES, 3-NO POST
          }
        }
        else if (what2 == "dislike") {
          string who, tmp_news, d_t;
          pair<string, vector<Post>> post;
          
          cout << "Please insert the news of the target post:\n>";
          cin.ignore();
          getline(cin, tmp_news);
          cout << "Please insert the date and the time (in format dd/mm/yyyy hh:mm) of the target post:\n>";
          cin.ignore();
          getline(cin, d_t);
          cout << "Please insert the id whose user would react to this post:\n>";
          cin >> who;
          
          Post cmp_post(tmp_news, d_t);
          
          if (manager.setReaction(0, 1, cmp_post, who)) {
            cout << "Done!" << endl;
          }
          else {
            cout << "Error! I could not set this reaction." << endl; //1-NO ID, 2-NO AUTOLIKES, 3-NO POST
          }
          
        }
        
        
      }
      
    }
    else if (cmd == "stats") {
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
            cout << "Error! " << id << "is not a User or it does not exist." << endl;
          }
          else {
            cout << "The number of " << what2 << "is: " << num << endl;
          }
        }
        else if (what2 == "relatives") {
          cout << "Please insert the ID:\n>";
          cin >> id;
          num = manager.NumRelatives(id);
          if (num == 0) {
            cout << "Error! " << id << "is not a User or it does not exist." << endl;
          }
          else {
            cout << "The number of " << what2 << "is: " << num << endl;
          }
        }
        else if (what2 == "employees") {
          cout << "Please insert the ID of the Company employer:\n>";
          cin >> id;
          num = manager.NumEmployees(id);
          if (num == 0) {
            cout << "Error! " << id << "is not a Company or it does not exist." << endl;
          }
          else {
            cout << "The number of " << what2 << "is: " << num << endl;
          }
        }
        else if (what2 == "subsidiaries") {
          cout << "Please insert the target ID:\n>";
          cin >> id;
          num = manager.NumSubsidiaries(id);
          if (num == 0) {
            cout << "Error! " << id << "is not a Company or it does not exist." << endl;
          }
          else {
            cout << "The number of " << what2 << "is: " << num << endl;
          }
        }
        else if (what2 == "members") {
          cout << "Please insert the target ID:\n>";
          cin >> id;
          num = manager.NumMembers(id);
          if (num == 0) {
            cout << "Error! " << id << "is not a Group or it does not exist." << endl;
          }
          else {
            cout << "The number of " << what2 << "is: " << num << endl;
          }
        }
        else if (what2 == "born_after") {
          string date;
          Date born_d;
          int not_valid;
          cout << "Please insert the starting date (in format dd/mm/yyyy):\n>";
          cin >> date;
          if (born_d.CheckDate(date)) {
            born_d.scanDateByStr(date);
            num = manager.NumBornAfter(born_d);
            cout << "The number of people born after your date is: " << num << endl;
          }
          else {
            cout << "Error! This date is not valid." << endl;
            num = 0;
          }
        }
        else {
          cout << "Error! I do not understand what statistic you'd like to retreive." << endl;
          num = 0;
        }
        
      }
      else if (what1 == "most") {
        if (what2 == "employing_company") {
          Company empl_comp;
          empl_comp = manager.MostEmployingCompany();
          cout << "Name: " << empl_comp.getName() << "\n"
               << "Financial location: " << empl_comp.getFinancialLocation() << "\n"
               << "Operative location: " << empl_comp.getOperativeLocation() << "\n"
               << "Product: " << empl_comp.getTypeOfProduct() << "\n"
               << "Inception: " << empl_comp.getInception() << "\n"
               << "Birth Date: " << empl_comp.getSubscription() << std::endl;
          
        }
        else if (what2 == "employing_partnership") {
          vector<string> part;
          part = manager.MostEmployingPartnership();
          cout << "The members of the Most Employing Partnership are:\n";
          for (auto it = part.begin(); it != part.end(); it++) {
            cout << *it << endl;
          }
        }
        else if (what2 == "user_friends") {
          User mostfr_user;
          mostfr_user = manager.UserWithMostFriends();
          cout << "Name:" << mostfr_user.getName() << "\n"
               << "Surname: " << mostfr_user.getSurname() << "\n"
               << "Gender: " << mostfr_user.getGender() << "\n"
               << "Address: " << mostfr_user.getAddress() << "\n"
               << "Birth Date :" << mostfr_user.getBirth() << "\n"
               << "Subscription Date: " << mostfr_user.getSubscription() << std::endl;
          
        }
        else if (what2 == "user_acquaintances") {
          User mostacq_user;
          mostacq_user = manager.UserWithMostAcquaintances();
          cout << "Name:" << mostacq_user.getName() << "\n"
               << "Surname: " << mostacq_user.getSurname() << "\n"
               << "Gender: " << mostacq_user.getGender() << "\n"
               << "Address: " << mostacq_user.getAddress() << "\n"
               << "Birth Date :" << mostacq_user.getBirth() << "\n"
               << "Subscription Date: " << mostacq_user.getSubscription() << std::endl;
          
        }
        else if (what2 == "liked_post") {
          pair<string, Post> l_post;
          l_post = manager.MostLikedPost();
          cout << "The Most Liked Post is:\n" << l_post.second << endl
               << "Wrote by: " << l_post.first;
          
        }
        else if (what2 == "disliked_post") {
          pair<string, Post> d_post;
          d_post = manager.MostDislikedPost();
          cout << "The Most Disliked Post is:\n" << d_post.second << endl
               << "Wrote by: " << d_post.first;
          
        }
        else if (what2 == "liked_account") {
          cout << "The Most Liked Account is:\n" << manager.MostLiked_DislikedAccount(1);
          
        }
        else if (what2 == "disliked_account") {
          cout << "The Most Disliked Account is:\n" << manager.MostLiked_DislikedAccount(0);
          
        }
        else {
          cout << "Error! I do not understand what statistic you'd like to retreive." << endl;
        }
        
      }
      else if (what1 == "average_age") {
        cout << "The Average of Users' ages is:\n"
             << manager.UsersAverageAge() << endl;
        
      }
      else if (what1 == "best_post") {
        pair<string, Post> best_post;
        best_post = manager.RatioReactionPost(1);
        cout << "The Post with the best Like/Dislike ratio is:\n" << best_post.second << endl
             << "Wrote by: " << best_post.first;
      }
      else if (what1 == "worse_post") {
        pair<string, Post> worse_post;
        worse_post = manager.RatioReactionPost(0);
        cout << "The Post with the worse Like/Dislike ratio is:\n" << worse_post.second << endl
             << "Wrote by: " << worse_post.first;
      }
      
    }
    else if (cmd == "exit") {
      exit = true;
    }
    else {
      std::cout << "Command " << cmd << " unknown." << std::endl;
    }
    command.clear();    //Libera lo stream del comando
    cmd.clear();        //Svuota la stringa per sicurezza
  } while (!exit);
  return 0;
}
