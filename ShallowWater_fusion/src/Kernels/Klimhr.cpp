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
  Datah* u;
  Datah* v;

  Klimhr(){}
  ~Klimhr(){}
  
  void go ()
  {
    //cout<<"====START Klimhr===="<<endl;
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
  }

};

#include <Kernels/Klimhr.l2c>
