#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBD.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimud:virtual public Go
{
public:
  Datah* u;

  Klimud(){}
  ~Klimud(){}
  
  void go ()
  {
    //cout<<"====START Klimud===="<<endl;
  	ControllerPHBD<double> cu(u);
    
    for(int64_t yy = cu.startHeight(); yy<cu.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cu.startWidth();xx<cu.width(); xx++)
	     {
	     	cu(xx,yy) = -cu(xx,yy-1);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                     
  }

};

#include <Kernels/Klimud.l2c>
