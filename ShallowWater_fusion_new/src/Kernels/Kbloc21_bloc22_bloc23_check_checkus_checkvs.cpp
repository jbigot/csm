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
#define he_ca 1.e-6
#define ve_ca 1.e-6

class Kbloc21_bloc22_bloc23_check_checkus_checkvs:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
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
  Datah* hs;
  Datah* qs1;
  Datah* qs2;
  Datah* Rain;

  Datah* us;
  Datah* vs;

  Kbloc21_bloc22_bloc23_check_checkus_checkvs(){}
  ~Kbloc21_bloc22_bloc23_check_checkus_checkvs(){}
  
  void go ()
  {
    //cout<<"====START Kbloc21_bloc22_bloc23_check_checkus_checkvs===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cq1(q1);
    Controller<double> cq2(q2);
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
    //Controller<double> crainm(rainm);
    //Controller<double> cmcf(mcf);
    Controller<double> chs(hs);
    Controller<double> cqs1(qs1);
    Controller<double> cqs2(qs2);
    //Controller<double> cRain(Rain);

    //check
    Controller<double> cus(us);
    Controller<double> cvs(vs);

    double tx=0.005;
    double ty=0.005;
    double dt=0.5;

    int64_t yyq=cq1.start();
    int64_t yy1=cf1.start();
    int64_t yy2=cg1.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++,yyq++,yy1++,yy2++)
    {
      int64_t xxq=cq1.start();
      int64_t xx1=cf1.start();
      int64_t xx2=cg1.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xxq++,xx1++,xx2++)
      {
        chs(xx,yy) = ch(xx,yy)-tx*(cf1(xx1+1,yy1)-cf1(xx1,yy1))-ty*(cg1(xx2,yy2)-cg1(xx2,yy2+1)) ;//+ cRain(xxq,yyq)*dt;
        cqs1(xxq,yyq) = cq1(xxq,yyq)-tx*(cf2(xx1+1,yy1)-cf2(xx1,yy1)+grav_dem*(ch1d(xx1,yy1)*ch1d(xx1,yy1)-ch1r(xx1,yy1)*ch1r(xx1,yy1)
          +ch1l(xx1+1,yy1)*ch1l(xx1+1,yy1)-ch1g(xx1+1,yy1)*ch1g(xx1+1,yy1)+(ch1r(xx1+1,yy1)+ch1l(xx1+1,yy1))*cdelzc1(xxq,yyq)))
          -ty*(cg2(xx2,yy2)-cg2(xx2,yy2+1));
        cqs2(xxq,yyq)= cq2(xxq,yyq)-tx*(cf3(xx1+1,yy1)-cf3(xx1,yy1))-ty*(cg3(xx2,yy2)-cg3(xx2,yy2+1)+grav_dem*(ch2d(xx2,yy2+1)
          *ch2d(xx2,yy2+1)-ch2r(xx2,yy2+1)*ch2r(xx2,yy2+1)+ch2l(xx2,yy2)*ch2l(xx2,yy2)-ch2g(xx2,yy2)*ch2g(xx2,yy2)+(ch2r(xx2,yy2+1)
          +ch2l(xx2,yy2))*cdelzc2(xxq,yyq)));
        cqs1(xxq,yyq)= cqs1(xxq,yyq)/(1./*+cmcf(xxq,yyq)*/*sqrt(cu(xx,yy)*cu(xx,yy)+cv(xx,yy)*cv(xx,yy))*dt/(8.*chs(xx,yy)));
        cqs2(xxq,yyq)= cqs2(xxq,yyq)/(1./*+cmcf(xxq,yyq)*/*sqrt(cu(xx,yy)*cu(xx,yy)+cv(xx,yy)*cv(xx,yy))*dt/(8.*chs(xx,yy)));

        //check checkus checkvs
        if(chs(xx,yy)<he_ca)
        {
          chs(xx,yy)=0.;
          cus(xx,yy)=0.;
          cvs(xx,yy)=0.;
          cqs1(xxq,yyq)=0.;
          cqs2(xxq,yyq)=0.;
        }
        else
        {
          cus(xx,yy)=cqs1(xxq,yyq)/chs(xx,yy);
          if(fabs(cus(xx,yy))<ve_ca)
          {
            cus(xx,yy)=0.;
            cqs1(xxq,yyq)=0.;
          }
          cvs(xx,yy)=cqs2(xxq,yyq)/chs(xx,yy);
          if(fabs(cvs(xx,yy))<ve_ca)
          {
            cvs(xx,yy)=0.;
            cqs2(xxq,yyq)=0.;
          }
        }
      }
    }                                                                                                                                                                                      
  }

};

#include <Kernels/Kbloc21_bloc22_bloc23_check_checkus_checkvs.l2c>
