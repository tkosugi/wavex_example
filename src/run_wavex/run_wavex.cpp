// Taichi Kosugi
// Nov 23, 2018

#include <stdio.h>
#include <array>
#include <vector>

#include "mpiutil.h"
#include "omputil.h"
#include "binfile.h"
#include "readlines.h"
#include "wavex.h"

int const ndirs = 3;
int const spin_up = 0;
int const spin_dn = 1;

std::vector<double> gen_k_points(std::array<int, ndirs> const& k_div);
void read_occ_eval(std::string const& fname_in, std::vector<double>& eval, std::vector<double>& occ);


int main(int argc, char *argv[]){
  int const RANK_0 = 0;

  std::string const prefix("test");
  std::array<int, ndirs> const k_div {6, 1, 1};

  auto const k_point = gen_k_points(k_div);
  int const nkpts = k_point.size()/ndirs;

  MPIUtil::init(argc, argv);
  MPIUtil mpi_util(MPIUtil::comm_world());
  int const nprocs = mpi_util.nprocs;

  EscSeq::is_esc_seq_used = false;

  mpi_util.single_printf("sample program of calling WaveX for CCSD and GF of a Be chain\n");
  mpi_util.single_printf("%d OpenMP threads in each of %d MPI processes\n\n", OMPUtil::get_max_threads(), nprocs);

  if(nprocs != 6){
    mpi_util.single_printf("This sample program assumes 6 MPI processes.\n");
    mpi_util.finalize();
    return 0;
  }

  mpi_util.single_printf("=========================== calling WaveX ===========================\n");
  mpi_util.single_printf("distribution pattern for two-electron integrals < k0 k1 | k2 k3 >\n");

  std::array<int, ndirs> nranks_k;
  for(int iproc = RANK_0; iproc < nprocs; iproc++){
    std::array<int, ndirs> start_k, end_k;
    wavex_det_distr_(&iproc, &nprocs, &nkpts,
		     nranks_k.data(), start_k.data(), end_k.data());

    if(iproc == RANK_0){
      mpi_util.single_printf("%d procs for kp (k0), %d procs for kq (k1), %d procs for kr (k2)\n",
			     nranks_k[0], nranks_k[1], nranks_k[2]);
    }
    mpi_util.single_printf("proc %d: kp[%d...%d] kq[%d...%d] kr[%d...%d]\n", iproc,
			   start_k[0] - 1, end_k[0] - 1,
			   start_k[1] - 1, end_k[1] - 1,
			   start_k[2] - 1, end_k[2] - 1); 
  }
  mpi_util.single_printf("\n");

  WaveXParams wx;

  {
    std::vector<std::vector<double>> occ_scf(2);
    read_occ_eval(prefix + "_eval_occ_up.txt", wx.eval[spin_up], occ_scf[spin_up]);
    read_occ_eval(prefix + "_eval_occ_dn.txt", wx.eval[spin_dn], occ_scf[spin_dn]);

    for(int isp: {spin_up, spin_dn}){
      wx.occ_int[isp].resize(occ_scf[isp].size());
      for(int i = 0; i < occ_scf[isp].size(); i++){
	wx.occ_int[isp][i] = static_cast<int>(std::round(occ_scf[isp][i]));
      }
    }
  }

  wx.k_point = k_point;
  wx.is_one_comp_pol = 1;
  wx.verbose = 0;
  wx.nbands_occ_omitted = 0;
  wx.nbands_vir_omitted = 0;
  wx.kp_to_calc = -1;
  wx.are_t_ampls_read = 0;
  wx.iter_max_ccsd = 150;
  wx.eps_dt_ccsd = 0.0;
  wx.smear_t1_ccsd = 0.0;
  wx.smear_t2_ccsd = 0.0;
  wx.mix_ratio_ccsd = 1.0;  
  wx.conv_ene_ccsd = 1.e-7;
  wx.are_ampls_written = 1;
  wx.conv_ampl_lambda = 1.e-10;
  wx.is_gf_calc = 1;
  wx.omega_min = -1.5;
  wx.omega_max = 1.5;
  wx.nomegas = 400;
  wx.omega_eta = 0.01;
  wx.are_eom_mat_written = 0;
  wx.are_diag_comps_gf_only = 1;
  wx.is_from_beginning = 1;

  wx.start();

  mpi_util.single_printf("======================== returned from WaveX ========================\n");

  mpi_util.show_lapse();
  mpi_util.finalize();
  mpi_util.single_printf("finished\n");
  return 0;
}


std::vector<double> gen_k_points(std::array<int, ndirs> const& div){
  std::vector<double> k_point(div[0]*div[1]*div[2]*ndirs);

  int cnt = 0;
  for(int ir1 = 0; ir1 < div[0]; ir1++){
    double const r1 = static_cast<double>(ir1)/div[0];

    for(int ir2 = 0; ir2 < div[1]; ir2++){
      double const r2 = static_cast<double>(ir2)/div[1];

      for(int ir3 = 0; ir3 < div[2]; ir3++){
        double const r3 = static_cast<double>(ir3)/div[2];
        k_point[cnt*ndirs    ] = r1;
        k_point[cnt*ndirs + 1] = r2;
        k_point[cnt*ndirs + 2] = r3;
        cnt++;
      }
    }
  }

  return k_point;
}


void read_occ_eval(std::string const& fname_in, std::vector<double>& eval, std::vector<double>& occ){
  ReadLines f_in;
  f_in.load(fname_in, "#");

  int const nkpts = f_in[0];
  int const nbands = f_in[1];
  f_in++;

  eval.clear();
  eval.resize(nkpts*nbands);

  occ.clear();
  occ.resize(nkpts*nbands);

  for(int ik = 0; ik < nkpts; ik++){
    for(int ib = 0; ib < nbands; ib++){
      int const ind = ik*nbands + ib;
      eval[ind] = f_in[0];
      occ[ind] = f_in[1];
      f_in++;
    }
  }
}
