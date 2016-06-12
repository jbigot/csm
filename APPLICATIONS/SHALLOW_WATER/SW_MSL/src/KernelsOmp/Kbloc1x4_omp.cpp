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

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)
#define grav 9.81
#define grav_dem 4.905

class Kbloc1x4_omp:virtual public Go
{
public:
  Datah* u1l;
  Datah* u1r;
  Datah* h1g;
  Datah* h1d;
  Datah* f2;

  Kbloc1x4_omp(){}
  ~Kbloc1x4_omp(){}
  
  void go ()
  {
    Chronometer chrono;
    chrono.start();

    //cout<<"====START Kbloc1x4===="<<endl;
    Controller<double> cu1l(u1l);
    Controller<double> cu1r(u1r);
    Controller<double> ch1g(h1g);
    Controller<double> ch1d(h1d);
    Controller<double> cf2(f2);

    int64_t xx,yy;
    double c,cd,qd,qg;

//dynamic also possible
#pragma omp parallel for shared(cu1r,cu1l,ch1g,ch1d,cf2) private(yy,xx,c,cd,qd,qg) schedule (static, CHUNK)
    for(yy = cu1l.start(); yy<cu1l.height();yy++)
    {
      for(xx = cu1l.start();xx<cu1l.width(); xx++)
      {
        if (ch1g(xx,yy)<=0. && ch1d(xx,yy)<=0.)
        {
          cf2(xx,yy)=0.;
        }
        else
        {
          c = max(fabs(cu1l(xx,yy))+sqrt(grav*ch1g(xx,yy)),fabs(cu1r(xx,yy))+sqrt(grav*ch1d(xx,yy)));
          cd = c*0.5;
          qd = cu1r(xx,yy)*ch1d(xx,yy);
          qg = cu1l(xx,yy)*ch1g(xx,yy);
          cf2(xx,yy)=((cu1l(xx,yy)*qg)+(grav_dem*ch1g(xx,yy)*ch1g(xx,yy))+(cu1r(xx,yy)*qd)+(grav_dem*ch1d(xx,yy)*ch1d(xx,yy)))*0.5-cd*(qd-qg);
        }
      }
    }
  }

};

#include <KernelsOmp/Kbloc1x4_omp.l2c>
