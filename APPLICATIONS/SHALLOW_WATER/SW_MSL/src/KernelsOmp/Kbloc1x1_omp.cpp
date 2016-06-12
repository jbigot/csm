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

class Kbloc1x1_omp:virtual public Go
{
public:
  Datah* h1l;
  Datah* delz1;
  Datah* h1g;

  Kbloc1x1_omp(){}
  ~Kbloc1x1_omp(){}
  
  void go ()
  {
    //cout<<"====START Kbloc1x1===="<<endl;
    Controller<double> ch1l(h1l);
    Controller<double> cdelz1(delz1);
    Controller<double> ch1g(h1g);

    int64_t xx,yy;

#pragma omp parallel for shared(ch1l,cdelz1,ch1g) private(yy,xx) schedule (static, CHUNK) //dynamic also possible
    for(yy = ch1l.start(); yy<ch1l.height();yy++)
    {
      for(xx = ch1l.start();xx<ch1l.width(); xx++)
      {
        ch1g(xx,yy)=max(0.,ch1l(xx,yy)-max(0.,cdelz1(xx,yy)));
      }
    }
  }

};

#include <KernelsOmp/Kbloc1x1_omp.l2c>
