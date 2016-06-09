#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBU.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimuu_omp:virtual public Go
{
public:
  Datah* u;

  Klimuu_omp(){}
  ~Klimuu_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimuu===="<<endl;
    ControllerPHBU<double> cu(u);

//dynamic also possible
#pragma omp parallel for shared(cu) private(yy,xx) schedule (static, CHUNK)
    for(int64_t yy = cu.startHeight(); yy<cu.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cu.startWidth();xx<cu.width(); xx++)
	     {
	     	cu(xx,yy) = -cu(xx,yy+1);
	     }                                                                                                                                                                                           
      }             
  }

};

#include <KernelsOmp/Klimuu_omp.l2c>
