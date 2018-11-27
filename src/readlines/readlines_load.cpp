#include "readlines.h"

using string = std::string;

template <typename T>
using vector = std::vector<T>;


//! Open a file "fname" with a comment string.
/*!
  Each line will be loaded and stored if it is not blank.
*/
bool ReadLines::load(string const& fname, vector<string> const& comment){
  std::ifstream ifs;
  ifs.open(fname.c_str());
  if(ifs.fail()) return false;
  
  while(!ifs.eof()){
    string oneline;
    getline(ifs, oneline);
    adopt_line(comment, oneline);
  }
  
  ifs.close();
  set_pos(0); 
  return true;
}


bool ReadLines::load(string const& fname, string const& comment){
  return load(fname, vector<string> {comment});
}


bool ReadLines::load(string const& fname){
  return load(fname, vector<string> {});
}
