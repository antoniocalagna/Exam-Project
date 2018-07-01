#include "Shell.h"
#include "FileHandler.h"
#include <vector>
#include "Social_Handlers.h"

#define BUFF_TOTAL_SIZE_MAX 5
#define CYCLES_WITHOUT_SAVING_MAX 10

using namespace std;

void flushBuffers(FH::FileHandler &fh, IOBuffer &new_data, IOBuffer &data_to_delete);

int main_di_clara(int argc, const char *argv[]) {
  Manager manager;
  FH::AccountsHandler accounts_fh;      //FileHandlers per il controllo e l'acquisizione dei dati da file
  FH::RelationsHandler relations_fh;
  FH::PostsHandler posts_fh;
  
  IOBuffer new_data_buffer;                                 //Buffer necessario per l'acquisizione dei dati
  IOBuffer data_to_erase_buffer;                            //Buffer per i dati da cancellare
  
  //Controllo dei parametri passati da linea di comando
  if (argc != 4) {
    std::cerr
            << "Parameters error. Plese insert input file names as follows: <accounts_file> <relations_file> <posts_file>."
            << std::endl;
    return -1;
  }
  
  //Apri e controlla i file richiesti.
  accounts_fh.open(argv[1]);
  relations_fh.open(argv[2]);
  posts_fh.open(argv[3]);
  
  
  ///////////////////////////////////              CONTROLLI             //////////////////////////////////////////////
  std::cout << "" << "Beginning checks:" << "" << std::endl;
  //Controllo del file degli account
  std::cout << "Checking accounts file..." << std::endl;
  FH::Error check_results = accounts_fh.checkFile();
  if (check_results.code == 0) {
    std::cout << "\t*Accounts file correctly formatted and ready to be read. "
                 "(" << check_results.data << " lines analyzed)" << std::endl;
  }
  else {
    std::cerr << "** Accounts file returned error code " << std::hex << check_results.code << " at line " << std::dec
              << check_results.data
              << "."
              << " File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  std::cout << "Checking relations file..." << std::endl;
  //Controllo del file delle relazioni
  check_results = relations_fh.checkFile();
  if (check_results.code == 0) {
    std::cout << "\t*Relations file correctly formatted and ready to be read. "
                 "(" << check_results.data << " lines analyzed)" << std::endl;
  }
  else {
    std::cerr << "** Relations file returned error code " << std::hex << check_results.code << " at line " << std::dec
              << check_results.data
              << "."
              << " File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  //Controllo del file dei post
  std::cout << "Checking posts file..." << std::endl;
  check_results = posts_fh.checkFile();
  if (check_results.code == 0) {
    std::cout << "\t*Posts file correctly formatted and ready to be read. "
                 "(" << check_results.data << " lines analyzed)" << std::endl;
  }
  else {
    std::cerr << "** Posts file returned error code " << std::hex << check_results.code << " at line "
              << check_results.data << "."
              << " File needs to be corrected before data can be read." << std::endl;
    return -1;
  }
  
  //Controlli completati
  std::cout << "Checks completed.\n" << std::endl;
  
  ///////////////////////////////////            ACQUISIZIONE             //////////////////////////////////////////////
  std::cout << "Beginning acquisition:" << std::endl;
  
  IOBuffer reorganizing_buffer;
  std::cout << "Acquiring accounts..." << std::endl;
  accounts_fh.fetchData(new_data_buffer);                           //Leggi i dati dal file degli account
  
  reorganizing_buffer.append(new_data_buffer);                      //Prepara il buffer per la riorganizzazione dei file
  while (!new_data_buffer.usersEmpty()) {                           //Svuota il buffer degli utenti
    User user_tmp;
    new_data_buffer >> user_tmp;
    if (!manager.addAccount(user_tmp)) {
      std::cerr << "** Accounts file returned an error. " << user_tmp.getID() << "is not unique.\n"
                << " File needs to be corrected before data can be acquired." << std::endl;
      return -2;; //ritorno -2 quando l'id non è univoco
    }
  }
  while (!new_data_buffer.groupsEmpty()) {                          //Svuota il buffer dei gruppi
    Group group_tmp;
    new_data_buffer >> group_tmp;
    if (!manager.addAccount(group_tmp)) {
      std::cerr << "** Accounts file returned an error. " << group_tmp.getID() << "is not unique.\n"
                << " File needs to be corrected before data can be acquired." << std::endl;
      return -2;
    }
  }
  while (!new_data_buffer.companiesEmpty()) {                       //Svuota il buffer delle compagnie
    Company comp_tmp;
    new_data_buffer >> comp_tmp;
    if (!manager.addAccount(comp_tmp)) {
      std::cerr << "** Accounts file returned an error. " << comp_tmp.getID() << "is not unique.\n"
                << " File needs to be corrected before data can be acquired." << std::endl;
      return -2;
    }
  }
  std::cout << "\t*Accounts correctly acquired." << std::endl;
  
  std::cout << "Acquiring relations..." << std::endl;
  
  relations_fh.fetchData(new_data_buffer);
  reorganizing_buffer.append(new_data_buffer);                        //Prepara il buffer per la riorganizzazione
  
  while (!new_data_buffer.relationsEmpty()) {
    IOBuffer::Relation relation_tmp;
    new_data_buffer >> relation_tmp;
    string id_start, id_target, relationship;
    id_start = relation_tmp.first.first;
    id_target = relation_tmp.first.second;
    relationship = relation_tmp.second;
    int err = manager.addRelationship(id_start, id_target, relationship);
    if (err == -1) {
      std::cerr << "** Relations file returned an error: the id " << id_start << " does not exist.\n"
                << " File needs to be corrected before data can be acquired." << std::endl;
      return -3; //ritorno -3 quando l'id non esiste
      
    }
    else if (err == -2) {
      std::cerr << "** Relations file returned an error: the id " << id_target << " does not exist.\n"
                << " File needs to be corrected before data can be acquired." << std::endl;
      return -3;
      
    }
    else if (err == -3) {
      std::cerr << "** Relations file returned an error: the relationship " << relationship << " does not exist.\n"
                << " File needs to be corrected before data can be acquired." << std::endl;
      return -4; //ritorno -4 quando la relazione non esiste o non può esistere
      
    }
    else if (err == -4) {
      std::cerr << "** Relations file returned an error: the relationship " << relationship
                << " and the Users' ages are not compatible.\n"
                << " File needs to be corrected before data can be acquired." << std::endl;
      return -4;
      
    }
  }
  std::cout << "\t*Relations correctly acquired." << std::endl;
  
  std::cout << "Acquiring posts..." << std::endl;
  posts_fh.fetchData(new_data_buffer);
  reorganizing_buffer.append(new_data_buffer);
  
  while (!new_data_buffer.postsEmpty()) {
    IOBuffer::m_Post post_tmp;
    new_data_buffer >> post_tmp;
    if (!manager.addPost(post_tmp.second, post_tmp.first)) {
      std::cerr << "** Posts file returned an error: the id " << post_tmp.first << " does not exist or an interaction does not belong to a user.\n"
                << " File needs to be corrected before data can be acquired." << std::endl;
      return -3;
    }
  }
  
  std::cout << "\t*Posts correctly acquired." << std::endl;
  std::cout << "Acquisition completed.\n" << std::endl;
  ///////////////////////////////////       RIORGANIZZAZIONE DEI FILE     //////////////////////////////////////////////
  std::cout << "Beginning accounts file reorganization..." << std::endl;
  accounts_fh.clear();
  accounts_fh.putData(reorganizing_buffer);
  std::cout << "\t*Accounts file correctly reorganized." << std::endl;
  
  std::cout << "Beginning relations file reorganization..." << std::endl;
  relations_fh.clear();
  relations_fh.putData(reorganizing_buffer);
  std::cout << "\t*Relations file correctly reorganized." << std::endl;
  
  std::cout << "Beginning posts file reorganization..." << std::endl;
  posts_fh.clear();
  posts_fh.putData(reorganizing_buffer);
  std::cout << "\t*Posts file correctly reorganized.\n" << std::endl;
  
  ///////////////////////////////////          INIZIO  PROGRAMMA          //////////////////////////////////////////////
  std::cout << "Welcome. This program is shell based, so commands should be typed in shell-style.\n"
               "Type \"help\" for a list of useful commands." << std::endl;
  
  const std::map<std::string, Shell::Function> commands{      //Mappa che lega il nome del comando alla sua funzione
          std::make_pair("help", Shell::help),          //Inizzializiamo le coppie come pair nome - pointer a funzione
          std::make_pair("list", Shell::list),
          std::make_pair("get", Shell::get),
          std::make_pair("set", Shell::set),
          std::make_pair("delete", Shell::del),
          std::make_pair("stats", Shell::stats),
          std::make_pair("add", Shell::add),
          std::make_pair("search", Shell::search)
  };
  
  bool exit = false;
  bool save_data = false;
  bool autosave = true;
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
    else if (cmd == "autosave") {
      std::string request;
      command >> request;
      if (request == "on") {
        autosave = true;
        std::cout << "Autosave turned on." << std::endl;
      }
      else if (request == "off") {
        autosave = false;
        std::cout << "Autosave turned off." << std::endl;
      }
      else {
        std::cout << "Cannot set autosave " << request << ".\n"
                  << "Correct format: autosave on/off" << std::endl;
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
    if ((new_data_buffer.size() + data_to_erase_buffer.size() >= BUFF_TOTAL_SIZE_MAX ||
         cycles_without_saving >= CYCLES_WITHOUT_SAVING_MAX) &&
        autosave) {
      save_data = true;
    }
    
    if (save_data) {
      std::cout << "Saving data..." << std::endl;
      accounts_fh.putData(new_data_buffer, data_to_erase_buffer);
      relations_fh.putData(new_data_buffer, data_to_erase_buffer);
      posts_fh.putData(new_data_buffer, data_to_erase_buffer);
      std::cout << "Data saved." << std::endl;
      cycles_without_saving = 0;
      save_data = false;
    }
    else {
      cycles_without_saving++;
    }
    command.str(std::string());   //Svuota il buffer del comando
  } while (!exit);
  
  accounts_fh.close();
  relations_fh.close();
  posts_fh.close();
  return 0;
}
