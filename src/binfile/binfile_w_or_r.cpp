#include "binfile.h"

template <typename T> using vector = std::vector<T>;
using string = std::string;
using cmplx = std::complex<double>;


template <typename T>
void BinFile::write_or_read(T& dat){
  if(mode == open_mode::write){
    write(dat);
  }else if(mode == open_mode::read){
    read(dat);
  }
}

// explicit instantiation
template void BinFile::write_or_read(char& dat);
template void BinFile::write_or_read(BYTE& dat);
template void BinFile::write_or_read(int& dat);
template void BinFile::write_or_read(double& dat);
template void BinFile::write_or_read(cmplx& dat);
template void BinFile::write_or_read(bool& dat);
template void BinFile::write_or_read(string& dat);


template <typename T>
void BinFile::write_or_read(vector<T>& dat){
  if(mode == open_mode::write){
    write(dat);
  }else if(mode == open_mode::read){
    read(dat);
  }
}

// explicit instantiation
template void BinFile::write_or_read(vector<char>& dat);
template void BinFile::write_or_read(vector<BYTE>& dat);
template void BinFile::write_or_read(vector<int>& dat);
template void BinFile::write_or_read(vector<double>& dat);
template void BinFile::write_or_read(vector<cmplx>& dat);
template void BinFile::write_or_read(vector<bool>& dat);
template void BinFile::write_or_read(vector<string>& dat);
