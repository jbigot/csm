#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBL.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimhl_limul_limvl_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;

  Klimhl_limul_limvl_omp(){}
  ~Klimhl_limul_limvl_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimhl_limul_limvl===="<<endl;
  	ControllerPHBL<double> ch(h);
    ControllerPHBL<double> cu(u);
    ControllerPHBL<double> cv(v);
    
    //std::cout<<"max xx="<<ch.width()<<" et max yy="<<ch.height()<<std::endl;

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(ch,cu,cv) private(yy,xx) schedule (static, CHUNK)
    for(yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx+1,yy);
        cu(xx,yy) = -cu(xx+1,yy);
        cv(xx,yy) = cv(xx+1,yy);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                    
  }

};

#include <KernelsOmp/Klimhl_limul_limvl_omp.l2c>
