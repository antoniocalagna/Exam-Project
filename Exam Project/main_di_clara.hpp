#include "Post.h"
#include "User.h"
#include "Account.h"
#include "Manager.h"
#include "FileHandler.h"

using namespace std;

int main_di_clara(/*int argc, char *argv[]*/) {
  FH::FileHandler accounts_fh,          //FileHandler per il controllo e l'acquisizione dei dati da file
          relations_fh,
          posts_fh;
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

  accounts_fh.fetchData(FH::accountsFile, buffer);
  relations_fh.fetchData(FH::relationsFile, buffer);
  posts_fh.fetchData(FH::postsFile, buffer);
  Manager manager;
  User user_tmp;
  Company comp_tmp;
  Group group_tmp;
  IOBuffer::m_Post post_tmp;
  IOBuffer::Relation relation_tmp;
  while (!buffer.usersEmpty()) {
    buffer >> user_tmp;
    if (!manager.addAccount(user_tmp)) {
      std::cerr << "Accounts file returned error code: " << user_tmp.getID() << "is not unique.\n"
                << "File needs to be corrected before data can be acquire." << std::endl;
      return -2;; //ritorno -2 quando l'id non è univoco
    }
  }
  while (!buffer.groupsEmpty()) {
    buffer >> group_tmp;
    if (!manager.addAccount(group_tmp)) {
      std::cerr << "Accounts file returned error code: " << group_tmp.getID() << "is not unique.\n"
                << "File needs to be corrected before data can be acquire." << std::endl;
      return -2;
    }
  }
  while (!buffer.companiesEmpty()) {
    buffer >> comp_tmp;
    if (!manager.addAccount(comp_tmp)) {
      std::cerr << "Accounts file returned error code, the id: " << comp_tmp.getID() << "is not unique.\n"
                << "File needs to be corrected before data can be acquire." << std::endl;
      return -2;
    }
  }
  while (!buffer.relationsEmpty()) {
    buffer >> relation_tmp;
    string id_start, id_target, relationship;
    id_start = relation_tmp.first.first;
    id_target = relation_tmp.first.second;
    relationship = relation_tmp.second;
    int err = manager.addRelationship(id_start, id_target, relationship);
    if (err == -1) {
      std::cerr << "Accounts file returned error code, the id " << id_start << " does not exist.\n"
                << "File needs to be corrected before data can be acquire." << std::endl;
      return -3; //ritorno -3 quando l'id non esiste

    } else if (err == -2) {
      std::cerr << "Accounts file returned error code, the id " << id_target << " does not exist.\n"
                << "File needs to be corrected before data can be acquire." << std::endl;
      return -3;

    } else if (err == -3) {
      std::cerr << "Accounts file returned error code, the relationship " << relationship << " does not exist.\n"
                << "File needs to be corrected before data can be acquire." << std::endl;
      return -4; //ritorno -4 quando la relazione non esiste o non può esistere

    } else if (err == -4) {
      std::cerr << "Accounts file returned error code, the relationship " << relationship
                << " and the Users' ages are not compatible.\n"
                << "File needs to be corrected before data can be acquire." << std::endl;
      return -4;

    }
  }

  while (!buffer.postsEmpty()) {
    buffer >> post_tmp;
    if (!manager.addPost(post_tmp.second, post_tmp.first)) {
      std::cerr << "Accounts file returned error code, the id " << post_tmp.first << " does not exist.\n"
                << "File needs to be corrected before data can be acquire." << std::endl;
      return -3;
    }

  }


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
      cout << ">";

      getline(cin, input);
      
      command.clear();
      command.str(input);
      command >> cmd;
    } else if (cmd == "set") {
      string what, who; //non so se questi nomi sono messi per scherzo, a me sembrano carini
      command >> who >> what;
      if (what.empty()) {
        cout << "Error! I do not understand what information you'd like to set." << endl;
      } else {
        if (who == "user") {
          string id_to_set;
          User user_to_set; //user a cui vuoi cambiare info
          cout << "Please insert the ID whose User you'd like to set info about:\n>" << endl;
          cin >> id_to_set; //id "vecchio" account
          user_to_set = manager.getUser(id_to_set); //se l'id non esiste, user_to_set è un default constructor
          if (user_to_set == User()) {
            cout << "Error! This ID is not valid." << endl;
          } else {
            if (what == "name") {
              string new_name;
              cout << "Please insert the new" << what << "." << endl;
              //cin.ignore(); da provare se ci vada o no
              getline(cin, new_name);
              user_to_set.setName(new_name);
              if (!manager.replaceAccount(id_to_set, user_to_set)) { //da provare
                cout << "Error! I could not modify your information." << endl;
              } else {
                cout << "Done!" << endl;
              }
            } else if (what == "surname") {
              string new_surname;
              cout << "Please insert the new" << what << "." << endl;
              //cin.ignore();
              getline(cin, new_surname);
              user_to_set.setSurname(new_surname);
              if (!manager.replaceAccount(id_to_set, user_to_set)) {
                cout << "Error! I could not modify your information." << endl;
              } else {
                cout << "Done!" << endl;
              }

            } else if (what == "gender") {
              char new_gender;
              cout << "Please insert the new gender." << endl;
              //cin.ignore();
              cin >> new_gender;
              user_to_set.setGender(new_gender);
              if (!manager.replaceAccount(id_to_set, user_to_set)) {
                cout << "Error! I could not modify your information." << endl;
              } else {
                cout << "Done!" << endl;
              }

            } else if (what == "birth") {
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
                } else {
                  cout << "Done!" << endl;
                }
              } else {
                cout << "Error! Date is not valid." << endl;
              }

            } else if (what == "address") {
              string new_addr;
              cout << "Please insert the new address." << endl;
              //cin.ignore();
              getline(cin, new_addr);
              user_to_set.setAddress(new_addr);
              if (!manager.replaceAccount(id_to_set, user_to_set)) {
                cout << "Error! I could not modify your information." << endl;
              } else {
                cout << "Done!" << endl;
              }

            } else if (what == "id") {
              string new_id;
              cout << "Please insert the new ID." << endl;
              //cin.ignore();
              getline(cin, new_id);
              user_to_set.setID(new_id);
              if (manager.deleteAccount(
                      new_id)) { //questo l'ho fatto "a mano" perchè non penso che con la funzione si possa fare
                if (!manager.addAccount(user_to_set)) {
                  cout << "Error! I could not modify your information." << endl;
                } else {
                  cout << "Done!" << endl;
                }
              } else {
                cout << "Error! The ID does not exist." << endl;
              }

            } else if (what == "subscription") {
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
                } else {
                  cout << "Done!" << endl;
                }

              } else {
                cout << "Error! This date is not valid." << endl;
              }
            }
          }
        } else if (who == "group") {
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
            } else {
              cout << "Done!" << endl;
            }

          } else if (what == "id") {
            string new_id;
            cout << "Please insert the new ID." << endl;
            //cin.ignore();
            getline(cin, new_id);
            group_to_set.setID(new_id);
            if (manager.deleteAccount(
                    new_id)) { //questo l'ho fatto "a mano" perchè non penso che con la funzione si possa fare
              if (!manager.addAccount(group_to_set)) {
                cout << "Error! This ID already exists." << endl;
              } else {
                cout << "Done!" << endl;
              }
            } else {
              cout << "Error! This ID does not exist." << endl;
            }

          } else if (what == "subscription") {
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
              } else {
                cout << "Done!" << endl;
              }
            } else {
              cout << "Error! This date is not valid." << endl;
            }

          } else if (what == "location") {
            string new_loc;
            cout << "Please insert the new location." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_loc);
            group_to_set.setLegalLocation(new_loc);
            if (!manager.replaceAccount(id_to_set, group_to_set)) { //da provare
              cout << "Error! I could not modify your information." << endl;
            } else {
              cout << "Done!" << endl;
            }

          } else if (what == "type_activity") {
            string new_act;
            cout << "Please insert the new type of activity." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_act);
            group_to_set.setTypeOfActivity(new_act);
            if (!manager.replaceAccount(id_to_set, group_to_set)) { //da provare
              cout << "Error! I could not modify your information." << endl;
            } else {
              cout << "Done!" << endl;
            }

          } else if (what == "inception") {
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
              } else {
                cout << "Done!" << endl;
              }
            } else {
              cout << "Error! The date is not valid." << endl;
            }

          }
        } else if (who == "company") {
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
            } else {
              cout << "Done!" << endl;
            }

          } else if (what == "id") {
            string new_id;
            cout << "Please insert the new ID." << endl;
            //cin.ignore();
            getline(cin, new_id);
            company_to_set.setID(new_id);
            if (manager.deleteAccount(new_id)) {
              if (!manager.addAccount(company_to_set)) {
                cout << "Error! This ID already exists." << endl;
              } else {
                cout << "Done!" << endl;
              }

            } else if (what == "subscription") {
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
                } else {
                  cout << "Done!" << endl;
                }
              } else {
                cout << "Error! This date is not valid." << endl;
              }
            }

          } else if (what == "inception") {
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
              } else {
                cout << "Done!" << endl;
              }
            } else {
              cout << "Error! This date is not valid." << endl;
            }

          } else if (what == "financial_location") {
            string new_loc;
            cout << "Please insert the new financial location." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_loc);
            company_to_set.setFinancialLocation(new_loc);
            if (!manager.replaceAccount(id_to_set, company_to_set)) {
              cout << "Error! I could not modify your information." << endl;
            } else {
              cout << "Done!" << endl;
            }

          } else if (what == "operative_location") {
            string new_loc;
            cout << "Please insert the new operative location." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_loc);
            company_to_set.setOperativeLocation(new_loc);
            if (!manager.replaceAccount(id_to_set, company_to_set)) {
              cout << "Error! I could not modify your information." << endl;
            } else {
              cout << "Done!" << endl;
            }

          } else if (what == "products") {
            string new_prod;
            cout << "Please insert the new type of products." << endl;
            //cin.ignore(); da provare se ci vada o no
            getline(cin, new_prod);
            company_to_set.setTypeOfProduct(new_prod);
            if (!manager.replaceAccount(id_to_set, company_to_set)) {
              cout << "Error! I could not modify your information." << endl;
            } else {
              cout << "Done!" << endl;
            }

          }
        }
      }
    } else if (cmd == "get") {
      string who, what;
      command >> who >> what;
      if (what.empty()) {
        cout << "Error! I do not understand what information you'd like to retreive." << endl;
      }
      if (who == "user") {
        if (what == "info") {
          string id;
          User u;
          cout << "Please insert the ID:\n>";
          cin >> id;
          u = manager.getUser(id);
          cout << u.getName() << "\n" << u.getSurname() << "\n" << u.getID() << "\n" << u.getAddress() << "\n"
               << u.getSubscription() << "\n" << u.getBirth() << "\n" << u.getGender() << endl;
        }
      } else if (who == "group") {
        if (what == "info") {
          string id;
          Group g;
          cout << "Please insert the ID:\n>";
          cin >> id;
          g = manager.getGroup(id);
          cout << g.getName() << "\n" << g.getID() << "\n" << g.getLegalLocation() << "\n" << g.getTypeOfActivity()
               << "\n" << g.getSubscription() << "\n" << g.getInception() << endl;
        }
      } else if (who == "company") {
        if (what == "info") {
          string id;
          Company c;
          cout << "Please insert the ID:\n>";
          cin >> id;
          c = manager.getCompany(id);
          cout << c.getName() << "\n" << c.getID() << "\n" << c.getFinancialLocation() << "\n"
               << c.getOperativeLocation() << "\n" << c.getTypeOfProduct() << "\n" << c.getSubscription() << "\n"
               << c.getInception() << endl;
        }
      }

    } else if (cmd == "delete") {
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
        } else {
          manager.deleteAccount(who);
        }

      } else if (what == "relationship") {
        string id_start, id_target;
        cout << "Please insert: <id_subject> <id_target>:\n";
        cin >> id_start >> id_target;
        if (id_start.empty() || id_target.empty()) {
          cout << "Error! I do not understand which IDs I have to work with." << endl;
        }
        if (!manager.deleteRelationship(id_start, id_target)) {
          cout << "Error! One or both of your IDs don't exist." << endl;
        }
      } else if (what == "post") {
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
      } else if (what == "like") {
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

      } else if (what == "dislike") {
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

            getline(cin, input);

            command.clear();
            command.str(input);
            command >> cmd;

          } else if (what2 == "group") {
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
            } else cout << "Done!";
            getline(cin, input);

            command.clear();
            command.str(input);
            command >> cmd;

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

            getline(cin, input);

            command.clear();
            command.str(input);
            command >> cmd;

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

    } else if (cmd == "stats") {
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
          } else {
            cout << "The number of " << what2 << "is: " << num << endl;
          }
        } else if (what2 == "relatives") {
          cout << "Please insert the ID:\n>";
          cin >> id;
          num = manager.NumRelatives(id);
          if (num == 0) {
            cout << "Error! " << id << "is not a User or it does not exist." << endl;
          } else {
            cout << "The number of " << what2 << "is: " << num << endl;
          }
        } else if (what2 == "employees") {
          cout << "Please insert the ID of the Company employer:\n>";
          cin >> id;
          num = manager.NumEmployees(id);
          if (num == 0) {
            cout << "Error! " << id << "is not a Company or it does not exist." << endl;
          } else {
            cout << "The number of " << what2 << "is: " << num << endl;
          }
        } else if (what2 == "subsidiaries") {
          cout << "Please insert the target ID:\n>";
          cin >> id;
          num = manager.NumSubsidiaries(id);
          if (num == 0) {
            cout << "Error! " << id << "is not a Company or it does not exist." << endl;
          } else {
            cout << "The number of " << what2 << "is: " << num << endl;
          }
        } else if (what2 == "members") {
          cout << "Please insert the target ID:\n>";
          cin >> id;
          num = manager.NumMembers(id);
          if (num == 0) {
            cout << "Error! " << id << "is not a Group or it does not exist." << endl;
          } else {
            cout << "The number of " << what2 << "is: " << num << endl;
          }
        } else if (what2 == "born_after") {
          string date;
          Date born_d;
          int not_valid;
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

    } else if (cmd == "exit") {
      exit = true;
    } else {
      std::cout << "Command " << cmd << " unknown." << std::endl;
    }
  } while (!exit);
  return 0;
}
