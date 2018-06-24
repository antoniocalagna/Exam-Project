#include "Shell.h"
#include "FileHandler.h"

#define BUFF_TOTAL_SIZE_MAX 5
#define CYCLES_WITHOUT_SAVING_MAX 10

using namespace std;

void flushBuffers(FH::FileHandler &fh, IOBuffer &new_data, IOBuffer &data_to_delete);

int main_di_clara(/*int argc, char *argv[]*/) {
  Manager manager;
  FH::FileHandler accounts_fh("Accounts_TEST.dat"),        //FileHandler per il controllo e l'acquisizione dei dati da file
          relations_fh("Relations_TEST.dat"),
          posts_fh("Posts_TEST.dat");
  IOBuffer new_data_buffer;                                 //Buffer necessario per l'acquisizione dei dati
  IOBuffer data_to_erase_buffer;                            //Buffer per i dati da cancellare
  
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
  
  ///////////////////////////////////              CONTROLLI             //////////////////////////////////////////////
  std::cout << "\e[1m" << "Beginning checks:" << "\e[0m" << std::endl;
  //Controllo del file degli account
  std::cout << "Checking accounts file..." << std::endl;
  FH::Error check_results = accounts_fh.checkFile(FH::accountsFile);
  if (check_results.code == 0) {
    std::cout << "* Accounts file correctly formatted and ready to be read." << std::endl;
  }
  else {
    std::cerr << "** Accounts file returned error code " << check_results.code << " at line " << check_results.data
              << "."
              << "File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  std::cout << "Checking relations file..." << std::endl;
  //Controllo del file delle relazioni
  check_results = relations_fh.checkFile(FH::relationsFile);
  if (check_results.code == 0) {
    std::cout << "* Relations file correctly formatted and ready to be read." << std::endl;
  }
  else {
    std::cerr << "** Relations file returned error code " << check_results.code << " at line " << check_results.data
              << "."
              << "File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  //Controllo del file dei post
  std::cout << "Checking posts file..." << std::endl;
  check_results = posts_fh.checkFile(FH::postsFile);
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
  
  ///////////////////////////////////            ACQUISIZIONE             //////////////////////////////////////////////
  
  std::cout << "Beginning acquisition: " << std::endl;
  
  std::cout << "Acquiring accounts..." << std::endl;
  accounts_fh.fetchData(FH::accountsFile, new_data_buffer);
  while (!new_data_buffer.usersEmpty()) {
    User user_tmp;
    new_data_buffer >> user_tmp;
    if (!manager.addAccount(user_tmp)) {
      std::cerr << "Accounts file returned error code: " << user_tmp.getID() << "is not unique.\n"
                << "File needs to be corrected before data can be acquired." << std::endl;
      return -2;; //ritorno -2 quando l'id non è univoco
    }
  }
  while (!new_data_buffer.groupsEmpty()) {
    Group group_tmp;
    new_data_buffer >> group_tmp;
    if (!manager.addAccount(group_tmp)) {
      std::cerr << "Accounts file returned error code: " << group_tmp.getID() << "is not unique.\n"
                << "File needs to be corrected before data can be acquired." << std::endl;
      return -2;
    }
  }
  while (!new_data_buffer.companiesEmpty()) {
    Company comp_tmp;
    new_data_buffer >> comp_tmp;
    if (!manager.addAccount(comp_tmp)) {
      std::cerr << "Accounts file returned error code, the id: " << comp_tmp.getID() << "is not unique.\n"
                << "File needs to be corrected before data can be acquired." << std::endl;
      return -2;
    }
  }
  std::cout << "* Accounts correctly acquired." << std::endl;
  
  std::cout << "Acquiring relations..." << std::endl;
  relations_fh.fetchData(FH::relationsFile, new_data_buffer);
  while (!new_data_buffer.relationsEmpty()) {
    IOBuffer::Relation relation_tmp;
    new_data_buffer >> relation_tmp;
    string id_start, id_target, relationship;
    id_start = relation_tmp.first.first;
    id_target = relation_tmp.first.second;
    relationship = relation_tmp.second;
    int err = manager.addRelationship(id_start, id_target, relationship);
    if (err == -1) {
      std::cerr << "Accounts file returned error code, the id " << id_start << " does not exist.\n"
                << "File needs to be corrected before data can be acquiredd." << std::endl;
      return -3; //ritorno -3 quando l'id non esiste
      
    }
    else if (err == -2) {
      std::cerr << "Accounts file returned error code, the id " << id_target << " does not exist.\n"
                << "File needs to be corrected before data can be acquired." << std::endl;
      return -3;
      
    }
    else if (err == -3) {
      std::cerr << "Accounts file returned error code, the relationship " << relationship << " does not exist.\n"
                << "File needs to be corrected before data can be acquired." << std::endl;
      return -4; //ritorno -4 quando la relazione non esiste o non può esistere
      
    }
    else if (err == -4) {
      std::cerr << "Accounts file returned error code, the relationship " << relationship
                << " and the Users' ages are not compatible.\n"
                << "File needs to be corrected before data can be acquired." << std::endl;
      return -4;
      
    }
  }
  std::cout << "* Relations correctly acquired." << std::endl;
  
  std::cout << "Acquiring posts..." << std::endl;
  posts_fh.fetchData(FH::postsFile, new_data_buffer);
  while (!new_data_buffer.postsEmpty()) {
    IOBuffer::m_Post post_tmp;
    new_data_buffer >> post_tmp;
    if (!manager.addPost(post_tmp.second, post_tmp.first)) {
      std::cerr << "Accounts file returned error code, the id " << post_tmp.first << " does not exist.\n"
                << "File needs to be corrected before data can be acquired." << std::endl;
      return -3;
    }
  }
  std::cout << "* Posts correctly acquired." << std::endl;
  std::cout << "Acquisition completed.\n" << std::endl;
  
  ///////////////////////////////////          INIZIO  PROGRAMMA          //////////////////////////////////////////////
  std::cout << "Welcome. This program is shell based, so commands should be typed in shell-style.\n"
               "Type \"help\" for a list of useful commands." << std::endl;
  
  std::map<std::string, Shell::Function> commands;      //Mappa di tutti i comandi e delle loro relative funzioni
  
  commands["help"] = Shell::help;
  commands["list"] = Shell::list;
  commands["get"] = Shell::get;
  commands["set"] = Shell::set;
  commands["delete"] = Shell::del;
  
  bool exit = false;
  bool save_data = false;
  unsigned int cycles_without_saving = 0;
  do {
    string input;             //Input dell'utente
    string cmd;               //Nome del comando (primo parametro dell'input)
    
    std::cout << std::endl;
    do {                      //Acquisisci ignorando le righe vuote
      cout << ">";
      getline(cin, input);
    } while (input.empty());
    
    stringstream command(input);
    command >> cmd;
    
    if (commands.count(cmd) == 1) {
      //Il comando è stato trovato. Esegui la funzione ad esso associata
      commands.at(cmd)(command, manager, new_data_buffer, data_to_erase_buffer);
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
            
            command.clear();
            command.str(input);
            command >> cmd;
            
          }
          else if (what2 == "group") {
            string tmp_n, tmp_id, tmp_loc, tmp_act, d1, d2;
            Date tmp_sub, tmp_inc;
            cout << "Name:\n>";
            cin >> tmp_n;
            cout << "Id:\n>";
            cin >> tmp_id;
            cout << "Legal location:\n>";
            cin.ignore();                     //Ignora l'andata a capo
            getline(cin, tmp_loc);
            cout << "Type of activity:\n>";
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
            
            command.clear();
            command.str(input);
            command >> cmd;
            
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
            
            getline(cin, input);
            
            command.clear();
            command.str(input);
            command >> cmd;
            
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
            else if (error == -4) {
              cout << "Error! The relationship and the Users' ages are not compatible!" << endl;
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
    else if (cmd == "save") {
      save_data = true;
    }
    else if (cmd == "exit") {
      save_data = true;             //Richiedi di salvare i dati
      exit = true;                  //Richiedi l'uscita
    }
    
    else {
      std::cout << "Command \"" << cmd << "\" unknown." << std::endl;
    }
    
    /**************************************    Salvataggio dati     ***************************************************/
    if (new_data_buffer.size() + data_to_erase_buffer.size() >= BUFF_TOTAL_SIZE_MAX ||
        cycles_without_saving >= CYCLES_WITHOUT_SAVING_MAX) {
      save_data = true;
    }
    
    if (save_data) {
      std::cout << "Saving data..." << std::endl;
      accounts_fh.putData(FH::accountsFile, new_data_buffer, data_to_erase_buffer);
      relations_fh.putData(FH::relationsFile, new_data_buffer, data_to_erase_buffer);
      posts_fh.putData(FH::postsFile, new_data_buffer, data_to_erase_buffer);
      std::cout << "Data saved." << std::endl;
      cycles_without_saving = 0;
    }
    else {
      cycles_without_saving++;
    }
    
    std::cin.clear();   //Pulizia degli stream
    command.clear();
  } while (!exit);
  return 0;
}