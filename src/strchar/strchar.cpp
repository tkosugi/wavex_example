#include "strchar.h"

using string = std::string;

template <typename T>
using vector = std::vector<T>;


//! Read line-by-line standard input and store them.
void StrsToChars::read_stdin(vector<string>& input_line){
  input_line.clear();
  while(!std::cin.fail()){
    string line;
    std::getline(std::cin, line);
    input_line.push_back(line);
  }
}


//! Combine strings to a single char[].
void StrsToChars::combine(vector<string> const& vec_str, vector<char>& chrs){
  chrs.clear();
  for(auto const& str: vec_str){
    for(auto c: str) chrs.push_back(c);
    chrs.push_back(0);
  }
}


//! Decompose a single char[] to strings.
void StrsToChars::decompose(vector<char> const& chrs, vector<string>& vec_str){
  vec_str.clear();
  int cnt_chr = 0;
  while(cnt_chr < chrs.size()){
    char const *cstr = chrs.data() + cnt_chr;
    vec_str.push_back(cstr);
    cnt_chr += strlen(cstr) + 1;
  }
}
