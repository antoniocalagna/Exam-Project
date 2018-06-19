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
              << "\nset name <user_id>\nset surname <user_id>\nset gender <user_id>\nset birth <user_id>\nset address <user_id>\nset id <user_id>\nset subscription <user_id>\n"
              << endl
              << "get name <user_id>\nget surname <user_id>\nget gender <user_id>\nget birth <user_id>\nget address <user_id>\nget id <user_id>\nget subscription <user_id>\n"
              << endl;
      cout << "add account user\n";
      cout << "delete <id>\n"
           << endl;

      cout << "Groups:" << endl;
      cout
              << "\nset name <group_id>\nset n_members <group_id>\nset location <group_id>\nset type_activity <group_id>\nset inception <group_id>\nset id <group_id>\nset subscription <group_id>\n"
              << endl
              << "get name <group_id>\nget n_members <group_id>\nget location <group_id>\nget type_activity <group_id>\nget inception <group_id>\nget id <group_id>\nget subscription <group_id>\n"
              << endl;
      cout << "add account group\n";
      cout << "delete <id>\n"
           << endl;

      cout << "Companies:" << endl;
      cout
              << "\nset name <company_id>\nset finantial_location <company_id>\nset operative_location <company_id>\nset products <company_id>\nset inception <company_id>\nset id <company_id>\nset subscription <company_id>\n"
              << endl
              << "get name <company_id>\nget finantial_location <company_id>\nget operative_location <company_id>\nget products <company_id>\nget inception <company_id>\nget id <company_id>\nget subscription <company_id>\n"
              << endl;
      cout << "add account company\n";
      cout << "delete <id>\n"
           << endl;
      cout << "add relationship\n"
           << endl;
      cout << ">";

      getline(cin, input);

      command.clear();
      command.str(input);
      command >> cmd;
    }
    if (cmd == "set") {
      string what, who; //non so se questi nomi sono messi per scherzo, a me sembrano carini
      command >> what >> who;
      if (who == "") {
        cout << "Error! Check the numbers of parameters you have to insert." << endl;
      }/*NON POSSO METTERE command.good(), OGNI VOLTA CHE VEDE LA FINE DELLA RIGA ROMPE IL CAZZO*/
      else /*if(who appartiene al vettore di ID preso dal file)*/{
        if (what == "name") {
          //prendo dal vettore di id quello con id==who, chiedo all'utente il nuovo nome e con
          //setname(nuovonome) lo cambio
          //GENERICO PER TUTTI GLI ACCOUNT
        }
        if (what == "surname") {
          //se l'account non è user deve dare errore
        }
        if (what == "gender") {

        }
        if (what == "birth") {

        }
        if (what == "address") {

        }
        if (what == "id") {
          //GENERICO PER TUTTI GLI ACCOUNT
        }
        if (what == "subscription") {
          //GENERICO PER TUTTI GLI ACCOUNT
        }
        if (what == "n_members") {

        }
        if (what == "location") {

        }
        if (what == "type_activity") {

        }
        if (what == "inception") {

        }
        if (what == "finantial_location") {

        }
        if (what == "operative_location") {

        }
        if (what == "products") {

        }
      }
      /*else
       * {dai un messaggio di errore}*/
    }
    if (cmd == "get") {
      string what, who;
      command >> what >> who;
      if (who == "") {
        cout << "Error! Check the numbers of parameters you have to insert." << endl;
      } else /*if(who appartiene al vettore di ID preso dal file)*/{
        if (what == "name") {
          //prendo dal vettore di id quello con id==who
          // faccio Account.getname(who) lo cambio.
          //GENERICO PER TUTTI GLI ACCOUNT
        }
        if (what == "surname") {
          //se l'account non è user deve dare errore
        }
        if (what == "gender") {

        }
        if (what == "birth") {

        }
        if (what == "address") {

        }
        if (what == "id") {
          //GENERICO PER TUTTI GLI ACCOUNT
        }
        if (what == "subscription") {
          //GENERICO PER TUTTI GLI ACCOUNT
        }
        if (what == "n_members") {

        }
        if (what == "location") {

        }
        if (what == "type_activity") {

        }
        if (what == "inception") {

        }
        if (what == "finantial_location") {

        }
        if (what == "operative_location") {

        }
        if (what == "products") {

        }
      }
      /*else
       * {dai un messaggio di errore}*/
    }
    if (cmd == "delete") {
      string who;
      command >> who;
      if (who == "") {
        cout << "Error! Check the numbers of parameters you have to insert." << endl;
      } else /*if(who appartiene al vettore di ID preso dal file)*/{
        //si usa il distruttore per eliminare o imposto tutti i paramentri vuoti?
        //o forse lo fa l'onnisciente Manager

      }
      /*else
       * {dai un messaggio di errore}*/
    }
    if (cmd == "add") {
      string what1, what2;
      command >> what1 >> what2;
      if (what1 == "") {
        cout << "Error! Check the numbers of parameters you have to insert." << endl;
      } else {

        if (what1 == "account") {

          if (what2 == "") {
            cout << "Error! Check the numbers of parameters you have to insert." << endl;
          } else {
            if (what2 == "user") {

            }
            if (what2 == "group") {

            }
            if (what2 == "company") {

            }
          }
        }
        if (what1 == "relationship") {
          int error = 0;
          while (error!=1)
          {
            cout<<"Please insert: <id_1> <type_of_relation> <id_2>\n"
                  "types of relationship:\n"
                  "friendship\n"
                  "knowings\n"
                  "parent\n"
                  "born\n"
                  "partner\n"
                  "membership\n"
                  "partnership\n"
                  "employee\n"
                  "employer\n"<<endl;

            string who1, who2, type_rel;
            getline(cin, input);

            command.clear();
            command.str(input);
            command >> who1 >> type_rel >> who2;

            if(who1 == "" || who2 == "" || type_rel == ""){
              cout<<"Error! Check the numbers of parameters you have to insert." << endl;
            }
              error = manager.addRelationship(who1, who2, type_rel);
              //Analizzo il codice errore restituito da addRelationship.
            if(error == -1){
              cout<<"Error! The first ID does not exist!"<<endl;
            }
            if (error == -2){
              cout<<"Error! The second ID does not exist!"<<endl;
            }
            if (error == -3){
              cout << "Error! This relationship does not exist!"<<endl;
            }
          }
        }

        if (what1 == "post") {
          string news, d_t, whose_ID;
          set <string> likes, dislikes;
          bool isValid = false;
          Post post_tmp;
          
          cout<<"News:\n>";
          getline(cin, /*input*/news);
          post_tmp.setNews(news);
          
          /*command.clear();
          command.str(input);
          command >> news;*/ //SE FAI COSI' TI ACCHIAPPA SOLO LA PRIMA PAROLA!
          
          do {
            cout<<"Date and time:\n>";
            getline(cin, /*input*/ d_t);
            isValid=post_tmp.setDate_Time(d_t);
              
            /*command.clear();
            command.str(input);
            command >> d_t;*/
            
          }
          while (!isValid);

          cout<<"Likes:\n";
          while(input!="EOL") {
            cout<<">";
            getline(cin, input);
            if (input!="EOL")
              likes.insert(input);
          }
          post_tmp.setLikes(likes);

          cout<<"Dislikes:\n";
          input.clear();
          while(input!="EOL") {
            cout<<">";
            getline(cin, input);
            if (input!="EOL")
              dislikes.insert(input);
          }
          post_tmp.setDislikes(dislikes);

          cout << news << " " << d_t << endl;
          
          if (!likes.empty())
          {
            cout<<"\nLikes:"<< endl;
            
            for(auto it=likes.begin(); it!=likes.end(); it++ ){
              cout<<*it<<endl;
            }
          }
          
          if (!dislikes.empty())
          {
            cout << "\nDislikes:" << endl;
            for(auto it=dislikes.begin(); it!=dislikes.end(); it++ ) {
              cout<<*it<<endl;
            }
          }
          
          cout<<"Who wrote the post?:\n>";
          getline(cin, /*input*/whose_ID);
          if (!manager.addPost(post_tmp, whose_ID))
            cout<<"Something went wrong.."<<endl;
        }

      }

    }


    if (cmd == "exit") {
      exit = true;
    }

  } while (!exit);
  return 0;
}
