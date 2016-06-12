#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>
#include <math.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define grav_dem 4.905

class Kbloc21_bloc22_bloc23_heun1_heun2_heun3_omp:virtual public Go
{
public:
  Datah* h;
  Datah* us;
  Datah* vs;
  Datah* hs;
  Datah* q1;
  Datah* q2;
  Datah* h1g;
  Datah* h1d;
  Datah* h1l;
  Datah* h1r;
  Datah* h2g;
  Datah* h2d;
  Datah* h2l;
  Datah* h2r;
  Datah* delzc1;
  Datah* delzc2;
  Datah* f1;
  Datah* f2;
  Datah* f3;
  Datah* g1;
  Datah* g2;
  Datah* g3;
  Datah* rainm;
  Datah* mcf;
  Datah* qs1;
  Datah* qs2;
  Datah* Rain;

  //heun
  Datah* hsa;
  Datah* qsa1;
  Datah* qsa2;


  Kbloc21_bloc22_bloc23_heun1_heun2_heun3_omp(){}
  ~Kbloc21_bloc22_bloc23_heun1_heun2_heun3_omp(){}
  
  void go ()
  {
    //cout<<"====START Kbloc21_bloc22_bloc23_heun1_heun2_heun3===="<<endl;
    //bloc21
    Controller<double> ch(hs);
    Controller<double> cu(us);
    Controller<double> cv(vs);
    Controller<double> ch1g(h1g);
    Controller<double> ch1d(h1d);
    Controller<double> ch1l(h1l);
    Controller<double> ch1r(h1r);
    Controller<double> ch2g(h2g);
    Controller<double> ch2d(h2d);
    Controller<double> ch2l(h2l);
    Controller<double> ch2r(h2r);
    Controller<double> cdelzc1(delzc1);
    Controller<double> cdelzc2(delzc2);
    Controller<double> cf1(f1);
    Controller<double> cf2(f2);
    Controller<double> cf3(f3);
    Controller<double> cg1(g1);
    Controller<double> cg2(g2);
    Controller<double> cg3(g3);
    Controller<double> chs(hsa);
    Controller<double> cqs1(qsa1);
    Controller<double> cqs2(qsa2);
    Controller<double> cq1(qs1);
    Controller<double> cq2(qs2);

    //heun
    Controller<double> chh(h);
    Controller<double> cqq1(q1);
    Controller<double> cqq2(q2);

    double tx=0.005;
    double ty=0.005;
    double dt=0.5;

    int64_t yyq=cq1.start();
    int64_t yy1=cf1.start();
    int64_t yy2=cg1.start();

    int64_t xx,yy,xxq,xx1,xx2;

//dynamic also possible
#pragma omp parallel for shared(ch,cu,cv,ch1g,ch1d,ch1l,ch1r,ch2g,ch2d,ch2l,ch2r,cdelzc1,cdelzc2,cf1,cf2,cf3,cg1,cg2,cg3,chs,cqs1,cqs2,cq1,cq2,chh,cqq1,cqq2) private(yy,xx,yyq,yy1,yy2,xxq,xx1,xx2) schedule (static, CHUNK)
    for(yy = ch.start(); yy<ch.height();yy++)
    {
      xxq=cq1.start();
      xx1=cf1.start();
      xx2=cg1.start();
      for(xx = ch.start();xx<ch.width(); xx++)
      {
        chs(xx,yy) = ch(xx,yy)-tx*(cf1(xx1+1,yy1)-cf1(xx1,yy1))-ty*(cg1(xx2,yy2)-cg1(xx2,yy2+1)) ;//+ cRain(xxq,yyq)*dt;
        cqs1(xxq,yyq) = cq1(xxq,yyq)-tx*(cf2(xx1+1,yy1)-cf2(xx1,yy1)+grav_dem*(ch1d(xx1,yy1)*ch1d(xx1,yy1)-ch1r(xx1,yy1)*ch1r(xx1,yy1)
          +ch1l(xx1+1,yy1)*ch1l(xx1+1,yy1)-ch1g(xx1+1,yy1)*ch1g(xx1+1,yy1)+(ch1r(xx1+1,yy1)+ch1l(xx1+1,yy1))*cdelzc1(xxq,yyq)))
          -ty*(cg2(xx2,yy2)-cg2(xx2,yy2+1));
        cqs2(xxq,yyq)= cq2(xxq,yyq)-tx*(cf3(xx1+1,yy1)-cf3(xx1,yy1))-ty*(cg3(xx2,yy2)-cg3(xx2,yy2+1)+grav_dem*(ch2d(xx2,yy2+1)
          *ch2d(xx2,yy2+1)-ch2r(xx2,yy2+1)*ch2r(xx2,yy2+1)+ch2l(xx2,yy2)*ch2l(xx2,yy2)-ch2g(xx2,yy2)*ch2g(xx2,yy2)+(ch2r(xx2,yy2+1)
          +ch2l(xx2,yy2))*cdelzc2(xxq,yyq)));

        //heun
        chh(xx,yy)=0.5*(chh(xx,yy)+chs(xx,yy));
        cqq1(xxq,yyq)=0.5*(cqq1(xxq,yyq)+cqs1(xxq,yyq));
        cqq2(xxq,yyq)=0.5*(cqq2(xxq,yyq)+cqs2(xxq,yyq));

        xxq++;
        xx1++;
        xx2++;
      }
      yyq++;
      yy1++;
      yy2++;
    }                                                                                                                                                                                      
  }

};

#include <KernelsOmp/Kbloc21_bloc22_bloc23_heun1_heun2_heun3_omp.l2c>
