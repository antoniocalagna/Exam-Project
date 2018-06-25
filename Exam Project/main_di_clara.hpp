#include "Shell.h"
#include "FileHandler.h"
#include <vector>

#define BUFF_TOTAL_SIZE_MAX 5
#define CYCLES_WITHOUT_SAVING_MAX 10

using namespace std;

void flushBuffers(FH::FileHandler &fh, IOBuffer &new_data, IOBuffer &data_to_delete);

int main_di_clara(/*int argc, char *argv[]*/) {
  Manager manager;
  FH::FileHandler accounts_fh(
          "Accounts_TEST.dat"),        //FileHandler per il controllo e l'acquisizione dei dati da file
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
    std::cout << "* Accounts file correctly formatted and ready to be read. "
                 "(" << check_results.data << " lines analyzed)" << std::endl;
  } else {
    std::cerr << "** Accounts file returned error code " << check_results.code << " at line " << check_results.data
              << "."
              << "File needs to be corrected before data can be read." << std::endl;
    return -1;
  }

  std::cout << "Checking relations file..." << std::endl;
  //Controllo del file delle relazioni
  check_results = relations_fh.checkFile(FH::relationsFile);
  if (check_results.code == 0) {
    std::cout << "* Relations file correctly formatted and ready to be read. "
                 "(" << check_results.data << " lines analyzed)" << std::endl;
  } else {
    std::cerr << "** Relations file returned error code " << check_results.code << " at line " << check_results.data
              << "."
              << "File needs to be corrected before data can be read." << std::endl;
    return -1;
  }

  //Controllo del file dei post
  std::cout << "Checking posts file..." << std::endl;
  check_results = posts_fh.checkFile(FH::postsFile);
  if (check_results.code == 0) {
    std::cout << "* Posts file correctly formatted and ready to be read. "
                 "(" << check_results.data << " lines analyzed)" << std::endl;
  } else {
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

    } else if (err == -2) {
      std::cerr << "Accounts file returned error code, the id " << id_target << " does not exist.\n"
                << "File needs to be corrected before data can be acquired." << std::endl;
      return -3;

    } else if (err == -3) {
      std::cerr << "Accounts file returned error code, the relationship " << relationship << " does not exist.\n"
                << "File needs to be corrected before data can be acquired." << std::endl;
      return -4; //ritorno -4 quando la relazione non esiste o non può esistere

    } else if (err == -4) {
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

  std::map<std::string, Shell::Function> commands{      //Mappa che lega il nome del comando alla sua funzione
          std::make_pair("help", Shell::help),          //Inizzializiamo le coppie come pair nome - pointer a funzione
          std::make_pair("list", Shell::list),
          std::make_pair("get", Shell::get),
          std::make_pair("set", Shell::set),
          std::make_pair("delete", Shell::del),
          std::make_pair("stats", Shell::stats),
  };

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
    } else if (cmd == "add") {
      string what1, what2;
      command >> what1 >> what2;
      if (what1.empty()) {
        cout << "Error! I do not understand what you would like to add. Please retry!" << endl;
      } else {

        if (what1 == "account") {
          if (what2.empty()) {
            cout << "Error! I do not understand what you would like to add. Please retry!" << endl;
          } else if (what2 == "user") {
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
            } else cout << "Done!";
          } else if (what2 == "group") {
            string tmp_n, tmp_id, tmp_loc, tmp_act, d1, d2;
            Date tmp_sub, tmp_inc;
            cout << "Name:\n>";
            cin >> tmp_n;
            cout << "Id:\n>";
            cin >> tmp_id;
            cout << "Legal location:\n>";
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
            } else cout << "Done!";
          } else if (what2 == "company") {
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
            } else
              cout << "Done!";
          }
        } else if (what1 == "relationship") {
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

            command >> who1 >> type_rel >> who2;

            if (who1.empty() || who2.empty() || type_rel.empty()) {
              cout << "Error! Your data were not inserted properly, I got some of them empty. Please retry!" << endl;
            }
            error = manager.addRelationship(who1, who2, type_rel);
            //Analizzo il codice errore restituito da addRelationship.
            if (error == -1) {
              cout << "Error! The first ID does not exist!" << endl;
            } else if (error == -2) {
              cout << "Error! The second ID does not exist!" << endl;
            } else if (error == -3) {
              cout << "Error! This relationship does not exist!" << endl;
            } else if (error == -4) {
              cout << "Error! The relationship and the Users' ages are not compatible!" << endl;
            } else if (error == 1) {
              cout << "Done!";
            }
          }
        } else if (what1 == "post") {
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

        } else if (what1 == "like") {
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
          } else {
            cout << "Error!" << endl; //1-NO ID, 2-NO AUTOLIKES, 3-NO POST
          }
        } else if (what2 == "dislike") {
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
          } else {
            cout << "Error! I could not set this reaction." << endl; //1-NO ID, 2-NO AUTOLIKES, 3-NO POST
          }

        }


      }

    } else if (cmd == "search") {
      string what_to_search;
      command >> what_to_search;
      if (what_to_search == "genealogical_trees") {
        /*vector <string> trees_to_print = manager.PrintAllTrees();
        for (auto it = trees_to_print.begin(); it!=trees_to_print.end(); it++){
          new_data_buffer << *it;
        }*/
        // devo poterli stampare su files diversi

      } else if (what_to_search == "genealogical_tree") {
        string id;
        cout << "Please insert the id of the User you want to build the genealogical tree:\n";
        cin >> id;
        cout << manager.PrintTree(id); //dentro la funzione non c'è il controllo dell'esistenza dell'id
        //per ora ho messo cout anche se vanno stampati su file

      } else if (what_to_search == "loner_people") {
        unsigned int relations, memberships, reactions;
        string ans; //answer
        bool unemployed;
        vector<string> lon_people;

        cout << "Please insert the parameters that define a loner person:\n"
                "Minimum number of relation: ";
        cin >> relations;
        cout << "Minimum number of groups: ";
        cin >> memberships;
        cout << "Does the user need to be unemployed? (yes/no): "; //non ho capito se logicamente sia giusto per come è fatta la funzione
        cin >> ans;
        if (ans == "yes") {
          unemployed = true;
        } else {
          unemployed = false;
        }
        cout << "Minimum number of added reactions: ";
        cin >> reactions;

        lon_people = manager.LonerPeople(relations, memberships, unemployed, reactions);
        for (auto it = lon_people.begin(); it != lon_people.end(); it++) {
          cout << *it;
        }

      } else if (what_to_search == "friendliest_company") {
        //function
      } else {
        cout << "Error! I do not understand what type of enquiry you'd like to obtain." << endl;
      }

    } else if (cmd == "save") {
      save_data = true;
    } else if (cmd == "exit") {
      save_data = true;             //Richiedi di salvare i dati
      exit = true;                  //Richiedi l'uscita
    } else {
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
    } else {
      cycles_without_saving++;
    }
    command.str(std::string());   //Svuota il buffer del comando
  } while (!exit);
  return 0;
}
