//
//  main.cpp
//  Social Network
//
//  Created by Antonio Calagna on 22/05/18.
//  Copyright © 2018 Antonio Calagna. All rights reserved.
//

#include <iostream>

#include "Manager.h"
#include "Account.h"


bool test_fun (Account first, User second);
size_t FindPosbyID (const vector<Account> &v, const string &id_original);

int main(int argc, const char * argv[]) {
    
    Date birth_p1 (27,10,1998);
    Date sub_p1 (10,05,2016);
    User p1 ("Antonio", "Calagna", "toti", "Via Caraglio", sub_p1, birth_p1,'M');
    
    Date birth_p2 (10,05,1969);
    Date sub_p2 (27,11,1998);
    User p2 ("Leo", "Calagna", "leo", "Via Streva", sub_p2, birth_p2, 'M');
    
    Date birth_p3 (27,04,1983);
    Date sub_p3 (30,8,1997);
    User p3 ("Signorina_Ketamina", "Monica", "MYSSKETA", "Porta Venezia", sub_p2, birth_p2, 'F');
    
    Date sub_c1 (23,05,2018);
    Date inc_c1 (25,07,2003);
    Company c1 ("Intel", "inteltopwow", "wherever", "wherever", "processors", sub_c1, inc_c1);
    
    vector<User> vect_u;
    
    vector<Company> vect_c;
    
    vector<Group> vect_g;

    vect_u.push_back(p3);
    vect_u.push_back(p1);
    vect_c.push_back(c1);
    
    vector<Account> all;
    
    all.push_back(p1);
    all.push_back(p2);
    all.push_back(p3);
    
    const string to_look = "MYSSKETA";
    
    size_t pos=0;
    pos = FindPosbyID(all, to_look);
    
    cout<<"La posizione di "<<to_look<<" è: "<<pos<<"\n"<<endl;
    
    if (all[pos].getType()==Account::user_type)
    {
        cout <<"Il nome è (Dato Account): "<<all[pos].getName() <<endl;
        User test = FindByID(to_look, vect_u);
        cout <<"Il cognome è (Dato User): "<<test.getSurname() <<"\n"<<endl;
    }
    
    vector<User>::iterator first_u = vect_u.begin();
    vector<Account>::iterator first_a = all.begin();
    vector<User>::iterator last_u = vect_u.end();
    vector<Account>::iterator last_a = all.end();
    
    cout << "Ordino il vettore di account e lo stampo.." << endl;
    sort (first_a, last_a);
    
    for (; first_a!=last_a; first_a++)
    {
        cout << first_a->getName() << " with ID: "<< first_a->getID()<<endl;
    }
    
    cout << endl;
    
    cout << "Inserisco l'elemento "<<c1.getID()<<" nel vettore di account e lo stampo"<<endl;
    insert_sorted(all, c1);
    first_a=all.begin();
    last_a=all.end();
    
    for (; first_a!=last_a; first_a++)
    {
        cout << first_a->getName() << " with ID: "<< first_a->getID()<<endl;
    }
    
    cout << endl;
    
    cout << "Inserisco l'elemento "<<p2.getID()<<" nel vettore di user e lo stampo"<<endl;
    insert_sorted(vect_u, p2);
    first_u=vect_u.begin();
    last_u=vect_u.end();
    
    for (; first_u!=last_u; first_u++)
    {
        cout << first_u->getName() << " with ID: "<< first_u->getID()<<endl;
    }
    
    Manager Manager(vect_u, vect_c, vect_g);
    
    Manager.addUndirectedRelationship("toti", "leo", relation::fatherhood);
    
    vector<Account> all_manager = Manager.getAllAccounts();
    
    /*
    char FLAG=0;
    FLAG = FindTypeByID(ID_to_look_for, vect_u, vect_c, vect_g);
    
    User result_u;
    Company result_c;
    Group result_g;
    
    if (FLAG=='U')
    {
        result_u = FindByID(ID_to_look_for, vect_u);
        cout << result_u.getName();
    }
    
    if (FLAG=='C')
    {
        result_c = FindByID(ID_to_look_for, vect_c);
        cout << result_u.getName();
    }
    
    if (FLAG=='G')
    {
        result_g = FindByID(ID_to_look_for, vect_g);
        cout << result_u.getName();
    }
    */
    
    
    /*Date testdate (10,67,2018);
     
     Clock testtime1 (12,30,15);
     Clock testtime2 (24,12,0);
     Clock testtime3 (13,63,60);*/
   
    /*
    ++testdate;
    birth_p1++;
    
    bool test = false;
    
    //Testing Date
    if (birth_p1<sub_p1)
        test = true;
    
    if (birth_p1<birth_p2)
        test = false;
    
    if (birth_p1<sub_p2)
        test = true;
    
    //Testing Users
    if (p2<p1)
        test = true;
    
    if (c1<p1)
        test=true;
    
    //test = test_fun(c1, p1);
    
    if (testtime1<testtime3)
        test = true;
    
    ++testtime1;
    ++testtime1;
    
    if (testtime1==testtime2)
        test = true;*/
    
    /*Graph<Account, string> grafo;
    grafo.addNode(p1);
    grafo.addNode(p2);
    grafo.setEdge(p1,p2,"Figliaggine");
    grafo.setEdge(p2, p2, "Patricinio");
    */
    
    
    return 0;
}

bool test_fun (Account first, User second)
{
    return first<second;
}
