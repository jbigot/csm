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
  Go * limhd0;
  Go * limud0;
  Go * limvd0;
  Go * limhl0;
  Go * limul0;
  Go * limvl0;
  Go * limhu0;
  Go * limuu0;
  Go * limvu0;
  Go * limhr0;
  Go * limur0;
  Go * limvr0;
  Go * sync0;

  Go * huvh1l0;
  Go * huvh1r0;
  Go * huvu1l0;
  Go * huvu1r0;
  Go * huvv1l0;
  Go * huvv1r0;
  Go * huvz1l0;
  Go * huvz1r0;

  Go * huvh2l0;
  Go * huvh2r0;
  Go * huvu2l0;
  Go * huvu2r0;
  Go * huvv2l0;
  Go * huvv2r0;
  Go * huvz2l0;
  Go * huvz2r0;

  Go * delzc2f0;
  Go * delzf00;
  Go * delzc1f0;
  Go * delzf10;

  Go * bloc1x10;
  Go * bloc1x20;
  Go * bloc1y10;
  Go * bloc1y20;

  Go * bloc1x30;
  Go * bloc1x40;
  Go * bloc1x50;
  Go * bloc1y30;
  Go * bloc1y40;
  Go * bloc1y50;

  Go * bloc210;

  Go * bloc220;
  Go * bloc230;
  Go * check0;

  Go * checkus0;
  Go * checkvs0;
  Go * limhd1;
  Go * limhl1;
  Go * limhr1;
  Go * limhu1;

  Go * limud1;
  Go * limvd1;
  Go * limul1;
  Go * limvl1;
  Go * limuu1;
  Go * limvu1;
  Go * limur1;
  Go * limvr1;
  Go * sync1;

  Go * huvh1l1;
  Go * huvh1r1;
  Go * huvu1l1;
  Go * huvu1r1;
  Go * huvv1l1;
  Go * huvv1r1;
  Go * huvz1l1;
  Go * huvz1r1;

  Go * huvh2l1;
  Go * huvh2r1;
  Go * huvu2l1;
  Go * huvu2r1;
  Go * huvv2l1;
  Go * huvv2r1;
  Go * huvz2l1;
  Go * huvz2r1;

  Go * delzc2f1;
  Go * delzf01;
  Go * delzc1f1;
  Go * delzf11;

  Go * bloc1x11;
  Go * bloc1x21;
  Go * bloc1y11;
  Go * bloc1y21;

  Go * bloc1x31;
  Go * bloc1x41;
  Go * bloc1x51;
  Go * bloc1y31;
  Go * bloc1y41;
  Go * bloc1y51;

  Go * bloc211;

  Go * bloc221;
  Go * bloc231;
  Go * heun1;

  Go * heun2;
  Go * heun3;

  SeqParOmp (){}
  ~SeqParOmp (){}

  void go ()
  {
#pragma omp parallel proc_bind(spread)
    {
      //for(int i=0;i<500;i++)
      //{
#pragma omp master
    {
      //for(int i=0;i<500;i++)
      //{
      //SeqPar with task creation
      //first boundary conditions
      #pragma omp task untied final(1)
      limhd0->go();
      #pragma omp task untied final(1)
      limud0->go();
      #pragma omp task untied final(1)
      limvd0->go();
      #pragma omp task untied final(1)
      limhl0->go();
      #pragma omp task untied final(1)
      limul0->go();
      #pragma omp task untied final(1)
      limvl0->go();
      #pragma omp task untied final(1)
      limhu0->go();
      #pragma omp task untied final(1)
      limuu0->go();
      #pragma omp task untied final(1)
      limvu0->go();
      #pragma omp task untied final(1)
      limhr0->go();
      #pragma omp task untied final(1)
      limur0->go();
      #pragma omp task untied final(1)
      limvr0->go();
      //    }
      #pragma omp taskwait

      //#pragma omp master
	//{
      //sync 1
      sync0->go();

      #pragma omp task untied final(1)
      huvh1l0->go();
      #pragma omp task untied final(1)
      huvh1r0->go();
      #pragma omp task untied final(1)
      huvu1l0->go();
      #pragma omp task untied final(1)
      huvu1r0->go();
      #pragma omp task untied final(1)
      huvv1l0->go();
      #pragma omp task untied final(1)
      huvv1r0->go();
      #pragma omp task untied final(1)
      huvz1l0->go();
      #pragma omp task untied final(1)
      huvz1r0->go();

      #pragma omp task untied final(1)
      huvh2l0->go();
      #pragma omp task untied final(1)
      huvh2r0->go();
      #pragma omp task untied final(1)
      huvu2l0->go();
      #pragma omp task untied final(1)
      huvu2r0->go();
      #pragma omp task untied final(1)
      huvv2l0->go();
      #pragma omp task untied final(1)
      huvv2r0->go();
      #pragma omp task untied final(1)
      huvz2l0->go();
      #pragma omp task untied final(1)
      huvz2r0->go();
      //}

      #pragma omp taskwait

      // #pragma omp master
	//{
      #pragma omp task untied final(1)
      delzc2f0->go();
      #pragma omp task untied final(1)
      delzf00->go();
      #pragma omp task untied final(1)
      delzc1f0->go();
      #pragma omp task untied final(1)
      delzf10->go();
      //}

      #pragma omp taskwait

      //#pragma omp master
	//{
      #pragma omp task untied final(1)
      bloc1x10->go();
      #pragma omp task untied final(1)
      bloc1x20->go();
      #pragma omp task untied final(1)
      bloc1y10->go();
      #pragma omp task untied final(1)
      bloc1y20->go();
      //}

      #pragma omp taskwait

      //#pragma omp master
	//{
      #pragma omp task untied final(1)
      bloc1x30->go();
      #pragma omp task untied final(1)
      bloc1x40->go();
      #pragma omp task untied final(1)
      bloc1x50->go();
      #pragma omp task untied final(1)
      bloc1y30->go();
      #pragma omp task untied final(1)
      bloc1y40->go();
      #pragma omp task untied final(1)
      bloc1y50->go();
      //}

      #pragma omp taskwait

      //#pragma omp master
	//{
      bloc210->go();

      #pragma omp task untied final(1)
      bloc220->go();
      #pragma omp task untied final(1)
      bloc230->go();
      #pragma omp task untied final(1)
      check0->go();
      //}

      #pragma omp taskwait

      //#pragma omp master
	//{
      #pragma omp task untied final(1)
      checkus0->go();
      #pragma omp task untied final(1)
      checkvs0->go();
      #pragma omp task untied final(1)
      limhl1->go();
      #pragma omp task untied final(1)
      limhr1->go();
      #pragma omp task untied final(1)
      limhu1->go();
      #pragma omp task untied final(1)
      limhd1->go();
      //}

      #pragma omp taskwait

      // #pragma omp master
	//{
      sync1->go();

      #pragma omp task untied final(1)
      huvh1l1->go();
      #pragma omp task untied final(1)
      huvh1r1->go();
      #pragma omp task untied final(1)
      huvu1l1->go();
      #pragma omp task untied final(1)
      huvu1r1->go();
      #pragma omp task untied final(1)
      huvv1l1->go();
      #pragma omp task untied final(1)
      huvv1r1->go();
      #pragma omp task untied final(1)
      huvz1l1->go();
      #pragma omp task untied final(1)
      huvz1r1->go();

      #pragma omp task untied final(1)
      huvh2l1->go();
      #pragma omp task untied final(1)
      huvh2r1->go();
      #pragma omp task untied final(1)
      huvu2l1->go();
      #pragma omp task untied final(1)
      huvu2r1->go();
      #pragma omp task untied final(1)
      huvv2l1->go();
      #pragma omp task untied final(1)
      huvv2r1->go();
      #pragma omp task untied final(1)
      huvz2l1->go();
      #pragma omp task untied final(1)
      huvz2r1->go();
      //}

      #pragma omp taskwait

      //#pragma omp master
	//{
      #pragma omp task untied final(1)
      delzc2f1->go();
      #pragma omp task untied final(1)
      delzf01->go();
      #pragma omp task untied final(1)
      delzc1f1->go();
      #pragma omp task untied final(1)
      delzf11->go();
      //}

      #pragma omp taskwait

      //#pragma omp master
	//{
      #pragma omp task untied final(1)
      bloc1x11->go();
      #pragma omp task untied final(1)
      bloc1x21->go();
      #pragma omp task untied final(1)
      bloc1y11->go();
      #pragma omp task untied final(1)
      bloc1y21->go();
      //}

      #pragma omp taskwait

      //#pragma omp master
	//{
      #pragma omp task untied final(1)
      bloc1x31->go();
      #pragma omp task untied final(1)
      bloc1x41->go();
      #pragma omp task untied final(1)
      bloc1x51->go();
      #pragma omp task untied final(1)
      bloc1y31->go();
      #pragma omp task untied final(1)
      bloc1y41->go();
      #pragma omp task untied final(1)
      bloc1y51->go();
      //}

      #pragma omp taskwait

      //#pragma omp master
	//{
      bloc211->go();

      #pragma omp task untied final(1)
      bloc221->go();
      #pragma omp task untied final(1)
      bloc231->go();
      #pragma omp task untied final(1)
      heun1->go();
      //}

      #pragma omp taskwait

      //#pragma omp master
	//{
      #pragma omp task untied final(1)
      heun2->go();
      #pragma omp task untied final(1)
      heun3->go();

      #pragma omp taskwait

      //}
    }
    }
  }
  
};

#include <SeqParOmp/SeqParOmp.l2c>
