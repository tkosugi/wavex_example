#include "binfile.h"

template <typename T> using vector = std::vector<T>;
using string = std::string;
using cmplx = std::complex<double>;



//! Skip reading a value from the file.
template <typename T>
void BinFile::skip_read(T const& dat){
  fseek(fp, sizeof(T), SEEK_CUR);
}

// specialization
template <>
void BinFile::skip_read(bool const& dat){
  fseek(fp, sizeof(type_for_bool), SEEK_CUR);
}

// specialization
template <>
void BinFile::skip_read(string const& dat){
  vector<char> const dummy;
  skip_read(dummy);
}

// explicit instantiation
template void BinFile::skip_read(char const& dat);
template void BinFile::skip_read(BYTE const& dat);
template void BinFile::skip_read(int const& dat);
template void BinFile::skip_read(double const& dat);
template void BinFile::skip_read(cmplx const& dat);


//! Skip reading an array from the file.
template <typename T>
void BinFile::skip_read(vector<T> const& dat){
  type_for_nbytes nbytes;
  fread(&nbytes, sizeof(decltype(nbytes)), 1, fp);
  fseek(fp, nbytes, SEEK_CUR);
}

// explicit instantiation
template void BinFile::skip_read(vector<char> const& dat);
template void BinFile::skip_read(vector<BYTE> const& dat);
template void BinFile::skip_read(vector<int> const& dat);
template void BinFile::skip_read(vector<double> const& dat);
template void BinFile::skip_read(vector<cmplx> const& dat);
