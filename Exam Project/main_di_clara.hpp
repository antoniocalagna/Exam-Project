#include "Post.h"
#include "User.h"
#include "Account.h"

using namespace std;

int main_di_clara() {


    //metterei una introduzione al funzionamento per l'utente, del tipo : hi, this is a console menu, write your commands, or
    //write help for the list of commands (magari in inglese vero)

    string input;
    string cmd;
    int exit = 0;

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
                    << endl;
            //per ora mi occupo solo dei set
            cout << "Groups:" << endl;
            cout
                    << "\nset name <group_id>\nset n_members <group_id>\nset location <group_id>\nset type_activity <group_id>\nset inception <group_id>\nset id <group_id>\nset subscription <group_id>\n"
                    << endl;
            //altra roba
            cout << "Companies:" << endl;
            cout
                    << "\nset name <company_id>\nset finantial_location <company_id>\nset operative_location <company_id>\nset products <company_id>\nset inception <company_id>\nset id <group_id>\nset subscription <company_id>\n"
                    << endl;
            cout << ">";

            getline(cin, input);

            stringstream command(input);
            command >> cmd;
        }
        if (cmd == "set") {
            string what, who; //non so se questi nomi sono messi per scherzo, a me sembrano carini
            command >> what >> who;
            if (who== "") {
                cout << "Error! Check the numbers of parameters you have to insert." << endl;
            }/*NON POSSO METTERE command.good(), OGNI VOLTA CHE VEDE LA FINE DELLA RIGA ROMPE IL CAZZO*/
            else /*if(who appartiene al vettore di ID preso dal file)*/{
                if (what == "name") {
                    //prendo dal vettore di id quello con id==who, chiedo all'utente il nuovo nome e con
                    //setname(nuovonome) lo cambio.
                }
                if (what == "surname") {

                }
                if (what == "gender") {

                }
                if (what == "birth") {

                }
                if (what == "address") {

                }
                if (what == "id") {

                }
                if (what == "subscription") {

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
        if (cmd=="exit"){
            exit=1;
        }

    }
    while(exit==0);
    return 0;
}
