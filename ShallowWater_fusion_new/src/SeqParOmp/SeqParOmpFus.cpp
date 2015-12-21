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

class SeqParOmpFus:virtual public Go
{
public:
  Go * limhd0;
  Go * limhl0;
  Go * limhu0;
  Go * limhr0;
  Go * sync0;

  Go * huvh1l0;
  Go * huvh2l0;
  
  Go * delzc2f0;
  Go * delzf00;
  Go * delzc1f0;
  Go * delzf10;

  Go * bloc1x10;
  Go * bloc1y10;
  
  Go * bloc1x30;
  Go * bloc1y30;
  
  Go * bloc210;

  Go * bloc220;
  Go * check0;

  Go * checkus0;
  Go * checkvs0;
  
  Go * limhd1;
  Go * limhl1;
  Go * limhu1;
  Go * limhr1;
  Go * sync1;

  Go * huvh1l1;
  Go * huvh2l1;

  Go * delzc2f1;
  Go * delzf01;
  Go * delzc1f1;
  Go * delzf11;

  Go * bloc1x11;
  Go * bloc1y11;

  Go * bloc1x31;
  Go * bloc1y31;

  Go * bloc211;

  Go * bloc221;
  Go * heun1;

  Go * heun2;
  Go * heun3;

  SeqParOmpFus (){}
  ~SeqParOmpFus (){}

  void go ()
  {
#pragma omp parallel proc_bind(close)
    {
#pragma omp master
    {
      //SeqPar with task creation
      //first boundary conditions
      #pragma omp task untied final(1)
      limhd0->go();
      #pragma omp task untied final(1)
      limhl0->go();
      #pragma omp task untied final(1)
      limhu0->go();
      #pragma omp task untied final(1)
      limhr0->go();

      #pragma omp taskwait

      //sync 1
      sync0->go();

      #pragma omp task untied final(1)
      huvh1l0->go();
      #pragma omp task untied final(1)
      huvh2l0->go();

      #pragma omp taskwait

      #pragma omp task untied final(1)
      delzc2f0->go();
      #pragma omp task untied final(1)
      delzf00->go();
      #pragma omp task untied final(1)
      delzc1f0->go();
      #pragma omp task untied final(1)
      delzf10->go();

      #pragma omp taskwait

      #pragma omp task untied final(1)
      bloc1x10->go();
      #pragma omp task untied final(1)
      bloc1y10->go();

      #pragma omp taskwait

      #pragma omp task untied final(1)
      bloc1x30->go();
      #pragma omp task untied final(1)
      bloc1y30->go();

      #pragma omp taskwait

      bloc210->go();

      #pragma omp task untied final(1)
      bloc220->go();
      #pragma omp task untied final(1)
      check0->go();

      #pragma omp taskwait

      #pragma omp task untied final(1)
      checkus0->go();
      #pragma omp task untied final(1)
      checkvs0->go();

      #pragma omp taskwait

      #pragma omp task untied final(1)
      limhd1->go();
      #pragma omp task untied final(1)
      limhl1->go();
      #pragma omp task untied final(1)
      limhu1->go();
      #pragma omp task untied final(1)
      limhr1->go();

      #pragma omp taskwait

      sync1->go();

      #pragma omp task untied final(1)
      huvh1l1->go();
      #pragma omp task untied final(1)
      huvh2l1->go();

      #pragma omp taskwait

      #pragma omp task untied final(1)
      delzc2f1->go();
      #pragma omp task untied final(1)
      delzf01->go();
      #pragma omp task untied final(1)
      delzc1f1->go();
      #pragma omp task untied final(1)
      delzf11->go();

      #pragma omp taskwait

      #pragma omp task untied final(1)
      bloc1x11->go();
      #pragma omp task untied final(1)
      bloc1y11->go();

      #pragma omp taskwait

      #pragma omp task untied final(1)
      bloc1x31->go();
      #pragma omp task untied final(1)
      bloc1y31->go();

      #pragma omp taskwait

      bloc211->go();

      #pragma omp task untied final(1)
      bloc221->go();
      #pragma omp task untied final(1)
      heun1->go();

      #pragma omp taskwait

      #pragma omp task untied final(1)
      heun2->go();
      #pragma omp task untied final(1)
      heun3->go();

      #pragma omp taskwait

    }
    }
  }
  
};

#include <SeqParOmp/SeqParOmpFus.l2c>
