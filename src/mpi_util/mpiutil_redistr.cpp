#include "mpiutil.h"


template <typename T>
using vector = std::vector<T>;

using cmplx = std::complex<double>;


//! Redistribute tha data in processors according to the input integers.
/*!
  The data with the specified size will be treated as a unit with which an integer is associated.
  The data after the redistribution will be in ascending order
  wrt the indices of source processors.
*/
template <typename T> 
void MPIUtil::redistr_according_to_vals(vector<T>& dat,
					int unit_size,
					vector<int> const& val) const{
#ifdef MPI_USED
  if(dat.size()%unit_size != 0){
    single_eprintf(es_prop, "error in MPIUtil::redistr_according_to_vals: unit size %d for data size %d\n",
		   unit_size, static_cast<int>(dat.size()));
    exit(1);
  }

  if(val.size() != dat.size()/unit_size){
    single_eprintf(es_prop, "error in MPIUtil::redistr_according_to_vals: size of values %d\n",
		   static_cast<int>(val.size()));
    exit(1);
  }

  if(nprocs == 1) return;

  // Classify data according to val%nprocs.
  vector<int> remainder(val);
  for(int& r: remainder) r = r%nprocs;

  // Count the numbers of remainders in this processor.
  vector<int> size_for_rem_this_proc(nprocs, 0);
  for(int const r: remainder) size_for_rem_this_proc[r]++;

  // the numbers of remainders in all the processors
  vector<int> size_for_rem_all(size_for_rem_this_proc);
  sum_from_all(size_for_rem_all);

  // array for storing received data from all the processors
  vector<T> dat_recv(size_for_rem_all[myrank]*unit_size);

  // offsets for receiving data
  vector<int> offs_recv(nprocs, 0);
  for(int irank = 0; irank < nprocs; irank++){
    if(irank > myrank) offs_recv[irank] = size_for_rem_this_proc[irank]*unit_size;
  }
  sum_from_all(offs_recv);

  int dest_recv = offs_recv[myrank];
  int size_received = 0;
  int cnt = 0;
  while(cnt < nprocs){
    // Send and receive data to processors distant by cnt from this processor.

    int const rank_for_send = (myrank - cnt + nprocs)%nprocs;
    int const rank_for_recv = (myrank + cnt)%nprocs;

    // Prepare data to send.
    vector<T> dat_send(size_for_rem_this_proc[rank_for_send]*unit_size);
    int cnt_rem = 0;
    for(int ival = 0; ival < val.size(); ival++){
      if(remainder[ival] == rank_for_send){
	for(int j = 0; j < unit_size; j++) dat_send[cnt_rem*unit_size + j] = dat[ival*unit_size + j];
	cnt_rem++;
      }
    }

    int size_to_send = dat_send.size();
    if(cnt == 0){
      for(int i = 0; i < size_to_send; i++) dat_recv[dest_recv + i] = dat_send[i];
      dest_recv += size_to_send;
    }else{
      int size_to_recv;
      MPI_Status stat;
      MPI_Sendrecv(&size_to_send, 1, MPI_INTEGER, rank_for_send, 0,
		   &size_to_recv, 1, MPI_INTEGER, rank_for_recv, 0,
		   comm, &stat);

      MPI_Sendrecv(dat_send.data(), size_to_send, mpi_datatype<T>, rank_for_send, 0,
                   dat_recv.data() + dest_recv, size_to_recv, mpi_datatype<T>, rank_for_recv, 0,
                   comm, &stat);

      dest_recv += size_to_recv;
    }
    if(dest_recv >= dat_recv.size()) dest_recv = 0;

    cnt++;
  } // end while

  dat = dat_recv;
#endif
}

// explicit instantiation
template void MPIUtil::redistr_according_to_vals(vector<int>& dat, int unit_size,
						 vector<int> const& val) const;
template void MPIUtil::redistr_according_to_vals(vector<double>& dat, int unit_size,
						 vector<int> const& val) const;
template void MPIUtil::redistr_according_to_vals(vector<cmplx>& dat, int unit_size,
						 vector<int> const& val) const;
