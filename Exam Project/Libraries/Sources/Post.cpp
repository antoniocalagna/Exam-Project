//
// Created by clara on 24/05/2018.
//

#include "Post.h"

//Constructors & Destructor
Post::Post() {
  _news = "blank";
  _date = Date();
}

Post::Post(const string &news, const Date &date, const set<string> &likes, const set<string> &dislikes) {
  _news = news;
  _date = date;
  _likes = likes;
  _dislikes = dislikes;
}

//Setters
void Post::setNews(const string &news) {
  _news = news;
}

void Post::setDate(const Date &date) {
  _date = date;
}

void Post::setLikes(const set<string> &likes) {
  _likes = likes;
}

void Post::setDislikes(const set<string> &dislikes) {
  _dislikes = dislikes;
}

//Getters
string Post::getNews() const {
  return _news;
}

Date Post::getDate() const {
  return _date;
}

set<string> Post::getLikes() const {
  return _likes;
}

set<string> Post::getDislikes() const {
  return _dislikes;
}

//Operators
bool Post::operator==(const Post &to_be_compared) {
  return ((_news == to_be_compared._news) && (_date == to_be_compared._date));
}

bool Post::operator!=(const Post &to_be_compared) {
  return !(*this == to_be_compared);
}

bool Post::operator>(const Post &is_smaller) {
  return (_likes.size() > is_smaller._likes.size()); //si farà con tutti sti punti, bho
}

bool Post::operator<(const Post &is_greater) {
  return !((*this > is_greater) || (*this == is_greater));
}

ostream &operator<<(ostream &stream, const Post &p) {
  stream << p._news << ", " << p._date << "like: {";
  if (p._likes.size() != 0) {
    auto it = p._likes.begin();
    int count = 0;
    while (count != p._likes.size() - 1) {
      stream << *it << ", ";
      it++;
      count++;
    }
    stream << *it << "}, ";
  } else { stream << "}, "; }

  stream << "dislike: {";

  if (p._dislikes.size() != 0) {
    auto it = p._dislikes.begin();
    int count = 0;
    while (count != p._dislikes.size() - 1) {
      stream << *it << ", ";
      it++;
      count++;
    }
    stream << *it << "} " << endl;
  } else { stream << "}, " << endl; }

  return stream;

}

//Others
bool Post::LikeExists(const string &ID) const {
  if (_likes.find(ID) != _likes.end()) {
    return true;
  } else { return false; }
}

bool Post::DislikeExists(const string &ID) const {
  if (_dislikes.find(ID) != _dislikes.end()) {
    return true;
  } else { return false; }

}

set<string>::iterator Post::FindLike(const string &id) const {
  if (!LikeExists(id)) {
    return _likes.end();
  }

  return _likes.find(id);
}

set<string>::iterator Post::FindDislike(const string &id) const {
  if (!DislikeExists(id)) {
    return _dislikes.end();
  }

  return _dislikes.find(id);
}

int Post::NumLikes() {
  return (int) _likes.size();
}

int Post::NumDislikes() {
  return (int) _dislikes.size();
}

float Post::RatioReaction() {
  if ((NumDislikes() + NumLikes()) == 0) {
    return 0;
  } else {
    return ((float) NumLikes() / (NumLikes() + NumDislikes()));
  }
}

bool Post::AddLike(const string &id) {
  if(DislikeExists(id) == false) {
    pair<set<string>::iterator, bool> ret;
    ret = _likes.insert(id);
    if (/*LikeExists(id) == true || */ret.second == true) { //condizione ridondante, insert già controlla l'unicità dell'elemento inserito
      return true;
    }
  }
    return false;

}

bool Post::AddDislike(const string &id) {
  if(LikeExists(id) == false) {
    pair<set<string>::iterator, bool> ret;
    ret = _dislikes.insert(id);
    if (/*DislikeExists(id) == true || */ret.second == true) { //condizione ridondante
      return true;
    }
  }
  return false;

  return true;
}

bool Post::RemoveLike(const string &id) {
  int s = _likes.size();
  if (LikeExists(id) != false) {
    _likes.erase(id);
    if (s == _likes.size()+1){
      return true; }
  }
  return false;
}

bool Post::RemoveDislike(const string &id) {
  int s = _dislikes.size();
  if (DislikeExists(id) != false) {
    _dislikes.erase(id);
    if (s == _dislikes.size()+1){
      return true; }
  }
  return false;
}
