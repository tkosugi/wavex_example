// Taichi Kosugi
// since 2008

//
// example:
//
// #include <string>
// #include <complex>
// #include "readlines.h"
//
// int main()
// {
//   const std::string fname("file.txt");
//   ReadLines f;
//
//   // Open a file fname. Letters appearing after "#" in each line are regarded as a comment.
//   // All lines not blank are loaded until the end of the file.
//   f.load(fname,"#"); 
//
//   // After moving the cursor just below "hahaha", f reads int, double, std::complex<double>, and string values.
//   int val_i;
//   double val_d[2];
//   std::complex<double> val_z;
//   std::string val_s;
//   f.seek_from_top("hahaha");
//   f.set_pointer(&val_i);
//   f.set_pointer(val_d,2);
//   f.set_pointer(&val_z); // A value of std::complex<double> is regarded to be consecutive two double values.
//   f.set_pointer(&val_s);
//   f.get_values();
//
//   // readValues enables one to obtain an arbitrary number of variables.
//   f.readValues(&val_z,&val_i,&val_s,&val_d[0],&val_d[1]);
//
//   // Strings in the presnt line can undergo lexical cast via operator[] for substitutions of
//   int, double, and std::string variables.
//   int i = f[0];
//   double d = f[1];
//   std::string s = f[2];
//
//   // std::complex<double> is not allowed for lexical cast.
//   // std::complex<double> z = f[3]; // error!
//
//   // Comparison operators with lexical cast can also be used.
//   if(f[0] == 1) printf("f[0] == 1\n");
//   if(f[1] == 2.34) printf("f[1] == 2.34\n");
//   if(f[2] == "This is a string.") printf("f[2] == \"This is a string.\"\n");
//
//   // Cursor can be moved by using ++ or -- or += or -= operators.
//   f--;
//   f += 2;
//
//   f.close();
//   return 0;
// }

#ifndef READLINES_DEFINED
#define READLINES_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <complex>
#include <algorithm>

// gcc requires typeinfo
#ifdef __GNUC__
#include <typeinfo>
#endif


//! class  for reading a file with keywords
/*!
  This class is for reading values in a file with keywords.
  Blank lines are ignored.
  You can specify the letter for a comment line,
  after which all the letters in that line are ignored.
  Substrings in vec_str_ignored found in a line are regarded to be spaces.
  You can use this class only by including this file.
*/
class ReadLines{
 public:
  static bool const capital_small_not_distinguished = true;    
  static bool const capital_small_distinguished = false;    
  
 private:
  using string = std::string;

  template <typename T>
    using vector = std::vector<T>;

  class lexical_cast;

  // current line
  int present;

  // vector storing lines loaded
  vector<string> lines;

  // pointers and types of values to be read in get_values
  vector<void*> vec_ptr;

  // Strings in vec_str_ignored found in a string will be replaced by spaces.
  vector<string> vec_str_ignored;

  // strings in the present line
  vector<string> vec_str_present;

  // pushed positions
  vector<int> vec_pushed_pos;

  // copy constructor and substitution operator are defined as private members for prohibition of copy
  ReadLines(ReadLines const& crl_){}
  void operator=(ReadLines const& crl_){}

  vector<string> split(string const& _str);
  void adopt_line(vector<string> const& comment, string const& line);
  void get_strs_present();

 public:
  ReadLines(){}

  lexical_cast operator[](int n) const;

  bool operator++(); // prefix --
  bool operator++(int dummy); // postfix ++
  bool operator+=(int n);

  bool operator--(); // prefix --
  bool operator--(int dummy); // postfix --
  bool operator-=(int n);

  bool load(string const& fname, vector<string> const& comment);
  bool load(string const& fname, string const& comment);
  bool load(string const& fname);
  void read(vector<string> const& input_line, vector<string> const& comment);
  void read(vector<string> const& input_line, string const& comment);
  void close();
  bool set_pos(int _present); 
  bool seek_from_top(string str, bool noul = capital_small_distinguished);
  bool seek_from_here(string str, bool noul = capital_small_distinguished);
  bool seek_within(string str, int start, int end, bool noul);
  bool seek_str(string str, int ind, bool noul);
  void ignore_substr(vector<string> const& _str);
  void ignore_substr(string const& _str);
  void clear_ignored_substrs();

  int get_pos() const;
  bool is_end_reached() const;
  string line_here() const;
  vector<string> strs_here() const;
  int nstrs_present() const;
  void print() const;
};


//! for lexical cast of strings to values in appropriate types
class ReadLines::lexical_cast{
 private:
  string _str;
  
 public:
  lexical_cast(string const& _str_in): _str(_str_in){}
    
    template<typename T>
      operator T() const{
      std::stringstream ss;
      ss << _str;
      
      T result;
      ss >> result;
      
      return result;
    }
    
    // comparison operators for int
    bool operator==(int n){ return (int(*this) == n);}
    bool operator!=(int n){ return (int(*this) != n);}
    
    // comparison operators for double
    bool operator==(double d){ return(double(*this) == d);}
    bool operator!=(double d){ return(double(*this) != d);}
    
    // comparison operators for string
    bool operator==(string const& _str_in){ return (_str == _str_in);}
    bool operator!=(string const& _str_in){ return (_str != _str_in);}
    
    // comparison operators for *char
    bool operator==(char const *ch){ return (_str == string(ch));}
    bool operator!=(char const *ch){ return (_str != string(ch));}
    
    string str(){ return _str;}
};

#endif
