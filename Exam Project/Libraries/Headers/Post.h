//
// Created by clara on 24/05/2018.
//

#ifndef PROG_POST_H
#define PROG_POST_H

#include <vector>
#include <iterator>
#include "Clock.h"

using namespace std;


class Post {
public:
    //Constructors & Destructor
    Post();
    Post(const string &iduser, const string &news, const Clock &time, const vector<string> &likes,
         const vector<string> &dislikes);
    ~Post();

    //Setters
    void setNews(const string &news);
    void setIdUser(const string &iduser);
    void setTime(const Clock &time);
    void setLike(const vector<string> &likes);
    void setDislike(const vector<string> &dislikes);

    //Getters
    string getNews() const;
    string getIdUser() const;
    Clock getTime() const;
    vector<string> getLikes() const;
    vector<string> getDislikes() const;

    //Overloading operators
    bool operator==(const Post &to_be_compared);//non comparo i likes perchè mi sembrano parametri troppo variabili
    bool operator!=(const Post &to_be_compared);
    bool operator>(const Post &is_smaller); //potrebbero basarsi sul numero di likes
    bool operator<(const Post &is_greater);
    friend ostream &operator<<(ostream &stream, const Post &p);

    //Others
    void AddLike(const string &id);
    void AddDislike(const string &id);
    void RemoveLike(const string &id);
    void RemoveDislike(const string &id);
    int SearchLike(const string &ID) const; //passo un ID, ritorna l'indice del vettore se il likes c'è, sennò ritorna -1
    int SearchDislike(const string &ID)const;
    int NumLikes(); //numero likes
    int NumDislikes();
    float RatioReaction(); //rapporto LIKE/(DISLIKE+LIKE) così da poter studiare i post più 'popolari'


private:
    string _news;
    string _iduser;
    Clock _time;
    vector<string> _likes, _dislikes; //empty constructors

    //ho deciso di usare VETTORI e non LISTE, perchè così posso accedervi tramite indice, senza dover sfogliare dall'inizio
    //(((((me l'ha detto Scisca, se non è vero ha sbagliato lui.))))) :-) tvb

};


#endif //PROG_POST_H
