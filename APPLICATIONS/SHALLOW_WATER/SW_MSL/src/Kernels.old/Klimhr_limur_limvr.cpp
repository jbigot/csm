#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBR.hpp>
#include <kernelh.h>

#include "../rdtsc.h"

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimhr_limur_limvr:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;

  Klimhr_limur_limvr(){}
  ~Klimhr_limur_limvr(){}
  
  void go ()
  {
    //unsigned long long a,b;
    //a=rdtsc();

    //cout<<"====START Klimhr_limur_limvr===="<<endl;
    ControllerPHBR<double> ch(h);
    ControllerPHBR<double> cu(u);
    ControllerPHBR<double> cv(v);
    
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx-1,yy);
        cu(xx,yy) = -cu(xx-1,yy);
        cv(xx,yy) = cv(xx-1,yy);
	     }                                                                                                                                                                                           
      }

    //b =rdtsc();
    //std::cout<<"limhr rdtsc = "<<b-a<<std::endl;
  }

};

#include <Kernels/Klimhr_limur_limvr.l2c>
