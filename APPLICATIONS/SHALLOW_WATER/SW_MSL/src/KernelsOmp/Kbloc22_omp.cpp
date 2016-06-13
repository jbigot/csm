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

class Kbloc22_omp:virtual public Go
{
public:
  Datah* u;
  Datah* v;
  Datah* q1;
  Datah* h1g;
  Datah* h1d;
  Datah* h1l;
  Datah* h1r;
  Datah* delzc1;
  Datah* f2;
  Datah* g2;
  Datah* hs;
  Datah* qs1;

  Kbloc22_omp(){}
  ~Kbloc22_omp(){}
  
  void go ()
  {
    //cout<<"====START Kbloc22===="<<endl;
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cq1(q1);
    Controller<double> ch1g(h1g);
    Controller<double> ch1d(h1d);
    Controller<double> ch1l(h1l);
    Controller<double> ch1r(h1r);
    Controller<double> cdelzc1(delzc1);
    Controller<double> cf2(f2);
    Controller<double> cg2(g2);
    Controller<double> chs(hs);
    Controller<double> cqs1(qs1);

    double tx=0.005;
    double ty=0.005;
    double dt=0.5;

    int64_t yyq=cq1.start();
    int64_t yy1=ch1r.start();
    int64_t yy2=cg2.start();

    int64_t xx,yy,xxq,xx1,xx2;

//dynamic also possible
#pragma omp parallel for shared(cu,cv,cq1,ch1g,ch1d,ch1l,ch1r,cdelzc1,cf2,cg2,chs,cqs1) private(yy,xx,xxq,xx1,xx2) firstprivate(yyq,yy1,yy2) schedule (static, CHUNK)
    for(yy = chs.start(); yy<chs.height();yy++)
    {
      xxq=cq1.start();
      xx1=ch1r.start();
      xx2=cg2.start();
      for(xx = chs.start();xx<chs.width(); xx++)
      {
        cqs1(xxq,yyq) = cq1(xxq,yyq)-tx*(cf2(xx1+1,yy1)-cf2(xx1,yy1)+grav_dem*(ch1d(xx1,yy1)*ch1d(xx1,yy1)-ch1r(xx1,yy1)*ch1r(xx1,yy1)
          +ch1l(xx1+1,yy1)*ch1l(xx1+1,yy1)-ch1g(xx1+1,yy1)*ch1g(xx1+1,yy1)+(ch1r(xx1+1,yy1)+ch1l(xx1+1,yy1))*cdelzc1(xxq,yyq)))
          -ty*(cg2(xx2,yy2)-cg2(xx2,yy2+1));
        //cqs1(xxq,yyq)= cqs1(xxq,yyq)/(1.+cmcf(xxq,yyq)*sqrt(cu(xx,yy)*cu(xx,yy)+cv(xx,yy)*cv(xx,yy))*dt/(8.*chs(xx,yy)));
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

#include <KernelsOmp/Kbloc22_omp.l2c>
