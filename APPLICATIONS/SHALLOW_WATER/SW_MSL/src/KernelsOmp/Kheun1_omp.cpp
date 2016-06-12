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

class Kheun1_omp:virtual public Go
{
public:
  Datah* hsa;
  Datah* h;

  Kheun1_omp(){}
  ~Kheun1_omp(){}
  
  void go ()
  {
    //cout<<"====START Kheun1===="<<endl;
    Controller<double> chsa(hsa);
    Controller<double> ch(h);

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(chsa,ch) private(yy,xx) schedule (static, CHUNK)
    for(yy = ch.start(); yy<ch.height();yy++)
    {
      //int64_t xxq=cq1.start();
      for(xx = ch.start();xx<ch.width(); xx++)
      {
        ch(xx,yy)=0.5*(ch(xx,yy)+chsa(xx,yy));
      }
    }
  }

};

#include <KernelsOmp/Kheun1_omp.l2c>
