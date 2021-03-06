#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBU.hpp>
#include <kernelh.h>

#include "../rdtsc.h"

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimhu_limuu_limvu:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;

  Klimhu_limuu_limvu(){}
  ~Klimhu_limuu_limvu(){}
  
  void go ()
  {
    //unsigned long long a,b;
    //a=rdtsc();

    //cout<<"====START Klimhu_limuu_limvu===="<<endl;
    ControllerPHBU<double> ch(h);
    ControllerPHBU<double> cu(u);
    ControllerPHBU<double> cv(v);
    
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx,yy+1);
        cu(xx,yy) = -cu(xx,yy+1);
        cv(xx,yy) = cv(xx,yy+1);
	     }                                                                                                                                                                                           
      }

    //b =rdtsc();
    //std::cout<<"limhu rdtsc = "<<b-a<<std::endl;
  }

};

#include <Kernels/Klimhu_limuu_limvu.l2c>
