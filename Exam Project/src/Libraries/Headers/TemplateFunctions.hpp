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
//L'algoritmo di inserimento è di complessità N*log(N), il massimo desiderabile per i vettori.
//Esso fa uso della upper_bound dei vettori, sfrutta il fatto che il vettore sia già ordinato per determinare la posizione di inserimento corretta per il nuovo elemento.
//La upper_bound, basata su binary_search è molto veloce ed efficace, il colpevole del rallentamento è l'inserimento nel vettore.
//La funzione consente quindi l'inserimento di un derivato (ad es. User di tipo T) in un vettore ordinato di Account (tipo E)

template <typename T, typename E>
typename std::vector<E>::iterator insert_sorted (vector<E> &vec, T const &item)
{
  return vec.insert (upper_bound(vec.begin(), vec.end(), item), item);
}

template <typename AccountType>
size_t FindPosbyID (const vector<AccountType> &v, const string &ID)
{
  //Ricerchiamo l'id richiesto all'interno del vettore (opportunamente ordinato)
  
  size_t a = 0, b = v.size() - 1;  //Estremi di ricerca
  size_t m;
  
  while (a < b)
  {
    m = (a + b) / 2;  //Dividiamo l'intervallo a metà
    
    if (v[a].getID() == ID) {
      return a;
    }
    else if (v[b].getID() == ID) {
      return b;
    }
    if(v[m].getID() == ID) {
      //Abbiamo trovato l'elemento
      return m;
    }
    //L'elemento non è stato trovato
    else if(v[m].getID() < ID) {
      //Abbiamo scelto un elemento che si trova prima di quello ricercato. Il nuovo intervallo è la seconda metà
      a = m;
    }
    else {
      //L'elemento ricercato si trova prima del punto medio (v[m] > id). L'intervallo è la prima metà
      b = m;
    }
    
    a++;
    b--;
  }
  /*
   * Arrivati a questo punto, l'intervallo è stato suddiviso così tante volte che a >= b.
   * L'elemento non era presente.
   */
  return v.size();
}

#endif /* Main_Functions_h */ 
