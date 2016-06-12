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

class Kbloc1y1_omp:virtual public Go
{
public:
  Datah* h2l;
  Datah* delz2;
  Datah* h2g;

  Kbloc1y1_omp(){}
  ~Kbloc1y1_omp(){}
  
  void go ()
  {
    //cout<<"====START Kbloc1y1===="<<endl;
    Controller<double> ch2l(h2l);
    Controller<double> cdelz2(delz2);
    Controller<double> ch2g(h2g);

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(ch2l,cdelz2,ch2g) private(yy,xx) schedule (static, CHUNK)
    for(yy = ch2l.start(); yy<ch2l.height();yy++)
    {
      for(xx = ch2l.start();xx<ch2l.width(); xx++)
      {
        ch2g(xx,yy)=max(0.,ch2l(xx,yy)-max(0.,cdelz2(xx,yy)));
      }
    }
  }

};

#include <KernelsOmp/Kbloc1y1_omp.l2c>
