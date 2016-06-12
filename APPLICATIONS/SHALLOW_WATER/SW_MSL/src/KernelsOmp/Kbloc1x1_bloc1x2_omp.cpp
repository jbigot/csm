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

class Kbloc1x1_bloc1x2_omp:virtual public Go
{
public:
  Datah* h1l;
  Datah* h1r;
  Datah* delz1;
  Datah* h1g;
  Datah* h1d;

  Kbloc1x1_bloc1x2_omp(){}
  ~Kbloc1x1_bloc1x2_omp(){}
  
  void go ()
  {
    //cout<<"====START Kbloc1x1_bloc1x2===="<<endl;
    Controller<double> ch1l(h1l);
    Controller<double> ch1r(h1r);
    Controller<double> cdelz1(delz1);
    Controller<double> ch1g(h1g);
    Controller<double> ch1d(h1d);

    int64_t xx,yy;

#pragma omp parallel for shared(ch1l,ch1r,cdelz1,ch1g,ch1d) private(yy,xx) schedule (static, CHUNK) //dynamic also possible
    for(yy = ch1l.start(); yy<ch1l.height();yy++)
    {
      for(xx = ch1l.start();xx<ch1l.width(); xx++)
      {
        ch1g(xx,yy)=max(0.,ch1l(xx,yy)-max(0.,cdelz1(xx,yy)));
        ch1d(xx,yy)=max(0.,ch1r(xx,yy)-max(0.,-cdelz1(xx,yy)));
      }
    }                                                                                                                                                                                    
  }

};

#include <KernelsOmp/Kbloc1x1_bloc1x2_omp.l2c>
