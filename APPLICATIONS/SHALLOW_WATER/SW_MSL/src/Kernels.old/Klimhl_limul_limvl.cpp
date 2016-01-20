#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include "../rdtsc.h"

#include <datah.h>
#include <controllerPHBL.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimhl_limul_limvl:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;

  Klimhl_limul_limvl(){}
  ~Klimhl_limul_limvl(){}
  
  void go ()
  {
    //unsigned long long a,b;
    //a=rdtsc();

    //cout<<"====START Klimhl_limul_limvl===="<<endl;
  	ControllerPHBL<double> ch(h);
    ControllerPHBL<double> cu(u);
    ControllerPHBL<double> cv(v);
    
    //std::cout<<"max xx="<<ch.width()<<" et max yy="<<ch.height()<<std::endl;
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx+1,yy);
        cu(xx,yy) = -cu(xx+1,yy);
        cv(xx,yy) = cv(xx+1,yy);
	     }                                                                                                                                                                                           
      }

    //b =rdtsc();
    //std::cout<<"limhl rdtsc = "<<b-a<<std::endl;
  }

};

#include <Kernels/Klimhl_limul_limvl.l2c>
