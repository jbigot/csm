#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>

#include "../rdtsc.h"

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
    //unsigned long long a,b;
    //a=rdtsc();

    //cout<<"====START Kdelzf===="<<endl;
    Controller<double> cz1l(z1l);
    Controller<double> cz1r(z1r);
    Controller<double> cdelz1(delz1);

    for(int64_t yy = cdelz1.start(); yy<cdelz1.height();yy++)
       for(int64_t xx = cdelz1.start();xx<cdelz1.width(); xx++)
          cdelz1(xx,yy) = cz1r(xx,yy) - cz1l(xx,yy);                                                                                                                                             

    //b =rdtsc();
    //std::cout<<"delzf rdtsc = "<<b-a<<std::endl;
  }

};

#include <Kernels/Kdelzf.l2c>
