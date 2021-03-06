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

class Kbloc1x1_bloc1x2:virtual public Go
{
public:
  Datah* h1l;
  Datah* h1r;
  // Datah* u1l;
  // Datah* u1r;
  // Datah* v1l;
  // Datah* v1r;
  Datah* delz1;
  Datah* h1g;
  Datah* h1d;
  // Datah* f1;
  // Datah* f2;
  // Datah* f3;

  Kbloc1x1_bloc1x2(){}
  ~Kbloc1x1_bloc1x2(){}
  
  void go ()
  {
    //cout<<"====START Kbloc1x1_bloc1x2===="<<endl;
    Controller<double> ch1l(h1l);
    Controller<double> ch1r(h1r);
    // Controller<double> cu1l(u1l);
    // Controller<double> cu1r(u1r);
    // Controller<double> cv1l(v1l);
    // Controller<double> cv1r(v1r);
    Controller<double> cdelz1(delz1);
    Controller<double> ch1g(h1g);
    Controller<double> ch1d(h1d);
    // Controller<double> cf1(f1);
    // Controller<double> cf2(f2);
    // Controller<double> cf3(f3);

    for(int64_t yy = ch1l.start(); yy<ch1l.height();yy++)
    {
      for(int64_t xx = ch1l.start();xx<ch1l.width(); xx++)
      {
        ch1g(xx,yy)=max(0.,ch1l(xx,yy)-max(0.,cdelz1(xx,yy)));
        ch1d(xx,yy)=max(0.,ch1r(xx,yy)-max(0.,-cdelz1(xx,yy)));
        // if (ch1g(xx,yy)<=0. && ch1d(xx,yy)<=0.)
        // {
        //   cf1(xx,yy)=0.;
        //   cf2(xx,yy)=0.;
        //   cf3(xx,yy)=0.;
        // }
        // else
        // {
        //   double c = max(fabs(cu1l(xx,yy))+sqrt(grav*ch1g(xx,yy)),fabs(cu1r(xx,yy))+sqrt(grav*ch1d(xx,yy)));
        //   double cd = c*0.5;
        //   double qd = cu1r(xx,yy)*ch1d(xx,yy);
        //   double qg = cu1l(xx,yy)*ch1g(xx,yy);
        //   cf1(xx,yy)=(qg+qd)*0.5-cd*(ch1d(xx,yy)-ch1g(xx,yy));
        //   cf2(xx,yy)=((cu1l(xx,yy)*qg)+(grav_dem*ch1g(xx,yy)*ch1g(xx,yy))+(cu1r(xx,yy)*qd)+(grav_dem*ch1d(xx,yy)*ch1d(xx,yy)))*0.5-cd*(qd-qg);
        //   cf3(xx,yy)=(qg*cv1l(xx,yy)+qd*cv1r(xx,yy))*0.5-cd*(ch1d(xx,yy)*cv1r(xx,yy)-ch1g(xx,yy)*cv1l(xx,yy));
        // }
      }
    }                                                                                                                                                                                    
  }

};

#include <Kernels/Kbloc1x1_bloc1x2.l2c>
