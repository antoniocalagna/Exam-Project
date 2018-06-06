//
// Created by clara on 24/05/2018.
//

#include "Post.h"

//Constructors & Destructor
Post::Post() {
  _news = "blank";
  _iduser = "blank";
  _time = Clock();
}

Post::Post(const string &iduser, const string &news, const Clock &time, const vector<string> &likes,
           const vector<string> &dislikes) {
  _iduser = iduser;
  _news = news;
  _time = time;
  _likes = likes;
  _dislikes = dislikes;
  
}

Post::~Post() {
  
}

//Setters
void Post::setNews(const string &news) {
  _news = news;
}

void Post::setIdUser(const string &iduser) {
  _iduser = iduser;
}

void Post::setTime(const Clock &time) {
  _time = time;
}

void Post::setLike(const vector<string> &likes) {
  _likes=likes;
}

void Post::setDislike(const vector<string> &dislikes) {
  _dislikes=dislikes;
}

//Getters
string Post::getNews() const {
  return _news;
}

string Post::getIdUser() const {
  return _iduser;
}

Clock Post::getTime() const {
  return _time;
}

vector<string> Post::getLikes() const {
  return _likes;
}

vector<string> Post::getDislikes() const {
  return _dislikes;
}

//Operators
bool Post::operator==(const Post &to_be_compared) {
  return ((_iduser == to_be_compared._iduser) && (_news == to_be_compared._news) && (_time == to_be_compared._time));
  
  
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
  int i;
  stream << p._iduser << ": < " << p._news << ">, like: {";
  if(p._likes.size()!=0) {
    for (i = 0; i < (p._likes.size() - 1); i++) {
      stream << p._likes[i] << ", ";
    }
    stream << p._likes[i] << "}, ";
  }
  else {stream<<"}, ";}
  
  stream<<"dislike: {";
  
  if(p._dislikes.size()!=0) {
    for (i = 0; i < (p._dislikes.size() - 1); i++) {
      stream << p._dislikes[i] << ", ";
    }
    stream << p._dislikes[i] << "} " << endl;
  }
  else{stream<<"}, "<<endl;}
  
  return stream;
  
}

//Others
int Post::SearchLike(const string &ID) const {
  
  for (int i = 0; i < _likes.size(); i++) {
    if (ID == _likes[i]) {
      return i;
    }
  }
  return -1;
  
}

int Post::SearchDislike(const string &ID) const {
  for (int i = 0; i < _dislikes.size(); i++) {
    if (ID == _dislikes[i]) {
      return i;
    }
  }
  return -1;
}

int Post::NumLikes() {
  return (int)_likes.size(); //ok, sta cosa è abbastanza inutile
}

int Post::NumDislikes(){
  return (int)_dislikes.size(); //idem con patate
}

float Post::RatioReaction(){
  if((NumDislikes()+NumLikes())==0){
    return 0;
  }
  else{
    return ((float)NumLikes()/(NumLikes()+NumDislikes()));
  }
}

void Post::AddLike(const string &id){
  _likes.push_back(id);
}

void Post::AddDislike(const string &id){
  _dislikes.push_back(id);
}

void Post::RemoveLike(const string &id){
  if (SearchLike(id)!=-1){
    _likes.erase(_likes.begin()+SearchLike(id));
  }
}

void Post::RemoveDislike(const string &id){
  if(SearchDislike(id)!=-1){
    _dislikes.erase(_dislikes.begin()+SearchDislike(id));
  }
}
