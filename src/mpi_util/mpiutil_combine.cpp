#include "mpiutil.h"

template <typename T>
using vector = std::vector<T>;

using cmplx = std::complex<double>;


//! Combine all the distributed data and store it into the specified processor.
template <typename T>
void MPIUtil::combine_from_all(vector<T>& dat, int rank_dst) const{
#ifdef MPI_USED
  // size of data in each processor
  vector<int> size_prev(nprocs, 0);
  size_prev[myrank] = dat.size();
  sum_from_all(size_prev);

  // size of combined data
  int const size_combined = std::accumulate(size_prev.cbegin(), size_prev.cend(), 0);

  if(rank_dst == all_procs){
    vector<T> prev(dat);

    dat.clear();
    dat.resize(size_combined, 0);

    int offs = 0;
    for(int iproc = RANK_0; iproc < myrank; iproc++) offs += size_prev[iproc];
    for(int i = 0; i < size_prev[myrank]; i++) dat[offs + i] = prev[i];

    sum_from_all(dat);
  }else{
    single_eprintf(es_prop, "NIY in combine_from_all\n");
    exit(1);
  }
#endif
}

// explicit instantiation
template void MPIUtil::combine_from_all(vector<int>& dat, int rank_dst) const;
template void MPIUtil::combine_from_all(vector<double>& dat, int rank_dst) const;
template void MPIUtil::combine_from_all(vector<cmplx>& dat, int rank_dst) const;
