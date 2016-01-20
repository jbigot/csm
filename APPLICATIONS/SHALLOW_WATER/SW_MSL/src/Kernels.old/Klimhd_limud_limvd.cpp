#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBD.hpp>
#include <kernelh.h>

#include "../rdtsc.h"

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimhd_limud_limvd:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;

  Klimhd_limud_limvd(){}
  ~Klimhd_limud_limvd(){}
  
  void go ()
  {
    //unsigned long long a,b;
    //a=rdtsc();

    //cout<<"====START Klimhd_limud_limvd===="<<endl;
    ControllerPHBD<double> ch(h);
    ControllerPHBD<double> cu(u);
    ControllerPHBD<double> cv(v);
    
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx,yy-1);
        cu(xx,yy) = -cu(xx,yy-1);
        cv(xx,yy) = cv(xx,yy-1);
	     }                                                                                                                                                                                           
      }

    //b =rdtsc();
    //std::cout<<"limhd rdtsc = "<<b-a<<std::endl;
  }

};

#include <Kernels/Klimhd_limud_limvd.l2c>
