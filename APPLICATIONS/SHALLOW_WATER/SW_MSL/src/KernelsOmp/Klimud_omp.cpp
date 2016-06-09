#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBD.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimud_omp:virtual public Go
{
public:
  Datah* u;

  Klimud_omp(){}
  ~Klimud_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimud===="<<endl;
  	ControllerPHBD<double> cu(u);
    
//dynamic also possible
#pragma omp parallel for shared(cu) private(yy,xx) schedule (static, CHUNK)
    for(int64_t yy = cu.startHeight(); yy<cu.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cu.startWidth();xx<cu.width(); xx++)
	     {
	     	cu(xx,yy) = -cu(xx,yy-1);
	     }                                                                                                                                                                                           
      }                  
  }

};

#include <KernelsOmp/Klimud_omp.l2c>
