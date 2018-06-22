//
// Created by clara on 24/05/2018.
//

#ifndef PROG_POST_H
#define PROG_POST_H

#include <iterator>
#include <set>
#include <vector>
#include "Date.h"
#include "Clock.h"

using namespace std;


class Post {
public:
  //Constructors & Destructor
  Post();
  Post(const string &news, const string &date_time, const set<string> &likes, const set<string> &dislikes);
  Post(const string &news, const string &date_time);
  ~Post()=default;
  
  //Setters
  void setNews(const string &news);
  bool setDate_Time(const string &date_time);
  void setLikes(const set<string> &likes);
  void setDislikes(const set<string> &dislikes);
  
  //Getters
  string getNews() const;
  Date getDate() const;
  Clock getTime() const;
  vector<string> getLikes() const;
  vector<string> getDislikes() const;
  
  //Overloading operators
  bool operator==(const Post &to_be_compared);
  bool operator!=(const Post &to_be_compared);
  bool operator>(const Post &is_smaller);
  bool operator<(const Post &is_greater);
  friend ostream &operator<<(ostream &stream, const Post &p);
  
  //Others
  bool AddLike(const string &id);
  bool AddDislike(const string &id);
  bool RemoveLike(const string &id);
  bool RemoveDislike(const string &id);
  bool LikeExists(const string &ID) const;
  bool DislikeExists(const string &ID) const;
  int NumLikes() const; //numero likes
  int NumDislikes() const;
  float RatioReaction() const; //rapporto LIKE/(DISLIKE+LIKE) così da poter studiare i post più 'popolari'
  
private:
  string _news;
  Date _date;
  Clock _time;
  set<string> _likes, _dislikes;
};


#endif //PROG_POST_H
