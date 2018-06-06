//
//  MainFunctions.cpp
//  Social Network
//
//  Created by Antonio Calagna on 27/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include "Manager.h"

size_t FindPosbyID (const vector<Account> &v, const string &id)
{
  //Ricerchiamo l'id richiesto all'interno del vettore (opportunamente ordinato)
  
  
  size_t a = 0, b = v.size() - 1;  //Estremi di ricerca
  size_t m;
  
  while (a < b)
  {
    m = (a + b) / 2;  //Dividiamo l'intervallo a metà
    
    if (v[a].getID() == id) {
      return a;
    }
    else if (v[b].getID() == id) {
      return b;
    }
    
    if(v[m].getID() == id)
    {
      //Abbiamo trovato l'elemento
      return m;
    }
    
    //L'elemento non è stato trovato
    else if(v[m].getID() < id)
    {
      //Abbiamo scelto un elemento che si trova prima di quello ricercato. Il nuovo intervallo è la seconda metà
      a = m;
    }
    else
    {
      //L'elemento ricercato si trova prima del punto medio (v[m] > id). L'intervallo è la prima metà
      b = m;
    }
    a++;
    b--;
  }
  /*
   * Arrivati a questo punto, l'intervallo è stato suddiviso così tante volte che a = b oppure a = b-1.
   * L'elemento non era presente.
   */
  return v.size();
}

