// Taichi Kosugi
// since Jun 30, 2016

#ifndef OMPUTIL_DEFINED
#define OMPUTIL_DEFINED

#include <tuple>

#ifdef _OPENMP
#include "omp.h"
#endif


class OMPUtil{
 public:
  static int get_max_threads();
  static int get_nthreads();
  static int get_mythread();
  static std::tuple<int, int> threads_info();

  OMPUtil() = delete;
};

#endif
