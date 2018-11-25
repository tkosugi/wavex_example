#include "omputil.h"


int OMPUtil::get_max_threads(){
#ifdef _OPENMP
  return omp_get_max_threads();
#else
  return 1;
#endif
}


int OMPUtil::get_nthreads(){
#ifdef _OPENMP
  return omp_get_num_threads();
#else
  return 1;
#endif
}


int OMPUtil::get_mythread(){
#ifdef _OPENMP
  return omp_get_thread_num();
#else
  return 0;
#endif
}


std::tuple<int, int> OMPUtil::threads_info(){
  return {get_nthreads(), get_mythread()};
}
