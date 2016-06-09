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

class Kdelzc1f_delzc2f_omp:virtual public Go
{
public:
  Datah* z1l;
  Datah* z1r;
  Datah* delzc1;
  Datah* z2r;
  Datah* z2l;
  Datah* delzc2;

  Kdelzc1f_delzc2f_omp(){}
  ~Kdelzc1f_delzc2f_omp(){}
  
  void go ()
  {
  	//cout<<"====START Kdelzc1f_delzc2f===="<<endl;
  	Controller<double> cz1l(z1l);
    Controller<double> cz1r(z1r);
    Controller<double> cdelzc1(delzc1);
    Controller<double> cz2l(z2l);
    Controller<double> cz2r(z2r);
    Controller<double> cdelzc2(delzc2);

//dynamic also possible
#pragma omp parallel for shared(cz1l,cz1r,cdelzc1,cz2l,cz2r,cdelzc2) private(yy,xx) schedule (static, CHUNK)
    for(int64_t yy = cdelzc1.start(); yy<cdelzc1.height();yy++)
      {
       for(int64_t xx = cdelzc1.start();xx<cdelzc1.width(); xx++)
	 {
          cdelzc1(xx,yy) = cz1r(xx+1,yy) - cz1l(xx,yy);
          cdelzc2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy+1);
	 }
      }
  }
};

#include <KernelsOmp/Kdelzc1f_delzc2f_omp.l2c>
