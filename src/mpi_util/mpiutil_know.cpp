#include "mpiutil.h"

template <typename T>
using vector = std::vector<T>;

using cmplx = std::complex<double>;


//! Broadcast the data in rank_src to all the other ranks.
template <typename T>
void MPIUtil::let_all_know(T& dat, int rank_src) const{
  vector<T> tmp;
  tmp.push_back(dat);
  let_all_know(tmp, rank_src);
  dat = tmp[0];
}

// explicit instantiation
template void MPIUtil::let_all_know(char& dat, int rank_src) const;
template void MPIUtil::let_all_know(bool& dat, int rank_src) const;
template void MPIUtil::let_all_know(int& dat, int rank_src) const;
template void MPIUtil::let_all_know(double& dat, int rank_src) const;
template void MPIUtil::let_all_know(cmplx& dat, int rank_src) const;
template void MPIUtil::let_all_know(std::string& dat, int rank_src) const;


//! Broadcast the data in rank_src to all the other ranks.
template <typename T>
void MPIUtil::let_all_know(vector<T>& dat, int rank_src) const{
#ifdef MPI_USED
  int dat_size = dat.size();
  MPI_Bcast(&dat_size, 1, MPI_INTEGER, rank_src, comm);
  if(myrank != rank_src){
    dat.clear();
    dat.resize(dat_size);
  }
  MPI_Bcast(dat.data(), dat_size, mpi_datatype<T>, rank_src, comm);
#endif
}

// specialization for bool
template <>
void MPIUtil::let_all_know(vector<bool>& dat, int rank_src) const{
#ifdef MPI_USED
  vector<char> bin(dat.size());
  for(int i = 0; i < dat.size(); i++) bin[i] = dat[i] ? 1: 0;
  let_all_know(bin, rank_src);
  for(int i = 0; i < dat.size(); i++) dat[i] = (bin[i] == 1);
#endif
}

// specialization for string
template <>
void MPIUtil::let_all_know(vector<std::string>& strs, int rank_src) const{
#ifdef MPI_USED
  // string[] -> char[]
  vector<char> chrs;
  StrsToChars::combine(strs, chrs);

  let_all_know(chrs, rank_src);

  // char[] -> string[]
  StrsToChars::decompose(chrs, strs);
#endif
}

// explicit instantiation
template void MPIUtil::let_all_know(vector<char>& dat, int rank_src) const;
template void MPIUtil::let_all_know(vector<int>& dat, int rank_src) const;
template void MPIUtil::let_all_know(vector<double>& dat, int rank_src) const;
template void MPIUtil::let_all_know(vector<cmplx>& dat, int rank_src) const;


//! Send the data in the rank 0 to the specified rank.
template <typename T>
void MPIUtil::let_other_know(vector<T>& dat_src, int rank_dst, vector<T>& dat_dst) const{
#ifdef MPI_USED
  if(myrank != RANK_0 && myrank != rank_dst) return;

  if(rank_dst == RANK_0){
    dat_dst.clear();
    dat_dst.resize(dat_src.size());
    for(int i = 0; i < dat_src.size(); i++) dat_dst[i] = dat_src[i];
    return;
  }

  MPI_Status stat;
  if(myrank == RANK_0){
    int dat_src_size = dat_src.size();
    MPI_Send(&dat_src_size, 1, MPI_INTEGER, rank_dst, 0, comm);
    MPI_Send(dat_src.data(), dat_src_size, mpi_datatype<T>, rank_dst, 0, comm);
  }else if(myrank == rank_dst){
    int dat_dst_size;
    MPI_Recv(&dat_dst_size, 1, MPI_INTEGER, RANK_0, 0, comm, &stat);
    dat_dst.clear();
    dat_dst.resize(dat_dst_size);
    MPI_Recv(dat_dst.data(), dat_dst_size, mpi_datatype<T>, RANK_0, 0, comm, &stat);
  }
#else
  dat_dst.clear();
  dat_dst.resize(dat_src.size());
  for(int i = 0; i < dat_src.size(); i++) dat_dst[i] = dat_src[i];
#endif
}

// explicit instantiation
template void MPIUtil::let_other_know(vector<char>& dat_src, int rank_dst, vector<char>& dat_dst) const;
template void MPIUtil::let_other_know(vector<int>& dat_src, int rank_dst, vector<int>& dat_dst) const;
template void MPIUtil::let_other_know(vector<double>& dat_src, int rank_dst, vector<double>& dat_dst) const;
template void MPIUtil::let_other_know(vector<cmplx>& dat_src, int rank_dst, vector<cmplx>& dat_dst) const;

