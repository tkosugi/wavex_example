#include "mpiutil.h"

template <typename T>
using vector = std::vector<T>;

using cmplx = std::complex<double>;


//! Sum up data in all the processors.
template <typename T>
void MPIUtil::sum_from_all(T& dat, int rank_dst) const{
#ifdef MPI_USED
  vector<T> tmp(1);
  tmp[0] = dat;
  sum_from_all(tmp, rank_dst);
  dat = tmp[0];
#endif
}

// explicit instantiation
template void MPIUtil::sum_from_all(int& dat, int rank_dst) const;
template void MPIUtil::sum_from_all(double& dat, int rank_dst) const;
template void MPIUtil::sum_from_all(cmplx& dat, int rank_dst) const;


//! Sum up data in all the processors.
template <typename T>
void MPIUtil::sum_from_all(vector<T>& dat, int rank_dst) const{
#ifdef MPI_USED
  vector<T> dat_send(dat);
  if(rank_dst == all_procs)
    MPI_Allreduce(dat_send.data(), dat.data(), dat.size(), mpi_datatype<T>, MPI_SUM, comm);
  else
    MPI_Reduce(dat_send.data(), dat.data(), dat.size(), mpi_datatype<T>, MPI_SUM, rank_dst, comm);
#endif
}

// explicit instantiation
template void MPIUtil::sum_from_all(vector<int>& dat, int rank_dst) const;
template void MPIUtil::sum_from_all(vector<double>& dat, int rank_dst) const;
template void MPIUtil::sum_from_all(vector<cmplx>& dat, int rank_dst) const;
