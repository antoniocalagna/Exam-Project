//
// Created by clara on 24/05/2018.
//

#ifndef PROG_POST_H
#define PROG_POST_H

#include <vector>
#include <iterator>
#include <set>
#include "Clock.h"

using namespace std;


class Post {
public:
  //Constructors & Destructor
  Post();
  Post(const string &news, const Clock &time, const set<string> &likes, const set<string> &dislikes);
  ~Post()=default;
  
  //Setters
  void setNews(const string &news);
  void setTime(const Clock &time);
  void setLikes(const set<string> &likes);
  void setDislikes(const set<string> &dislikes);
  
  //Getters
  string getNews() const;
  Clock getTime() const;
  set<string> getLikes() const;
  set<string> getDislikes() const;
  
  //Overloading operators
  bool operator==(const Post &to_be_compared);
  bool operator!=(const Post &to_be_compared);
  bool operator>(const Post &is_smaller);
  bool operator<(const Post &is_greater);
  friend ostream &operator<<(ostream &stream, const Post &p);
  
  //Others
  void AddLike(const string &id);
  void AddDislike(const string &id);
  void RemoveLike(const string &id);
  void RemoveDislike(const string &id);
  bool LikeExists(const string &ID) const;
  bool DislikeExists(const string &ID) const;
  set<string>::iterator FindLike(const string &id) const;
  set<string>::iterator FindDislike(const string &id) const;
  int NumLikes(); //numero likes
  int NumDislikes();
  float RatioReaction(); //rapporto LIKE/(DISLIKE+LIKE) così da poter studiare i post più 'popolari'
  
  
private:
  string _news;
  Clock _time;
  set<string> _likes, _dislikes;
};


#endif //PROG_POST_H
