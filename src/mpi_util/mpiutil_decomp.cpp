#include "mpiutil.h"

template <typename T>
using vector = std::vector<T>;

using cmplx = std::complex<double>;


//! Decompose data stored in rank_src to all the ranks.
/*!
  The original data will be stored in ascending order wrt the ranks.
*/
template <typename T> 
void MPIUtil::decompose(vector<T>& dat,
			int unit_size,
			int rank_src,
			vector<int>& ndat_decomp) const{
#ifdef MPI_USED
  vector<int> involved_rank;
  std::iota(involved_rank.begin(), involved_rank.end(), RANK_0);

  vector<int> val;

  if(myrank == rank_src){
    int const ndat = dat.size()/unit_size;
    val.resize(ndat);

    int ndat_per_rank = ndat/nprocs;
    while(ndat_per_rank*nprocs < ndat) ndat_per_rank++;

    for(int i = 0; i < ndat; i++) val[i] = i/ndat_per_rank;

    // only for debug
    for(int i = 0; i < ndat; i++){
      if(val[i] >= nprocs){
	single_eprintf(es_prop, "invalid val in decompose\n");
	exit(1);
      }
    }
  }else{
    dat.clear();
  }

  redistr_according_to_vals(dat, unit_size, val);

  // numbers of data in processors
  ndat_decomp.resize(nprocs, 0);
  ndat_decomp[find_among_ranks(involved_rank)] = dat.size()/unit_size;
  sum_from_all(ndat_decomp);
#else
  ndat_decomp.resize(nprocs, dat.size()/unit_size);
#endif
}

// explicit instantiation
template void MPIUtil::decompose(vector<int>& dat, int unit_size, int rank_src,
				 vector<int>& ndat_decomp) const;
template void MPIUtil::decompose(vector<double>& dat, int unit_size, int rank_src,
				 vector<int>& ndat_decomp) const;
template void MPIUtil::decompose(vector<cmplx>& dat, int unit_size, int rank_src,
				 vector<int>& ndat_decomp) const;
