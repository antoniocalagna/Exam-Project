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
Post::Post(const string &news, const string &date_time){
  _news = news;
  setDate_Time(date_time);
}

//Setters
void Post::setNews(const string &news) {
  _news = news;
}

bool Post::setDate_Time(const string &date_time) {
  string date = date_time.substr(0, 10);
  if (date_time.size() < 11)
    _time = Clock();
  else {
    string time = date_time.substr(11, 8);
    if (Clock::CheckTime(time))
      _time.scanTimeByStr(time);
    else
      return false;
  }
  if (Date::CheckDate(date))
  {
    _date.scanDateByStr(date);
    return true;
  }
  else
    return false;
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

vector<string> Post::getLikes() const {
  vector<string> out(_likes.size());
  int i = 0;
  for(auto it = _likes.begin(); it != _likes.end(); it++, i++)
    out[i] = *it;
  return out;
}

vector<string> Post::getDislikes() const {
  vector<string> out(_dislikes.size());
  int i = 0;
  for(auto it = _dislikes.begin(); it != _dislikes.end(); it++, i++)
    out[i] = *it;
  return out;
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
  if (!p._likes.empty()) {
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
  
  if (!p._dislikes.empty()) {
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

int Post::NumLikes() const {
  return (int) _likes.size();
}

int Post::NumDislikes() const {
  return (int) _dislikes.size();
}

float Post::RatioReaction() const{
  if ((NumDislikes() + NumLikes()) == 0) {
    return 0;
  } else {
    return ((float) NumLikes() / (NumLikes() + NumDislikes()));
  }
}

bool Post::AddLike(const string &id) {
  if (!DislikeExists(id)) {
    pair<set<string>::iterator, bool> ret;
    ret = _likes.insert(id);
    if (ret.second) {
      return true;
    }
  }
  return false;
}

bool Post::AddDislike(const string &id) {
  if (!LikeExists(id)) {
    pair<set<string>::iterator, bool> ret;
    ret = _dislikes.insert(id);
    if (ret.second) {
      return true;
    }
  }
  return false;
}

bool Post::RemoveLike(const string &id) {
  if (LikeExists(id)) {
    _likes.erase(id);
      return true;
    }
  
  return false;
}

bool Post::RemoveDislike(const string &id) {
  if (LikeExists(id)) {
    _dislikes.erase(id);
    return true;
  }
  
  return false;
}
