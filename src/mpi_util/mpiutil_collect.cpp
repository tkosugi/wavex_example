#include "mpiutil.h"

template <typename T>
using vector = std::vector<T>;

using cmplx = std::complex<double>;


//! Collect the distributed data to the specified fewer processors.
template <typename T>
void MPIUtil::collect_to_procs(vector<T>& dat_send,
			       vector<int> const& target_rank,
			       vector<T>& dat_recv) const{
#ifdef MPI_USED
  vector<int> involved_rank;
  std::iota(involved_rank.begin(), involved_rank.end(), RANK_0);

  int const ind_rank_involved = myrank;
  int const ind_rank_target = find_among_ranks(target_rank);

  if(target_rank.size() > involved_rank.size()){
    single_eprintf(es_prop, "error in MPIUtil::collect_to_procs: invalid ranks\n");
    exit(1);
  }

  // Find target ranks in involved ranks.
  int const nf = not_found;
  vector<int> ind_target(target_rank.size(), nf);
  for(int itgr = 0; itgr < target_rank.size(); itgr++){
    for(int iivr = 0; iivr < involved_rank.size(); iivr++){
      if(target_rank[itgr] == involved_rank[iivr]) ind_target[itgr] = iivr;
    }
  }

  if(std::find(ind_target.cbegin(), ind_target.cend(), nf) != ind_target.cend()){
    single_eprintf(es_prop, "error in MPIUtil::collect_to_procs: invalid target ranks\n");
    exit(1);
  }

  // size of data in each processor
  vector<int> dat_size(involved_rank.size(), 0);
  dat_size[ind_rank_involved] = dat_send.size();
  sum_from_all(dat_size);

  int const mytarget = target_rank[ind_rank_involved%target_rank.size()];

  if(ind_rank_target != not_found){
    // size of data this processor will receive
    int size_recv_tot = 0;
    for(int iivr = 0; iivr < involved_rank.size(); iivr++){
      if(iivr%target_rank.size() == ind_rank_target) size_recv_tot += dat_size[iivr];
    }
    dat_recv.resize(size_recv_tot);

    // This processor send and receive data.
    
    MPI_Request req_send;
    MPI_Isend(dat_send.data(), dat_send.size(), mpi_datatype<T>, mytarget, 0, comm, &req_send);
    
    vector<MPI_Request> req_recv;
    int offs = 0;
    for(int iivr = 0; iivr < involved_rank.size(); iivr++){
      if(iivr%target_rank.size() == ind_rank_target){
	int const size_recv = dat_size[iivr];
	req_recv.push_back(MPI_Request());
	MPI_Irecv(dat_recv.data() + offs, size_recv, mpi_datatype<T>, involved_rank[iivr], 0, comm, &req_recv.back());
	offs += size_recv;
      }
    }   

    MPI_Status stat;
    for(auto& req: req_recv) MPI_Wait(&req, &stat);
    MPI_Wait(&req_send, &stat);
  }else{
    // This processor only send data.
    MPI_Send(dat_send.data(), dat_send.size(), mpi_datatype<T>, mytarget, 0, comm);
  } 

#else
  dat_recv = dat_send;
#endif
}

// explicit instantiation
template void MPIUtil::collect_to_procs(vector<int>& dat_send,
					vector<int> const& target_rank, vector<int>& dat_recv) const;
template void MPIUtil::collect_to_procs(vector<double>& dat_send,
					vector<int> const& target_rank, vector<double>& dat_recv) const;
template void MPIUtil::collect_to_procs(vector<cmplx>& dat_send,
					vector<int> const& target_rank, vector<cmplx>& dat_recv) const;
