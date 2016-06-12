#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Kdelzf_omp:virtual public Go
{
public:
  Datah* z1l;
  Datah* z1r;
  Datah* delz1;

  Kdelzf_omp(){}
  ~Kdelzf_omp(){}
  
  void go ()
  {
    //cout<<"====START Kdelzf===="<<endl;
    Controller<double> cz1l(z1l);
    Controller<double> cz1r(z1r);
    Controller<double> cdelz1(delz1);

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(cz1l,cz1r,cdelz1) private(yy,xx) schedule (static, CHUNK)
    for(yy = cdelz1.start(); yy<cdelz1.height();yy++)
       for(xx = cdelz1.start();xx<cdelz1.width(); xx++)
          cdelz1(xx,yy) = cz1r(xx,yy) - cz1l(xx,yy);
  }

};

#include <KernelsOmp/Kdelzf_omp.l2c>
