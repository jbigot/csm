#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBR.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimur_omp:virtual public Go
{
public:
  Datah* u;

  Klimur_omp(){}
  ~Klimur_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimur===="<<endl;
  	ControllerPHBR<double> cu(u);

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(cu) private(yy,xx) schedule (static, CHUNK)
    for(yy = cu.startHeight(); yy<cu.height();yy++)                                                                                                               
      {
	     for(xx = cu.startWidth();xx<cu.width(); xx++)
	     {
	     	cu(xx,yy) = -cu(xx-1,yy);
	     }                                                                                                                                                                                           
      }                  
  }

};

#include <KernelsOmp/Klimur_omp.l2c>
