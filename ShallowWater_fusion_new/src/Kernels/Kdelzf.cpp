#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>
#include <chronometer.hpp>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Kdelzf:virtual public Go
{
public:
  Datah* z1l;
  Datah* z1r;
  Datah* delz1;

  Kdelzf(){}
  ~Kdelzf(){}
  
  void go ()
  {
    //cout<<"====START Kdelzf===="<<endl;
    Controller<double> cz1l(z1l);
    Controller<double> cz1r(z1r);
    Controller<double> cdelz1(delz1);

    int64_t xx,yy;
    int thread=0;

#pragma omp parallel shared(cz1l,cz1r,cdelz1) private(yy,xx,thread)
    {
    Chronometer chrono;
    chrono.start();

#pragma omp for schedule(static)
    for(yy = cdelz1.start(); yy<cdelz1.height();yy++)
       for(xx = cdelz1.start();xx<cdelz1.width(); xx++)
          cdelz1(xx,yy) = cz1r(xx,yy) - cz1l(xx,yy);

    chrono.stop();
    cout<<"Thread "<<thread<<" - Time bloc2 cahck : "<<chrono.dureeCalcule()<<endl;                                                                                                                                          
    }
  }

};

#include <Kernels/Kdelzf.l2c>
