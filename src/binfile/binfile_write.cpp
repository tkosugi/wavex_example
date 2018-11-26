#include "binfile.h"

template <typename T> using vector = std::vector<T>;
using string = std::string;
using cmplx = std::complex<double>;


//! Write a datum to the file.
template <typename T>
void BinFile::write(T const& dat){
  fwrite(&dat, sizeof(T), 1, fp);
}

// specialization
template <>
void BinFile::write(bool const& dat){
  type_for_bool const tmp = dat ? 1: 0;
  fwrite(&tmp, sizeof(decltype(tmp)), 1, fp);
}

// specialization
template <>
void BinFile::write(string const& dat){
  type_for_nbytes const nbytes = sizeof(char)*(dat.length() + 1);
  fwrite(&nbytes, sizeof(decltype(nbytes)), 1, fp);
  fwrite(dat.c_str(), sizeof(char), dat.length() + 1, fp);
}

// explicit instantiation
template void BinFile::write(char const& dat);
template void BinFile::write(BYTE const& dat);
template void BinFile::write(int const& dat);
template void BinFile::write(double const& dat);
template void BinFile::write(cmplx const& dat);


//! Write data of the specified size to the file.
template <typename T>
void BinFile::write(vector<T> const& dat, int start, int size){
  unsigned int const size_to_write = (size == all_components) ? dat.size() - start: size;
  if(start + size_to_write > dat.size()){
    printf("error in BinFile: %d exceeds size %d of vector\n", start + size_to_write, static_cast<int>(dat.size()));
    exit(1);
  }

  type_for_nbytes const nbytes = size_to_write*sizeof(T);
  fwrite(&nbytes, sizeof(decltype(nbytes)), 1, fp);
  fwrite(dat.data() + start, sizeof(T), size_to_write, fp);
}

// specialization
template <>
void BinFile::write(vector<bool> const& dat, int start, int size){
  unsigned int const size_to_write = (size == all_components) ? dat.size() - start: size;
  if(start + size_to_write > dat.size()){
    printf("error in BinFile: %d exceeds size %d of vector\n", start + size_to_write, static_cast<int>(dat.size()));
    exit(1);
  }

  vector<type_for_bool> tmp(dat.size());
  for(int i = 0; i < dat.size(); i++) tmp[i] = dat[i] ? 1: 0;

  type_for_nbytes const nbytes = size_to_write*sizeof(type_for_bool);
  fwrite(&nbytes, sizeof(decltype(nbytes)), 1, fp);
  fwrite(tmp.data() + start, sizeof(type_for_bool), size_to_write, fp);
}

// specialization
template <>
void BinFile::write(vector<string> const& dat, int start, int size){
  int const nstrs_to_write = (size == all_components) ? dat.size() - start: size;
  if(start + nstrs_to_write > dat.size()){
    printf("error in BinFile: %d exceeds size %d of vector\n", start + nstrs_to_write, static_cast<int>(dat.size()));
    exit(1);
  }

  write(nstrs_to_write);
  for(int istr = 0; istr < nstrs_to_write; istr++) write(dat[start + istr]);
}

// explicit instantiation
template void BinFile::write(vector<char> const& dat, int start, int size);
template void BinFile::write(vector<BYTE> const& dat, int start, int size);
template void BinFile::write(vector<int> const& dat, int start, int size);
template void BinFile::write(vector<double> const& dat, int start, int size);
template void BinFile::write(vector<cmplx> const& dat, int start, int size);
