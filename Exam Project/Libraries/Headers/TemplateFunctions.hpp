//
//  Main_Functions.h
//  Social Network
//
//  Created by Antonio Calagna on 26/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#ifndef Main_Functions_hpp
#define Main_Functions_hpp

#include <cctype>

template <typename T>
T FindByID(const string &ID, const vector<T> &vect_accounts)
{
  T blank;
  size_t size = vect_accounts.size();
  
  for (int i=0; i<size; i++)
  {
    if (vect_accounts[i].getID()==ID)
    {
      return (vect_accounts[i]);
    }
  }
  
  return blank;
}

//La funzione ritorna un iteratore all'elemento aggiunto nel vettore.
//L'algoritmo di inserimento è di complessità N*log(N), il massimo desiderabile.
//Esso fa uso della upper_bound dei vettori, sfrutta il fatto che il vettore sia già ordinato per determinare la posizione di inserimento corretta per il nuovo elemento.
//La upper_bound, basata su binary_search è molto veloce ed efficace, il colpevole del rallentamento è l'inserimento nel vettore.
//La funzione consente quindi l'inserimento di un derivato (ad es. User di tipo T) in un vettore ordinato di Account (tipo E)

template <typename T, typename E>
typename std::vector<E>::iterator insert_sorted (vector<E> &vec, T const &item)
{
  return vec.insert (upper_bound(vec.begin(), vec.end(), item), item);
}
#endif /* Main_Functions_h */
