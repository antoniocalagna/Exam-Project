//
// Created by Alex on 13/06/2018.
//

#ifndef SOCIAL_NETWORK_IOBUFFER_HPP
#define SOCIAL_NETWORK_IOBUFFER_HPP

#include <queue>

template<typename data_T>
class IOBuffer {
private:
  std::queue<data_T> _buffer;

public:
  /**Constructors & Destructor*/
  IOBuffer() = default;
  ~IOBuffer() = default;
  
  /**General*/
  bool isEmpty() const;
  void clear();
  
  /**Operators*/
  IOBuffer<data_T> &operator<<(const data_T &new_data);
  IOBuffer<data_T> &operator>>(data_T &to_push);
};

/**####################
 * ## IMPLEMENTATION ##
 * ####################
 */
/**#############
 * ## General ##
 * #############
 */
template<typename data_T>
bool IOBuffer<data_T>::isEmpty() const {
  //Controlla se il buffer è vuoto
  return _buffer.empty();
}

template<typename data_T>
void IOBuffer<data_T>::clear() {
  //Svuota il buffer
  while (!_buffer.empty()) {
    _buffer.pop();
  }
}

/**###############
 * ## Operators ##
 * ###############
 */

template<typename data_T>
IOBuffer<data_T> &IOBuffer<data_T>::operator<<(const data_T &new_data) {
  //Aggiungi un nuovo elemento al buffer
  _buffer.push(new_data);
  return *this;
}

template<typename data_T>
IOBuffer<data_T> &IOBuffer<data_T>::operator>>(data_T &to_push) {
  //Ritorna il primo elemento del buffer e rimuovilo
  //NOTA: non vengono fatti controlli!
  to_push = _buffer.front();
  _buffer.pop();
  return *this;
}

#endif //SOCIAL_NETWORK_IOBUFFER_HPP
