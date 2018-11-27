#include "readlines.h"

using string = std::string;

template <typename T>
using vector = std::vector<T>;


// specialization of template in order for an error to occur for lexical cast to complex<double>
template<>
ReadLines::lexical_cast::operator std::complex<double>() const;


ReadLines::lexical_cast ReadLines::operator[](int n) const{
  if(n >= nstrs_present()){
    printf("error in ReadLines: too large index for strings: %d\n",n);
    for(int i = 0; i < nstrs_present(); i++) printf("string %d: %s\n", i, vec_str_present[i].c_str());
    exit(1);
  }
  
  return lexical_cast(vec_str_present[n]);
}


void ReadLines::close(){
  lines.clear();
  vec_pushed_pos.clear();
}


bool ReadLines::operator++(){
  if(present < lines.size()){
    present++;
    get_strs_present();
    return true;
  }else{
    return false;
  }
}


bool ReadLines::operator++(int dummy){
  return ++(*this);
}


bool ReadLines::operator+=(int n){
  for(int i = 0; i < n; i++){
    if(!(++(*this))) return false;
  }
  return true;
}


bool ReadLines::operator--(){
  if(present > 0){
    present--;
    get_strs_present();
    return true;
  }else{
    return false;
  }
}


bool ReadLines::operator--(int dummy){
  return --(*this);
}


bool ReadLines::operator-=(int n){
  for(int i = 0; i < n; i++){
    if(!(--(*this))) return false;
  }
  return true;
}


int ReadLines::get_pos() const{
  return present;
}
 
 
//! Set the present position.
bool ReadLines::set_pos(int _present){
  if(_present < 0) return false;
  if(_present <= lines.size()){
    present = _present;
    get_strs_present();
    return true;
  }else{
    return false;
  }
}
 
 
//! Get the line at the cursor.
string ReadLines::line_here() const{
  auto const& line = lines[present];
  return present == lines.size() ? string(""): line.substr(line.find_first_not_of(" \t"));
}


//! Check whether the cursor is at the end of the series of lines.
bool ReadLines::is_end_reached() const{
  return present == lines.size();
}


//! Get strings in the present line.
vector<string> ReadLines::strs_here() const{
  return vec_str_present;
}


//! Get the number of strings contained in the present line
int ReadLines::nstrs_present() const{
  return vec_str_present.size();
}


//! Add _str to the group of strings which will be replaced by spaces.
void ReadLines::ignore_substr(string const& _str){
  vec_str_ignored.push_back(_str);
}


//! Add _str to the group of strings which will be replaced by spaces.
void ReadLines::ignore_substr(vector<string> const& _str){
  for(auto const& s: _str) ignore_substr(s);
}


//! Clear the substrings to be ignored.
void ReadLines::clear_ignored_substrs(){
  vec_str_ignored.clear();
}


//! Get the strings in the present line.
void ReadLines::get_strs_present(){
  vec_str_present.clear();
  string const line = line_here();
  vec_str_present = split(line);
}


void ReadLines::print() const{
  for(auto const& str: vec_str_present)
    printf("%s\n", str.c_str());
}

