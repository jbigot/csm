#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <omp.h>
//#include <datah.h>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class SeqParOmp:virtual public Go
{
public:
  Go * limhd;
  Go * limud;
  Go * limvd;
  Go * sync0;

  SeqParOmp (){}
  ~SeqParOmp (){}

  void go ()
  {
#pragma omp parallel
    {
#pragma omp single
    {
      //SeqPar with task creation
      //first boundary conditions
      #pragma omp task
      limhd->go();
      #pragma omp task
      limud->go();
      #pragma omp task
      limvd->go();
      #pragma omp taskwait

      //sync 1
      sync0->go();
    }
    }
  }
  
};

#include <SeqParOmp/SeqParOmp.l2c>
