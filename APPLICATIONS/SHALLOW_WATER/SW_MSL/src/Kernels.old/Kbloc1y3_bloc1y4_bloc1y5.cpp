#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>
#include <math.h>

#include "rdtsc.h"

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)
#define grav 9.81
#define grav_dem 4.905

class Kbloc1y3_bloc1y4_bloc1y5:virtual public Go
{
public:
  Datah* u2l;
  Datah* u2r;
  Datah* v2l;
  Datah* v2r;
  Datah* h2g;
  Datah* h2d;
  Datah* g1;
  Datah* g2;
  Datah* g3;

  Kbloc1y3_bloc1y4_bloc1y5(){}
  ~Kbloc1y3_bloc1y4_bloc1y5(){}
  
  void go ()
  {
    //unsigned long long a,b;
    //a=rdtsc();

    //cout<<"====START Kbloc1y3_bloc1y4_bloc1y5===="<<endl;
    Controller<double> cu2l(u2l);
    Controller<double> cu2r(u2r);
    Controller<double> cv2l(v2l);
    Controller<double> cv2r(v2r);
    Controller<double> ch2g(h2g);
    Controller<double> ch2d(h2d);
    Controller<double> cg1(g1);
    Controller<double> cg2(g2);
    Controller<double> cg3(g3);

    for(int64_t yy = cu2l.start(); yy<cu2l.height();yy++)
    {
      for(int64_t xx = cu2l.start();xx<cu2l.width(); xx++)
      {
        if (ch2g(xx,yy)<=0. && ch2d(xx,yy)<=0.)
        {
          cg1(xx,yy)=0.;
          cg2(xx,yy)=0.;
          cg3(xx,yy)=0.;
        }
        else
        {
          double c = max(fabs(cu2l(xx,yy))+sqrt(grav*ch2g(xx,yy)),fabs(cu2r(xx,yy))+sqrt(grav*ch2d(xx,yy)));
          double cd = c*0.5;
          double qd = cu2r(xx,yy)*ch2d(xx,yy);
          double qg = cu2l(xx,yy)*ch2g(xx,yy);
          cg1(xx,yy)=(qg+qd)*0.5-cd*(ch2d(xx,yy)-ch2g(xx,yy));
          cg2(xx,yy)=((cu2l(xx,yy)*qg)+(grav_dem*ch2g(xx,yy)*ch2g(xx,yy))+(cu2r(xx,yy)*qd)+(grav_dem*ch2d(xx,yy)*ch2d(xx,yy)))*0.5-cd*(qd-qg);
          cg3(xx,yy)=(qg*cv2l(xx,yy)+qd*cv2r(xx,yy))*0.5-cd*(ch2d(xx,yy)*cv2r(xx,yy)-ch2g(xx,yy)*cv2l(xx,yy));
        }
      }
    }
    //b =rdtsc();
    //std::cout<<"bloc1y3 rdtsc = "<<b-a<<std::endl;
  }

};

#include <Kernels/Kbloc1y3_bloc1y4_bloc1y5.l2c>
