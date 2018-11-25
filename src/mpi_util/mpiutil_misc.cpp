#include "mpiutil.h"

using cmplx = std::complex<double>;
using string = std::string;


#ifdef MPI_USED
template <>
MPI_Datatype const MPIUtil::mpi_datatype<char> = MPI_CHARACTER;

template <>
MPI_Datatype const MPIUtil::mpi_datatype<int> = MPI_INTEGER;

template <>
MPI_Datatype const MPIUtil::mpi_datatype<double> = MPI_DOUBLE;

template <>
MPI_Datatype const MPIUtil::mpi_datatype<cmplx> = MPI_DOUBLE_COMPLEX;
#endif


std::vector<EscSeq::property> const MPIUtil::es_prop {EscSeq::property::bold, EscSeq::property::fg_magenta};


void MPIUtil::init(int argc, char *argv[]){
#ifdef MPI_USED
  MPI_Init(&argc, &argv);
#endif
}


MPIUtil::communicator MPIUtil::comm_world(){
#ifdef MPI_USED
  return MPI_COMM_WORLD;
#else
  return 0;
#endif
}


#ifdef MPI_USED
//! constructor
MPIUtil::MPIUtil(communicator _comm): comm(_comm),
				      wtime_start(MPI_Wtime()),
				      myrank(get_myrank(_comm)),
				      nprocs(get_nprocs(_comm))
{
}
#else
MPIUtil::MPIUtil(communicator _comm): comm(_comm),
				      wtime_start(0.0),
				      myrank(get_myrank(_comm)),
				      nprocs(get_nprocs(_comm))
{
}
#endif


//! rank of this processor
int MPIUtil::get_myrank(communicator _comm){
  int myrank;
#ifdef MPI_USED
  MPI_Comm_rank(_comm, &myrank);
#else
  myrank = RANK_0;
#endif
  return myrank;
}


//! number of processors
int MPIUtil::get_nprocs(communicator _comm){
  int nprocs;
#ifdef MPI_USED
  MPI_Comm_size(_comm, &nprocs);
#else
  nprocs = 1;
#endif
  return nprocs;
}


//! MPI wall time
double MPIUtil::wtime() const{
#ifdef MPI_USED
  return MPI_Wtime();
#else
  return 0.0;
#endif
}


//! lapse since construction
double MPIUtil::lapse() const{
#ifdef MPI_USED
  return MPI_Wtime() - wtime_start;
#else
  return 0.0;
#endif
}


//! MPI barrier
void MPIUtil::barrier(string const& str) const{
#ifdef MPI_USED
  auto show_where = [&](string const b_or_a){
    if(str != "")
      single_eprintf(es_prop, "proc %d: just %s MPI_Barrier for %s\n", myrank, b_or_a.c_str(), str.c_str());
  };

  show_where("before");
  MPI_Barrier(comm);
  show_where("after");
#endif
}


//! MPI finalize
void MPIUtil::finalize() const{
#ifdef MPI_USED
  MPI_Finalize();
  single_eprintf(es_color_yellow, es_color_black, "MPI finalized\n");
#endif
}


void MPIUtil::show_lapse(string const& str) const{
  single_eprintf(es_color_yellow, es_color_black, "lapse %f sec %s\n\n", lapse(), str.c_str());
}


//! Finalize MPI and quit.
void MPIUtil::quit(string const& str) const{
  single_eprintf(es_prop, "%s\n", str.c_str());
  finalize();
  exit(1);
}


//! Get the index of this processor among the involved processors.
int MPIUtil::find_among_ranks(vector<int> const& involved_rank) const{
  for(int irank = 0; irank < involved_rank.size(); irank++) if(involved_rank[irank] == myrank) return irank;
  return not_found;
}


//! Show information of distributed data.
template <typename T>
void MPIUtil::show_info_distr(string const& msg, T val) const{
  vector<T> dat(nprocs, T(0));
  dat[myrank] = val;
  sum_from_all(dat);

  T const sum = std::accumulate(dat.begin(), dat.end(), T(0));
  single_printf("%s (min %d, max %d, sum %d, avr %d)\n",
		msg.c_str(),
		*std::min_element(dat.begin(), dat.end()),
		*std::max_element(dat.begin(), dat.end()),
		sum, sum/nprocs);
}

// explicit instantiation
template void MPIUtil::show_info_distr(string const& msg, int val) const;
