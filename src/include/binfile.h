// for I/O of binary files
// Taichi Kosugi
// since Jan. 19, 2016

#ifndef BINFILE_DEFINED
#define BINFILE_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <complex>


//! class for input and output of binary files
class BinFile{
 private:
  template <typename T>
    using vector = std::vector<T>;

 public:
  using BYTE = unsigned char;
  using type_for_bool = BYTE;
  using type_for_nbytes = unsigned int;

  static int const all_components = -1;

  enum class open_mode{
    write,
      read
      };

 public:
  BinFile(std::string const& _fname, open_mode _mode);
  ~BinFile();

  bool is_available();
  void close();

  template <typename T> void write(T const& dat);
  template <typename T> void write(vector<T> const& dat, int start = 0, int size = all_components);

  template <typename T> void read(T& dat);
  template <typename T> void read(vector<T>& dat);

  template <typename T> void skip_read(T const& dat);
  template <typename T> void skip_read(vector<T> const& dat);
  
  template <typename T> void write_or_read(T& dat);
  template <typename T> void write_or_read(vector<T>& dat);

 private:
  std::string const fname;
  open_mode const mode;

  FILE *fp;
  bool is_open;
};

#endif
