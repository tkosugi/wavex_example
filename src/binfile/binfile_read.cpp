#include "binfile.h"

template <typename T> using vector = std::vector<T>;
using string = std::string;
using cmplx = std::complex<double>;


//! Read a value from the file.
template <typename T>
void BinFile::read(T& dat){
  fread(&dat, sizeof(T), 1, fp);
}

// specialization
template <>
void BinFile::read(bool& dat){
  type_for_bool tmp;
  fread(&tmp, sizeof(decltype(tmp)), 1, fp);
  dat = (tmp == 1) ? true: false;
}

// specialization
template <>
void BinFile::read(string& dat){
  type_for_nbytes nbytes;
  fread(&nbytes, sizeof(decltype(nbytes)), 1, fp);

  auto const length = nbytes/sizeof(char) - 1;
  vector<char> tmp(length + 1);
  fread(tmp.data(), sizeof(char), length + 1, fp);

  dat = string(tmp.data());
}

// explicit instantiation
template void BinFile::read(char& dat);
template void BinFile::read(BYTE& dat);
template void BinFile::read(int& dat);
template void BinFile::read(double& dat);
template void BinFile::read(cmplx& dat);


//! Read an array from the file.
template <typename T>
void BinFile::read(vector<T>& dat){
  type_for_nbytes nbytes;
  fread(&nbytes, sizeof(decltype(nbytes)), 1, fp);

  auto const size = nbytes/sizeof(T);
  dat.resize(size);

  fread(dat.data(), sizeof(T), size, fp);
}

// specialization
template <>
void BinFile::read(vector<bool>& dat){
  type_for_nbytes nbytes;
  fread(&nbytes, sizeof(decltype(nbytes)), 1, fp);

  auto const size = nbytes/sizeof(type_for_bool);
  vector<type_for_bool> tmp(size);
  fread(tmp.data(), sizeof(type_for_bool), size, fp);

  dat.resize(size);
  for(int i = 0; i < dat.size(); i++) dat[i] = (tmp[i] == 1) ? true: false;
}

// specialization
template <>
void BinFile::read(vector<string>& dat){
  int nstrs_to_read;
  fread(&nstrs_to_read, sizeof(decltype(nstrs_to_read)), 1, fp);

  dat.clear();
  dat.resize(nstrs_to_read);

  for(int istr = 0; istr < nstrs_to_read; istr++) read(dat[istr]);
}

// explicit instantiation
template void BinFile::read(vector<char>& dat);
template void BinFile::read(vector<BYTE>& dat);
template void BinFile::read(vector<int>& dat);
template void BinFile::read(vector<double>& dat);
template void BinFile::read(vector<cmplx>& dat);
