#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBR.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimhr:virtual public Go
{
public:
  Datah* h;

  Klimhr(){}
  ~Klimhr(){}
  
  void go ()
  {
    //cout<<"====START Klimhr===="<<endl;
  	ControllerPHBR<double> ch(h);
    
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx-1,yy);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                      
  }

};

#include <Kernels/Klimhr.l2c>
