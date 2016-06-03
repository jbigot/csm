#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <omp.h>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class DynOmp:virtual public Go
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

  //dummies variables to create dependencies
  float * h,u,v,z,hs,us,vs,hsa,usa,vsa,q1,q2,qs1,qs2;
  float * f1,f2,f3,g1,g2,g3,h1d,h1g,h1l,h1r,h2d,h2g,h2l,h2r;
  float * delz1,delz2,delzc1,delzc2,qsa1,qsa2;
  float * u1l,u1r,u2l,u2r,v1l,v1r,v2l,v2r,z1l,z1r,z2l,z2r,rain,rainm,mcf;
  //dummies dummies to create modified dependencies (inout / boundaries)
  float * h1,v1,u1,h2,u2,v2,h3,u3,v3,h4,u4,v4;
  float * hs1,vs1,us1,hs2,us2,vs2,hs3,us3,vs3,hs4,us4,vs4;
  float * hsc,usc,vsc;

  DynOmp (){}
  ~DynOmp (){}

  void go ()
  {
#pragma omp parallel proc_bind(close)
    {
#pragma omp master
    {
      //---------------------------------
      //no inout in stencil codes
      //except for synchronizations
      //only one case inout is accepted for computations :
      // when the computation domain is independent (boundary conditions)
      //---------------------------------
      //waw, raw and war should be true when domains are dependent only, otherwise the dependency does not exist
      //in practice only waw is like this
      //---------------------------------
      #pragma omp task untied depend(in:) depend(out:h1) final(1)
      limhd0->go();
      #pragma omp task untied depend(in:) depend(out:u1) final(1)
      limud0->go();
      #pragma omp task untied depend(in:) depend(out:v1) final(1)
      limvd0->go();
      #pragma omp task untied depend(in:) depend(out:h2) final(1)
      limhl0->go();
      #pragma omp task untied depend(in:) depend(out:u2) final(1)
      limul0->go();
      #pragma omp task untied depend(in:) depend(out:v2) final(1)
      limvl0->go();
      #pragma omp task untied depend(in:) depend(out:h3) final(1)
      limhu0->go();
      #pragma omp task untied depend(in:) depend(out:u3) final(1)
      limuu0->go();
      #pragma omp task untied depend(in:) depend(out:v3) final(1)
      limvu0->go();
      #pragma omp task untied depend(in:) depend(out:h4) final(1)
      limhr0->go();
      #pragma omp task untied depend(in:) depend(out:u4) final(1)
      limur0->go();
      #pragma omp task untied depend(in:) depend(out:v4) final(1)
      limvr0->go();
      #pragma omp task untied depend(in:h1,u1,v1,h2,u2,v2,h3,u3,v3,h4,u4,v4) depend(out:h,u,v) final(1)
      sync0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:h1l) final(1)
      huvh1l0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:h1r) final(1)
      huvh1r0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:u1l) final(1)
      huvu1l0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:u1r) final(1)
      huvu1r0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:v1l) final(1)
      huvv1l0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:v1r) final(1)
      huvv1r0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:z1l) final(1)
      huvz1l0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:z1r) final(1)
      huvz1r0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:h2l) final(1)
      huvh2l0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:h2r) final(1)
      huvh2r0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:u2l) final(1)
      huvu2l0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:u2r) final(1)
      huvu2r0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:v2l) final(1)
      huvv2l0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:v2r) final(1)
      huvv2r0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:z2l) final(1)
      huvz2l0->go();
      #pragma omp task untied depend(in:h,u,v,z) depend(out:z2r) final(1)
      huvz2r0->go();

      #pragma omp task untied depend(in:z1l,z1r) depend(out:delz1) final(1)
      delzf00->go();
      #pragma omp task untied depend(in:z1l,z1r) depend(out:delzc1) final(1)
      delzc1f0->go();
      #pragma omp task untied depend(in:z2l,z2r) depend(out:delz2) final(1)
      delzf10->go();
      #pragma omp task untied depend(in:z2l,z2r) depend(out:delzc2) final(1)
      delzc2f0->go();

      #pragma omp task untied depend(in:h1l,delz1) depend(out:h1g) final(1)
      bloc1x10->go();
      #pragma omp task untied depend(in:h1r,delz1) depend(out:h1d) final(1)
      bloc1x20->go();
      #pragma omp task untied depend(in:u1l,u1r,h1g,h1d) depend(out:f1) final(1)
      bloc1x30->go();
      #pragma omp task untied depend(in:u1l,u1r,h1g,h1d) depend(out:f2) final(1)
      bloc1x40->go();
      #pragma omp task untied depend(in:u1l,u1r,v1l,v1r,h1g,h1d) depend(out:f3) final(1)
      bloc1x50->go();
      #pragma omp task untied depend(in:h2l,delz2) depend(out:h2g) final(1)
      bloc1y10->go();
      #pragma omp task untied depend(in:h2r,delz2) depend(out:h2d) final(1)
      bloc1y20->go();
      #pragma omp task untied depend(in:u2l,u2r,h2g,h2d) depend(out:g1) final(1)
      bloc1y30->go();
      #pragma omp task untied depend(in:u2l,u2r,h2g,h2d) depend(out:g2) final(1)
      bloc1y40->go();
      #pragma omp task untied depend(in:u2l,u2r,v2l,v2r,h2g,h2d) depend(out:g3) final(1)
      bloc1y50->go();
      #pragma omp task untied depend(in:h,f1,g1) depend(out:hs) final(1)
      bloc210->go();
      #pragma omp task untied depend(in:u,v,q1,h1l,h1r,h1g,h1d,delzc1,f2,g2,hs) depend(out:qs1) final(1)
      bloc220->go();
      #pragma omp task untied depend(in:u,v,q2,h2l,h2r,h2d,h2g,delzc2,f3,g3,hs) depend(out:qs2) final(1)
      bloc230->go();
      #pragma omp task untied depend(in:hs) depend(out:hsc) final(1)
      check0->go();
      #pragma omp task untied depend(in:hsc,us,qs1) depend(out:usc) final(1)
      checkus0->go();
      #pragma omp task untied depend(in:hsc,qs2,vs) depend(out:vsc) final(1)
      checkvs0->go();
      
      #pragma omp task untied depend(in:hs2) depend(out:hs1) final(1)
      limhd1->go();
      #pragma omp task untied depend(in:us2) depend(out:us1) final(1)
      limud1->go();
      #pragma omp task untied depend(in:vs2) depend(out:vs1) final(1)
      limvd1->go();
      #pragma omp task untied depend(in:hs2) depend(out:hs2) final(1)
      limhl1->go();
      #pragma omp task untied depend(in:us2) depend(out:us2) final(1)
      limul1->go();
      #pragma omp task untied depend(in:vs2) depend(out:vs2) final(1)
      limvl1->go();
      #pragma omp task untied depend(in:hs2) depend(out:hs3) final(1)
      limhu1->go();
      #pragma omp task untied depend(in:us2) depend(out:us3) final(1)
      limuu1->go();
      #pragma omp task untied depend(in:vs2) depend(out:vs3) final(1)
      limvu1->go();
      #pragma omp task untied depend(in:hs2) depend(out:hs4) final(1)
      limhr1->go();
      #pragma omp task untied depend(in:us2) depend(out:us4) final(1)
      limur1->go();
      #pragma omp task untied depend(in:vs2) depend(out:vs4) final(1)
      limvr1->go();

      #pragma omp task untied depend(in:hs1,us1,vs1,hs2,us2,vs2,hs3,us3,vs3,hs4,us4,vs4) depend(out:hs,us,vs) final(1)
      sync1->go();

      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:h1l) final(1)
      huvh1l1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:h1r) final(1)
      huvh1r1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:u1l) final(1)
      huvu1l1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:u1r) final(1)
      huvu1r1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:v1l) final(1)
      huvv1l1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:v1r) final(1)
      huvv1r1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:z1l) final(1)
      huvz1l1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:z1r) final(1)
      huvz1r1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:h2l) final(1)
      huvh2l1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:h2r) final(1)
      huvh2r1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:u2l) final(1)
      huvu2l1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:u2r) final(1)
      huvu2r1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:v2l) final(1)
      huvv2l1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:v2r) final(1)
      huvv2r1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:z2l) final(1)
      huvz2l1->go();
      #pragma omp task untied depend(in:hs,us,vs,z) depend(out:z2r) final(1)
      huvz2r1->go();

      #pragma omp task untied depend(in:z1l,z1r) depend(out:delz1) final(1)
      delzf01->go();
      #pragma omp task untied depend(in:z1l,z1r) depend(out:delzc1) final(1)
      delzc1f1->go();
      #pragma omp task untied depend(in:z2l,z2r) depend(out:delz2) final(1)
      delzf11->go();
      #pragma omp task untied depend(in:z2l,z2r) depend(out:delzc2) final(1)
      delzc2f1->go();

      #pragma omp task untied depend(in:h1l,delz1) depend(out:h1g) final(1)
      bloc1x11->go();
      #pragma omp task untied depend(in:h1r,delz1) depend(out:h1d) final(1)
      bloc1x21->go();
      #pragma omp task untied depend(in:u1l,u1r,h1g,h1d) depend(out:f1) final(1)
      bloc1x31->go();
      #pragma omp task untied depend(in:u1l,u1r,h1g,h1d) depend(out:f2) final(1)
      bloc1x41->go();
      #pragma omp task untied depend(in:u1l,u1r,v1l,v1r,h1g,h1d) depend(out:f3) final(1)
      bloc1x51->go();
      #pragma omp task untied depend(in:h2l,delz2) depend(out:h2g) final(1)
      bloc1y11->go();
      #pragma omp task untied depend(in:h2r,delz2) depend(out:h2d) final(1)
      bloc1y21->go();
      #pragma omp task untied depend(in:u2l,u2r,h2g,h2d) depend(out:g1) final(1)
      bloc1y31->go();
      #pragma omp task untied depend(in:u2l,u2r,h2g,h2d) depend(out:g2) final(1)
      bloc1y41->go();
      #pragma omp task untied depend(in:u2l,u2r,v2l,v2r,h2g,h2d) depend(out:g3) final(1)
      bloc1y51->go();
      #pragma omp task untied depend(in:hs,f1,g1) depend(out:hsa) final(1)
      bloc211->go();
      #pragma omp task untied depend(in:us,vs,qs1,h1l,h1r,h1g,h1d,delzc1,f2,g2,hsa) depend(out:qsa1) final(1)
      bloc221->go();
      #pragma omp task untied depend(in:us,vs,qs2,h2l,h2r,h2d,h2g,delzc2,f3,g3,hsa) depend(out:qsa2) final(1)
      bloc231->go();

      #pragma omp task untied depend(in:hsa) depend(out:h) final(1)
      heun1->go();
      #pragma omp task untied depend(in:qsa1,h) depend(out:q1) final(1)
      heun2->go();
      #pragma omp task untied depend(in:qsa2,h) depend(out:q2) final(1)
      heun3->go();

      //synchronization at each time iteration
      #pragma omp taskwait
    }
    }
  }
  
};

#include <DynOmp/DynOmp.l2c>
