#include "readlines.h"

using string = std::string;

template <typename T>
using vector = std::vector<T>;


//! Read lines with a comment string.
void ReadLines::read(vector<string> const& input_line, vector<string> const& comment){
  for(auto const& line: input_line) adopt_line(comment, line);
  set_pos(0);
}


void ReadLines::read(vector<string> const& input_line, string const& comment){
  read(input_line, vector<string> {comment});
}


void ReadLines::adopt_line(vector<string> const& comment, string const& line){
  auto oneline = line;

  for(auto const& cm: comment){
    auto const cpos = oneline.find(cm);
    if(cpos != string::npos) oneline = oneline.substr(0, cpos);
  }

  int lstart = oneline.find_first_not_of(" \t");
  if(lstart != -1){
    oneline = oneline.substr(lstart);
    if(oneline != "") lines.push_back(" " + oneline + " ");
  }
}
