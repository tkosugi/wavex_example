#include "binfile.h"

template <typename T> using vector = std::vector<T>;
using string = std::string;
using cmplx = std::complex<double>;


BinFile::BinFile(string const& _fname, open_mode _mode): fname(_fname), mode(_mode){
  fp = fopen(fname.c_str(), (mode == open_mode::write) ? "wb": "rb");

  if(fp == nullptr){
    printf("cannot open %s\n", fname.c_str());
    exit(1);
  }

  is_open = (fp != nullptr);
}


BinFile::~BinFile(){
  close();
}


bool BinFile::is_available(){
  return is_open;
}


//! Close the file if it exists.
void BinFile::close(){
  if(is_open) fclose(fp);
  is_open = false;
}
