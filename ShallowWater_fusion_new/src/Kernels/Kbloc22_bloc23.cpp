#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>
#include <math.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define grav_dem 4.905

class Kbloc22_bloc23:virtual public Go
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
  Datah* q2;
  Datah* h2g;
  Datah* h2d;
  Datah* h2l;
  Datah* h2r;
  Datah* delzc2;
  Datah* f3;
  Datah* g3;
  Datah* qs2;

  Kbloc22_bloc23(){}
  ~Kbloc22_bloc23(){}
  
  void go ()
  {
    //cout<<"====START Kbloc22_bloc23===="<<endl;
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
    Controller<double> cq2(q2);
    Controller<double> ch2g(h2g);
    Controller<double> ch2d(h2d);
    Controller<double> ch2l(h2l);
    Controller<double> ch2r(h2r);
    Controller<double> cdelzc2(delzc2);
    Controller<double> cf3(f3);
    Controller<double> cg3(g3);
    Controller<double> cqs2(qs2);

    double tx=0.005;
    double ty=0.005;
    double dt=0.5;

    int64_t yyq=cq1.start();
    int64_t yy1=ch1r.start();
    int64_t yy2=cg2.start();
    for(int64_t yy = chs.start(); yy<chs.height();yy++,yyq++,yy1++,yy2++)
    {
      int64_t xxq=cq1.start();
      int64_t xx1=ch1r.start();
      int64_t xx2=cg2.start();
      for(int64_t xx = chs.start();xx<chs.width(); xx++,xxq++,xx1++,xx2++)
      {
        cqs1(xxq,yyq) = cq1(xxq,yyq)-tx*(cf2(xx1+1,yy1)-cf2(xx1,yy1)+grav_dem*(ch1d(xx1,yy1)*ch1d(xx1,yy1)-ch1r(xx1,yy1)*ch1r(xx1,yy1)
          +ch1l(xx1+1,yy1)*ch1l(xx1+1,yy1)-ch1g(xx1+1,yy1)*ch1g(xx1+1,yy1)+(ch1r(xx1+1,yy1)+ch1l(xx1+1,yy1))*cdelzc1(xxq,yyq)))
          -ty*(cg2(xx2,yy2)-cg2(xx2,yy2+1));
        cqs2(xxq,yyq)= cq2(xxq,yyq)-tx*(cf3(xx1+1,yy1)-cf3(xx1,yy1))-ty*(cg3(xx2,yy2)-cg3(xx2,yy2+1)+grav_dem*(ch2d(xx2,yy2+1)
          *ch2d(xx2,yy2+1)-ch2r(xx2,yy2+1)*ch2r(xx2,yy2+1)+ch2l(xx2,yy2)*ch2l(xx2,yy2)-ch2g(xx2,yy2)*ch2g(xx2,yy2)+(ch2r(xx2,yy2+1)
          +ch2l(xx2,yy2))*cdelzc2(xxq,yyq)));
        //cqs1(xxq,yyq)= cqs1(xxq,yyq)/(1.+cmcf(xxq,yyq)*sqrt(cu(xx,yy)*cu(xx,yy)+cv(xx,yy)*cv(xx,yy))*dt/(8.*chs(xx,yy)));
      }
    }                                                                                                                                                                                      
  }

};

#include <Kernels/Kbloc22_bloc23.l2c>
