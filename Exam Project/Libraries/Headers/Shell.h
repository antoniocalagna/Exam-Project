//
// Created by Alex on 24/06/2018.
//

#ifndef SOCIAL_NETWORK_SHELL_H
#define SOCIAL_NETWORK_SHELL_H

#include <fstream>
#include "IOBuffer.h"
#include "Manager.h"

namespace Shell {
  using Function = void(*)(Manager&, IOBuffer&, IOBuffer&);
  void get(std::stringstream &command, Manager &manager, IOBuffer &new_data, IOBuffer &data_to_delete);
}

#endif //SOCIAL_NETWORK_SHELL_H
