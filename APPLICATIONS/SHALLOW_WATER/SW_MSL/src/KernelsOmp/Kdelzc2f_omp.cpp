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

class Kdelzc2f_omp:virtual public Go
{
public:
  Datah* z2r;
  Datah* z2l;
  Datah* delzc2;

  Kdelzc2f_omp(){}
  ~Kdelzc2f_omp(){}
  
  void go ()
  {
    //cout<<"====START Kdelzc2f===="<<endl;
    Controller<double> cz2l(z2l);
    Controller<double> cz2r(z2r);
    Controller<double> cdelzc2(delzc2);

//dynamic also possible
#pragma omp parallel for shared(cz2l,cz2r,cdelzc2) private(yy,xx) schedule (static, CHUNK)
    for(int64_t yy = cdelzc2.start(); yy<cdelzc2.height();yy++)
       for(int64_t xx = cdelzc2.start();xx<cdelzc2.width(); xx++)
          cdelzc2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy+1);
  }

};

#include <KernelsOmp/Kdelzc2f_omp.l2c>
