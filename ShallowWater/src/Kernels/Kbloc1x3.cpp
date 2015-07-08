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

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)
#define grav 9.81
#define grav_dem 4.905

class Kbloc1x3:virtual public Go
{
public:
  Datah* u1l;
  Datah* u1r;
  Datah* h1g;
  Datah* h1d;
  Datah* f1;

  Kbloc1x3(){}
  ~Kbloc1x3(){}
  
  void go ()
  {
    //cout<<"====START Kbloc1x3===="<<endl;
    Controller<double> cu1l(u1l);
    Controller<double> cu1r(u1r);
    Controller<double> ch1g(h1g);
    Controller<double> ch1d(h1d);
    Controller<double> cf1(f1);

    for(int64_t yy = cu1l.start(); yy<cu1l.height();yy++)
    {
      for(int64_t xx = cu1l.start();xx<cu1l.width(); xx++)
      {
        if (ch1g(xx,yy)<=0. && ch1d(xx,yy)<=0.)
        {
          cf1(xx,yy)=0.;
        }
        else
        {
          double c = max(fabs(cu1l(xx,yy))+sqrt(grav*ch1g(xx,yy)),fabs(cu1r(xx,yy))+sqrt(grav*ch1d(xx,yy)));
          double cd = c*0.5;
          double qd = cu1r(xx,yy)*ch1d(xx,yy);
          double qg = cu1l(xx,yy)*ch1g(xx,yy);
          cf1(xx,yy)=(qg+qd)*0.5-cd*(ch1d(xx,yy)-ch1g(xx,yy));
        }
      }
    }                                                                                                                                                                                    
  }

};

#include <Kernels/Kbloc1x3.l2c>
