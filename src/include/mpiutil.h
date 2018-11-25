// MPI utility class
// Taichi Kosugi
// since Oct 25, 2015

#ifndef MPIUTIL_DEFINED
#define MPIUTIL_DEFINED

#ifdef MPI_USED
#include "mpi.h"
#endif

#include <stdio.h>
#include <vector>
#include <string>
#include <complex>
#include <numeric>
#include <algorithm>

#include "strchar.h"
#include "esc_seq.h"


//! utility class for communication of data using MPI
class MPIUtil{
 private:
  template <typename T>
    using vector = std::vector<T>;

  using string = std::string;

  static int constexpr es_color_yellow = EscSeq::rgb(5, 5, 0);
  static int constexpr es_color_black = EscSeq::grayscale(0);

  static int const RANK_0 = 0;

#ifdef MPI_USED
  template <typename T> static MPI_Datatype const mpi_datatype;
#endif

 public:
#ifdef MPI_USED
  static bool const is_mpi_used = true;

  using communicator = MPI_Comm;
#else
  static bool const is_mpi_used = false;

  // dummy
  using communicator = int;
#endif

 private:
  static vector<EscSeq::property> const es_prop;

  static int get_myrank(communicator _comm);
  static int get_nprocs(communicator _comm);

  double const wtime_start;

 public:
  static int const not_found = -1;
  static int const all_procs = -1;

  static void init(int argc, char *argv[]);
  static communicator comm_world();

  communicator const comm;
  int const myrank, nprocs;

  explicit MPIUtil(communicator _comm);

  double wtime() const;
  double lapse() const;
  void barrier(string const& str = "") const;
  void finalize() const;
  void show_lapse(string const& str = "") const;
  void quit(string const& str = "") const;
  int find_among_ranks(vector<int> const& involved_rank) const;

  template <typename... T>
    void single_printf(T... arg) const{
    if(myrank == RANK_0) printf(arg...);
  }

  template <typename... T>
    void single_eprintf(std::vector<EscSeq::property> const& prop, char const *fmt, T... arg) const{
    if(myrank == RANK_0) EscSeq::eprintf(prop, fmt, arg...);
  }

  template <typename... T>
    void single_eprintf(int color_fg, int color_bg, char const *fmt, T... arg) const{
    if(myrank == RANK_0) EscSeq::eprintf(color_fg, color_bg, fmt, arg...);
  }

  template <typename T> void show_info_distr(string const& msg, T val) const;

  template <typename T> void let_all_know(T& dat, int rank_src) const;
  template <typename T> void let_all_know(vector<T>& dat, int rank_src) const;

  template <typename T> void let_other_know(vector<T>& dat_src, int rank_dst, vector<T>& dat_dst) const;

  template <typename T> void sum_from_all(T& dat, int rank_dst = all_procs) const;
  template <typename T> void sum_from_all(vector<T>& dat, int rank_dst = all_procs) const;

  template <typename T> void combine_from_all(vector<T>& dat, int rank_dst = all_procs) const;
  template <typename T> void redistr_according_to_vals(vector<T>& dat, int unit_size,
						       vector<int> const& val) const;
  template <typename T> void collect_to_procs(vector<T>& dat_send,
					      vector<int> const& target_rank, vector<T>& dat_recv) const; 

  template <typename T> void decompose(vector<T>& dat, int unit_size, int rank_src,
				       vector<int>& ndat_decomp) const;
};

#endif
