//
// Created by Alex on 21/06/2018.
//

#ifndef SOCIAL_NETWORK_MAIN_2_HPP
#define SOCIAL_NETWORK_MAIN_2_HPP

#include <array>
#include "Manager.h"
#include "FileHandler.h"

int main_2(int argc, char *argv[]) {
  Manager network;
  FH::FileHandler accountsFH,
          relationsFH,
          postsFH;
  std::set<std::string> commands = {"help", "get", "set", "list", "exit"};
  
  std::cout << "Welcome. This is program is console based, so commands must be typed in console-style."
            << "\nType \"help\" for a list of useful commands.";
  
  std::string user_input;
  
  do {
    do {
      std::getline(std::cin, user_input);
      std::stringstream command(user_input);
    } while(!command.good());
    
  } while(command[0] != "exit");
}

#endif //SOCIAL_NETWORK_MAIN_2_HPP
