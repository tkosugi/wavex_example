#include "readlines.h"

using string = std::string;

template <typename T>
using vector = std::vector<T>;


//! Split the string str according to blanks and store the pieces into vector.
vector<string> ReadLines::split(string const& _str){
  string str = _str;
  string::size_type index;

  // Replace ignored letters by spaces.
  for(auto const& str_ign: vec_str_ignored){
    index = 0;
    while(index = str.find(str_ign, index), index != str.npos){
      str.replace(index, 1, " ");
      index++;
    }
  }
  
  vector<string> result;
  index = 0;
  int start = 0;
  while((index = str.find_first_of(" ", start)) != str.npos){
    if(index > 0){
      string ss = str.substr(start, index - start);    
      // stored if not blank
      if(ss.find_first_not_of(" ") != ss.npos) result.push_back(ss);
    }
    start = index + 1;
  }
  
  if(str.length() > 0){
    string ss = str.substr(start, index - start);
    // stored if not blank
    if(ss.find_first_not_of(" ") != ss.npos) result.push_back(ss);
  }
  
  return result;
}	
