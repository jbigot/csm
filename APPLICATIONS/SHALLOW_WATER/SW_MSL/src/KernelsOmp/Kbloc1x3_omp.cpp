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

class Kbloc1x3_omp:virtual public Go
{
public:
  Datah* u1l;
  Datah* u1r;
  Datah* h1g;
  Datah* h1d;
  Datah* f1;

  Kbloc1x3_omp(){}
  ~Kbloc1x3_omp(){}
  
  void go ()
  {
    //cout<<"====START Kbloc1x3===="<<endl;
    Controller<double> cu1l(u1l);
    Controller<double> cu1r(u1r);
    Controller<double> ch1g(h1g);
    Controller<double> ch1d(h1d);
    Controller<double> cf1(f1);

    int64_t xx,yy;
    double c,cd,qd,qg;

//dynamic also possible
#pragma omp parallel for shared(cu1l,cu1r,ch1g,ch1d,cf1) private(yy,xx,c,cd,qd,qg) schedule (static, CHUNK)
    for(yy = cu1l.start(); yy<cu1l.height();yy++)
    {
      for(xx = cu1l.start();xx<cu1l.width(); xx++)
      {
        if (ch1g(xx,yy)<=0. && ch1d(xx,yy)<=0.)
        {
          cf1(xx,yy)=0.;
        }
        else
        {
          c = max(fabs(cu1l(xx,yy))+sqrt(grav*ch1g(xx,yy)),fabs(cu1r(xx,yy))+sqrt(grav*ch1d(xx,yy)));
          cd = c*0.5;
          qd = cu1r(xx,yy)*ch1d(xx,yy);
          qg = cu1l(xx,yy)*ch1g(xx,yy);
          cf1(xx,yy)=(qg+qd)*0.5-cd*(ch1d(xx,yy)-ch1g(xx,yy));
        }
      }
    }                                                                                                                                                                 /*        
    chrono.stop();
    cout<<mpi_rank<<" compute Kbloc1x3 : "<<chrono.dureeCalcule()<<endl;*/
  }

};

#include <KernelsOmp/Kbloc1x3_omp.l2c>
