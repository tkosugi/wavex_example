// Taichi Kosugi
// Nov 23, 2018

#include <stdio.h>
#include <array>
#include <vector>

#include "mpiutil.h"
#include "omputil.h"
#include "wavex.h"

  int const ndirs = 3;
  int const spin_up = 0;
  int const spin_dn = 1;

std::vector<double> gen_k_points(std::array<int, ndirs> const& k_div);

int main(int argc, char *argv[]){
  int const RANK_0 = 0;

  // ad hoc!!!!!
  std::array<int, ndirs> const k_div {4, 1, 1};


  MPIUtil::init(argc, argv);
  MPIUtil mpi_util(MPIUtil::comm_world());

  int const myrank = mpi_util.myrank;
  int const nprocs = mpi_util.nprocs;

  mpi_util.single_printf("%d OpenMP threads in each of %d MPI processes\n\n", OMPUtil::get_max_threads(), nprocs);

  auto const k_point = gen_k_points(k_div);
  int const nkpts = k_point.size()/ndirs;

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


  /*  
  // Calculate two-electron integrals according to the specified distribution pattern.
  calc_twoel_Bloch_states<double>(suffix_twoel_for_WaveX, true, nranks_k);
  */


  WaveXParams wx;

  wx.k_point = k_point;
  wx.is_one_comp_pol = 1;
  /*
  wx.eval[spin_up] = eval_scf[spin_up];
  wx.eval[spin_dn] = eval_scf[spin_dn];
  wx.occ_int[spin_up] = occ_int[spin_up];
  wx.occ_int[spin_dn] = occ_int[spin_dn];
  wx.verbose = params.wavex_verbose;
  wx.nbands_occ_omitted = params.wavex_nbands_occ_omitted;
  wx.nbands_vir_omitted = params.wavex_nbands_vir_omitted;
  */
  wx.kp_to_calc = -1;
  /*
  wx.are_t_ampls_read = params.wavex_are_t_ampls_read;
  wx.iter_max_ccsd = params.wavex_iter_max_ccsd;
  wx.eps_dt_ccsd = params.wavex_eps_dt_ccsd;
  wx.smear_t1_ccsd = params.wavex_smear_t1_ccsd;
  wx.smear_t2_ccsd = params.wavex_smear_t2_ccsd;
  wx.mix_ratio_ccsd = params.wavex_mix_ratio_ccsd;
  wx.conv_ene_ccsd = params.wavex_conv_ene_ccsd;
  wx.are_ampls_written = params.wavex_are_ampls_written;
  wx.conv_ampl_lambda = params.wavex_conv_ampl_lambda;
  wx.is_gf_calc = params.wavex_is_gf_calc;
  wx.omega_min = params.wavex_omega_min;
  wx.omega_max = params.wavex_omega_max;
  wx.nomegas = params.wavex_nomegas;
  wx.omega_eta = params.wavex_omega_eta;
  wx.are_eom_mat_written = params.wavex_are_eom_mat_written;
  wx.are_diag_comps_gf_only = params.wavex_are_diag_comps_gf_only;
  wx.is_from_beginning = params.wavex_is_from_beginning ? 1: 0;

  wx.start();

  */

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
