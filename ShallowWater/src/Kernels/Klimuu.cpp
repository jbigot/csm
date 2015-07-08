#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBU.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimuu:virtual public Go
{
public:
  Datah* u;

  Klimuu(){}
  ~Klimuu(){}
  
  void go ()
  {
    //cout<<"====START Klimuu===="<<endl;
  	ControllerPHBU<double> cu(u);
    
    for(int64_t yy = cu.startHeight(); yy<cu.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cu.startWidth();xx<cu.width(); xx++)
	     {
	     	cu(xx,yy) = -cu(xx,yy+1);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                 
  }

};

#include <Kernels/Klimuu.l2c>
