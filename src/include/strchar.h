// Taichi Kosugi
// since Sep 10, 2015

#ifndef STRSTOCHARS_DEFINED
#define STRSTOCHARS_DEFINED

#include <iostream>
#include <string.h>
#include <string>
#include <vector>


//! class for transformation between strings and characters
class StrsToChars{
 public:
  static void read_stdin(std::vector<std::string>& input_line);
  static void combine(std::vector<std::string> const& vec_str, std::vector<char>& chrs);
  static void decompose(std::vector<char> const& chrs, std::vector<std::string>& vec_str);

  StrsToChars() = delete;
};

#endif
