#include "Post.h"
#include "User.h"
#include "Account.h"
#include "Manager.h"
#include "FileHandler.h"

using namespace std;

int main_di_clara(/*int argc, char *argv[]*/) {
  //metterei una introduzione al funzionamento per l'utente, del tipo : hi, this is a console menu, write your commands, or
  //write help for the list of commands (magari in inglese vero)
  /*FH::FileHandler accounts_fh,          //FileHandler per il controllo e l'acquisizione dei dati da file
          relations_fh,
          posts_fh;
  IOBuffer buffer;                      //Buffer necessario per l'acquisizione dei dati
  
  //Controllo dei parametri passati da linea di comando
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

  Manager manager;
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
      cout << ">";

      getline(cin, input);

      command.clear();
      command.str(input);
      command >> cmd;
    } else {
      if (cmd == "set") {
        string what, who; //non so se questi nomi sono messi per scherzo, a me sembrano carini
        command >> who >> what;
        if (what.empty()) {
          cout << "Error! Check the numbers of parameters you have to insert." << endl;
        }
        if (who == "user") {

          if (what == "name") {

          } else {
            if (what == "surname") {

            } else {
              if (what == "gender") {

              } else {
                if (what == "birth") {

                } else {
                  if (what == "address") {

                  } else {
                    if (what == "id") {

                    } else {
                      if (what == "subscription") {

                      } else {
                        cout << "Error! You can't add: " << what << "." << endl;

                      }
                    }
                  }
                }
              }
            }
          }
        } else {
          if (who == "group") {
            if (what == "name") {

            } else {
              if (what == "id") {

              } else {
                if (what == "subscription") {

                } else {
                  if (what == "location") {

                  } else {
                    if (what == "type_activity") {

                    } else {
                      if (what == "inception") {

                      } else {
                        cout << "Error! You can't add: " << what << "." << endl;
                      }
                    }
                  }
                }
              }
            }

          } else {
            if (who == "company") {
              if (what == "name") {

              } else {
                if (what == "id") {

                } else {
                  if (what == "subscription") {

                  } else {
                    if (what == "inception") {

                    } else {
                      if (what == "finantial_location") {

                      } else {
                        if (what == "operative_location") {

                        } else {
                          if (what == "products") {

                          } else {
                            cout << "Error! You can't add: " << what << "." << endl;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      } else {
        if (cmd == "get") {
          string who, what;
          command >> who >> what;
          if (what.empty()) {
            cout << "Error! Check the numbers of parameters you have to insert." << endl;
          }
          if (who == "user") {
            if (what == "info") {
              string id;
              User u;
              cout << "Please insert the id:\n>";
              cin >> id;
              u = manager.getUser(id);
              cout << u.getName() << "\n" << u.getSurname() << "\n" << u.getID() << "\n" << u.getAddress() << "\n"
                   << u.getSubscription() << "\n" << u.getBirth() << "\n" << u.getGender() << endl;
            } else {
              cout << "Error! You can't get: " << what << "." << endl;
            }


          } else {
            if (who == "group") {
              if (what == "info") {
                string id;
                Group g;
                cout << "Please insert the id:\n>";
                cin >> id;
                g = manager.getGroup(id);
                cout << g.getName() << "\n" << g.getID() << "\n" << g.getLegalLocation() << "\n"
                     << g.getTypeOfActivity()
                     << "\n" << g.getSubscription() << "\n" << g.getInception() << endl;
              } else {
                cout << "Error! You can't get: " << what << "." << endl;
              }

            } else {
              if (who == "company") {
                if (what == "info") {
                  string id;
                  Company c;
                  cout << "Please insert the id:\n>";
                  cin >> id;
                  c = manager.getCompany(id);
                  cout << c.getName() << "\n" << c.getID() << "\n" << c.getFinancialLocation() << "\n"
                       << c.getOperativeLocation() << "\n" << c.getTypeOfProduct() << "\n" << c.getSubscription()
                       << "\n"
                       << c.getInception() << endl;
                } else {
                  cout << "Error! You can't get: " << what << "." << endl;
                }

              }
            }
          }

        } else {

          if (cmd == "delete") {
            string what;
            command >> what;
            if (what.empty()) {
              cout << "Error! Check the numbers of parameters you have to insert." << endl;
            }
            if (what == "account") {
              string who;
              command >> who;
              if (who.empty()) {
                cout << "Error! Check the numbers of parameters you have to insert." << endl;
              } else {
                manager.deleteAccount(who);
              }

            } else {
              if (what == "relationship") {
                string id_start, id_target;
                cout << "Please insert: <id_1> <id_2>:\n";
                cin >> id_start >> id_target;
                if (id_start.empty() || id_target.empty()) {
                  cout << "Error! Please check the number of parameters." << endl;
                } else {
                  if (!manager.deleteRelationship(id_start, id_target)) {
                    cout << "Error! The ids you have insert don't exist." << endl;
                  }
                }
              } else {
                if (what == "post") {
                  string who, tmp_news, d_t;
                  pair<string, vector<Post>> post;
                  Date tmp_date;
                  Clock tmp_time;
                  int find = 0;
                  cout << "Please insert the id who wrote the post:\n";
                  cin >> who;
                  if (who.empty()) {
                    cout << "Error! You don't insert any id." << endl;
                  } else {
                    post = manager.getPosts(who);
                    cout << "Please insert the news of the post you want to delete:\n";
                    cin.ignore();
                    getline(cin, tmp_news);
                    cout << "Please insert the date and the time of the post you want to delete:\n";
                    cin.ignore();
                    getline(cin, d_t);

                    Post cmp_post(tmp_news, d_t);

                    for (auto it = post.second.begin(); it != post.second.end(); it++) {
                      if (*it == cmp_post) {
                        manager.deletePost(*it, who);
                        find = 1;
                      }
                    }
                    if (find != 1) {
                      cout << "Post not found!" << endl;
                    } else {
                      cout << "Done!" << endl;
                    }
                  }


                } else {
                  cout << "Error! You can't delete: " << what << "." << endl;
                }
              }
            }
          } else {
            if (cmd == "add") {
              string what1, what2;
              command >> what1 >> what2;
              if (what1.empty()) {
                cout << "Error! Check the numbers of parameters you have to insert." << endl;
              } else {

                if (what1 == "account") {

                  if (what2.empty()) {
                    cout << "Error! Check the numbers of parameters you have to insert." << endl;
                  } else {
                    if (what2 == "user") {
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
                              "'N' ...\n"
                              "'A' ...\n"
                              "'B' ...\n"
                              "'O' OTHERS\n>";
                      cin >> tmp_g;

                      User new_u(tmp_n, tmp_s, tmp_id, tmp_a, tmp_sub, tmp_b, tmp_g);
                      if (!manager.addAccount(new_u)) {
                        cout << "Error! Your id already exist!";
                      } else cout << "Done!";

                      getline(cin, input);

                      command.clear();
                      command.str(input);
                      command >> cmd;

                    } else {
                      if (what2 == "group") {
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
                          cout << "Error! Your id already exist!";
                        } else cout << "Done!";
                        getline(cin, input);

                        command.clear();
                        command.str(input);
                        command >> cmd;

                      } else {
                        if (what2 == "company") {
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
                            cout << "Error! Your id already exist!";
                          } else cout << "Done!";
                          getline(cin, input);

                          command.clear();
                          command.str(input);
                          command >> cmd;

                        }
                      }
                    }
                  }
                } else {
                  if (what1 == "relationship") {
                    int error = 0;
                    while (error != 1) {
                      cout << "Please insert: <id_1> <type_of_relation> <id_2>\n"
                              "types of relationship:\n"
                              "friendship\n"
                              "knowings\n"
                              "parent\n"
                              "born\n"
                              "partner\n"
                              "membership\n"
                              "partnership\n"
                              "employee\n"
                              "employer\n" << endl;

                      string who1, who2, type_rel;
                      getline(cin, input);

                      command.clear();
                      command.str(input);
                      command >> who1 >> type_rel >> who2;

                      if (who1.empty() || who2.empty() || type_rel.empty()) {
                        cout << "Error! Check the numbers of parameters you have to insert." << endl;
                      } else {
                        error = manager.addRelationship(who1, who2, type_rel);
                        //Analizzo il codice errore restituito da addRelationship.
                        if (error == -1) {
                          cout << "Error! The first ID does not exist!" << endl;
                        }
                        if (error == -2) {
                          cout << "Error! The second ID does not exist!" << endl;
                        }
                        if (error == -3) {
                          cout << "Error! This relationship does not exist!" << endl;
                        }
                        if (error == 1) {
                          cout << "Done!";
                        }
                      }
                    }
                  } else {

                    if (what1 == "post") {
                      string news, d_t, whose_ID;
                      set<string> likes, dislikes;
                      bool isValid = false;
                      Post post_tmp;

                      cout << "News:\n>";
                      getline(cin, news);
                      post_tmp.setNews(news);

                      do {
                        cout << "Date and time:\n>"; //a me sta cosa non funziona, sto impazzendo, non esce dal ciclo
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
                    } else{
                      cout << "Error! You can't add: " << what1 << "." << endl;
                    }

                  }
                }
              }

            } else {

              //if (cmd)


              if (cmd == "exit") {
                exit = true;
              } else {
                cout << "Error! The command: " << cmd << " doesn't exist." << endl;
              }
            }
          }
        }
      }
    }
  } while (!exit);
  return 0;
}
