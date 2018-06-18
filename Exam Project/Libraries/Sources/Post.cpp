//
// Created by clara on 24/05/2018.
//

#include "Post.h"

//Constructors & Destructor
Post::Post() {
  _news = "blank";
  _date = Date();
  _time = Clock();
}

Post::Post(const string &news, const string &date_time, const set<string> &likes, const set<string> &dislikes) {
  _news = news;
  _likes = likes;
  _dislikes = dislikes;
  setDate_Time(date_time);
}

//Setters
void Post::setNews(const string &news) {
  _news = news;
}

void Post::setDate_Time(const string &date_time) {
  string date = date_time.substr(0, 10);
  if (date_time.size() < 11)
    _time = Clock();
  else {
    string time = date_time.substr(11, 8);
    _time.scanTimeByStr(time);
  }
  _date.scanDateByStr(date);
  
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

Clock Post::getTime() const {
  return _time;
}

set<string> Post::getLikes() const {
  return _likes;
}

set<string> Post::getDislikes() const {
  return _dislikes;
}

//Operators
bool Post::operator==(const Post &to_be_compared) {
  return ((_news == to_be_compared._news) && (_date == to_be_compared._date) && (_time == to_be_compared._time));
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
  stream << p._news << ", " << p._date;
  if (Clock() != p._time)
    stream << p._time;
  else
    stream << endl;
  stream << "likes: {";
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
  
  stream << "dislikes: {";
  
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
  if (DislikeExists(id) == false) {
    pair<set<string>::iterator, bool> ret;
    ret = _likes.insert(id);
    if (ret.second == true) {
      return true;
    }
  }
  return false;
  
}

bool Post::AddDislike(const string &id) {
  if (LikeExists(id) == false) {
    pair<set<string>::iterator, bool> ret;
    ret = _dislikes.insert(id);
    if (ret.second == true) {
      return true;
    }
  }
  return false;
  
  return true;
}

bool Post::RemoveLike(const string &id) {
  int s = (int) _likes.size();
  if (LikeExists(id) != false) {
    _likes.erase(id);
    if (s == _likes.size() + 1) {
      return true;
    }
  }
  return false;
}

bool Post::RemoveDislike(const string &id) {
  int s = (int) _dislikes.size();
  if (DislikeExists(id) != false) {
    _dislikes.erase(id);
    if (s == _dislikes.size() + 1) {
      return true;
    }
  }
  return false;
}
