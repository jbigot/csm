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

class Kdelzc1f_omp:virtual public Go
{
public:
  Datah* z1l;
  Datah* z1r;
  Datah* delzc1;

  Kdelzc1f_omp(){}
  ~Kdelzc1f_omp(){}
  
  void go ()
  {
    //cout<<"====START Kdelzc1f===="<<endl;
    Controller<double> cz1l(z1l);
    Controller<double> cz1r(z1r);
    Controller<double> cdelzc1(delzc1);

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(cz1l,cz1r,cdelzc1) private(yy,xx) schedule (static, CHUNK)
    for(yy = cdelzc1.start(); yy<cdelzc1.height();yy++)
       for(xx = cdelzc1.start();xx<cdelzc1.width(); xx++)
          cdelzc1(xx,yy) = cz1r(xx+1,yy) - cz1l(xx,yy);
  }

};

#include <KernelsOmp/Kdelzc1f_omp.l2c>
