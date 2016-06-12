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

#define he_ca 1.e-6
#define ve_ca 1.e-6

class Kcheck_omp:virtual public Go
{
public:
  Datah* hs;

  Kcheck_omp(){}
  ~Kcheck_omp(){}
  
  void go ()
  {
    //cout<<"====START Kcheck===="<<endl;
    Controller<double> chs(hs);

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(chs) private(yy,xx) schedule (static, CHUNK)
    for(yy = chs.start(); yy<chs.height();yy++)
    {
      for(xx = chs.start();xx<chs.width(); xx++)
      {
        if(chs(xx,yy)<he_ca)
        {
          chs(xx,yy)=0.;
        }
      }
    }
  }

};

#include <KernelsOmp/Kcheck_omp.l2c>
